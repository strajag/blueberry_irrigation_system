<?php

set_time_limit(60); // timeout time
ini_set('max_execution_time', '70'); // script execution time will not timeout instead will kill so timeout message will not be recieved if Jsoup.timeout is 0 as Jsoup will stuck forever then
ignore_user_abort(true); // if user abort connection with php, php will execute script till end anyway

$receivedBytes = file_get_contents('php://input');
$RMID = $receivedBytes[1];
$RSID = $receivedBytes[2];
$pcPhpAndMainArduinoMID = chr(0);
$pcSID = chr(255);
$phpSID = chr(254);
$mainArduinoSID = chr(0);
$startAndEndByte = chr(255);

class CommandEnum
{
	const CHECK_IS_SENT_BYTES_READED = 0;
	const DELETE_SENT_BYTES = 1;
	const GET_BYTES = 254;
}

$lockFile = fopen('lock', 'r');
while (!flock($lockFile, LOCK_EX|LOCK_NB)); // add timeout

if ($RMID === $pcPhpAndMainArduinoMID && $RSID === $pcSID) // PC ID
{
	if (ord($receivedBytes[0]) !== 2)
	{
		$fileBytes = '';
		if (file_exists('./arduino'))
			$fileBytes = base64_decode(file_get_contents('./arduino'));
		file_put_contents('./arduino', base64_encode($fileBytes . $receivedBytes));
	}
	sendPcBytesToArduino();
}
else if ($RMID === $pcPhpAndMainArduinoMID && $RSID === $phpSID) // PHP ID
{
	processPhpCommands();
}
else // ARDUINO ID
{
	$fileBytes = '';
	if (file_exists('./pc'))
		$fileBytes = base64_decode(file_get_contents('./pc'));
	file_put_contents('./pc', base64_encode($fileBytes . $receivedBytes));
	sendArduinoBytesToPc();
}

flock($lockFile, LOCK_UN);
fclose($lockFile);

function processPhpCommands()
{
	global $receivedBytes, $pcPhpAndMainArduinoMID, $pcSID;
	$SMID = $receivedBytes[3];
	$SSID = $receivedBytes[4];
	$command = $receivedBytes[5];

	if ($command === chr(CommandEnum::GET_BYTES))
	{
		if ($SMID === $pcPhpAndMainArduinoMID && $SSID === $pcSID) // PC ID
			sendArduinoBytesToPc();
		else // ARDUINO ID
			sendPcBytesToArduino();
	} 
	else if ($command === chr(CommandEnum::CHECK_IS_SENT_BYTES_READED))
	{
		$answer;
		if ($SMID === $pcPhpAndMainArduinoMID && $SSID === $pcSID) // PC ID
		{
			if (file_exists('./pc'))
				$answer = chr(0);
			else
				$answer = chr(1);
		}
		else // ARDUINO ID
		{
			if (file_exists('./arduino'))
				$answer = chr(0);
			else
				$answer = chr(1);
		}
		sendPhpBytes($SMID, $SSID, chr(CommandEnum::CHECK_IS_SENT_BYTES_READED) . $answer);
	} 
	else if ($command === chr(CommandEnum::DELETE_SENT_BYTES))
	{
		$answer;
		if ($SMID === $pcPhpAndMainArduinoMID && $SSID === $pcSID) // PC ID
		{
			if (file_exists('./pc'))
			{ 
				unlink('./pc');
				$answer = chr(1); 
			} 
			else
				$answer = chr(0);
		} else // ARDUINO ID
		{
			if (file_exists('./arduino'))
			{
				unlink('./arduino');
				$answer = chr(1);
			}
			else
				$answer = chr(0);
			sendPhpBytes($SMID, $SSID, chr(CommandEnum::DELETE_SENT_BYTES) . $answer);
		}
	}
}

function sendPcBytesToArduino()
{
	global $pcPhpAndMainArduinoMID, $mainArduinoSID, $startAndEndByte;

	if (file_exists('./pc'))
	{
		$fileBytes = base64_decode(file_get_contents('./pc'));
		echo $startAndEndByte . chr(strlen($fileBytes));
		echo $fileBytes;
		echo $startAndEndByte;
		unlink('./pc');
	}
	else
	{
		//sendPhpBytes($pcPhpAndMainArduinoMID, $mainArduinoSID, chr(CommandEnum::GET_BYTES) . chr(0));
		echo $startAndEndByte . chr(0) . $startAndEndByte;
	}
}

function sendArduinoBytesToPc()
{
	global $pcPhpAndMainArduinoMID, $pcSID;

	if (file_exists('./arduino'))
	{
		$fileBytes = base64_decode(file_get_contents('./arduino'));
		$fileBytesTotalLength = strlen($fileBytes);
		$fileBytesPartLength;
		$byteCounter = 0;

		while ($byteCounter < $fileBytesTotalLength)
		{
			$fileBytesPartLength = ord($fileBytes[$byteCounter++]);
			$fileBytesPartLength -= 2;
			echo chr($fileBytesPartLength);
			$byteCounter += 2;
			for ($i = 0; $i < $fileBytesPartLength; $i++) { echo $fileBytes[$byteCounter++]; }	
		}
		unlink('./arduino');
	}
	else
		sendPhpBytes($pcPhpAndMainArduinoMID, $pcSID, chr(CommandEnum::GET_BYTES) . chr(0));
}

function sendPhpBytes($RMID, $RSID, $bytes)
{
	global $pcPhpAndMainArduinoMID, $pcSID, $phpSID, $startAndEndByte;
	
	if ($RMID === $pcPhpAndMainArduinoMID && $RSID === $pcSID) // PC ID
	{
		$bytesLength = strlen($bytes) + 2; // +2 is for SMID and SSID (strlen($bytes) + 2)
		echo chr($bytesLength) . $pcPhpAndMainArduinoMID . $phpSID . $bytes;
	}
	else // ARDUINO ID
	{
		$bytesLength = strlen($bytes) + 4; // +4 is for RMID, RSID, SMID and SSID (strlen($bytes) + 4)
		echo $startAndEndByte . chr($bytesLength + 1) . chr($bytesLength) . $RMID . $RSID . $pcPhpAndMainArduinoMID . $phpSID . $bytes . $startAndEndByte;
	}
}

?>
