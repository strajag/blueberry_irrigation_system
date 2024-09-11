// header
var anchorDivs = document.getElementsByClassName("heightDiv");

function headerItemClick(page)
{
	anchorDivs[page].scrollIntoView();
}
// home
var divElement = document.getElementById("homeBotLeftDiv");
var divElement2 = document.getElementById("homeBotCenterDiv");
var divElement3 = document.getElementById("homeBotRightDiv");
var pNameElements = [];
var pValueElements = [];
var inputElements = [];

initialize();

function initialize()
{
	for (var i = 0; i < 20; i++)
	{
		pNameElements[i] = document.createElement("p");
		pNameElements[i].className = "homeBotLeftP";
		divElement.appendChild(pNameElements[i]);
		
		pValueElements[i] = document.createElement("p");
		pValueElements[i].className = "homeBotCenterDivP";
		pValueElements[i].innerText = "n/a";
		divElement2.appendChild(pValueElements[i]);
		
		inputElements[i] = document.createElement("input");
		inputElements[i].className = "homeBotRightInput";
		divElement3.appendChild(inputElements[i]);
	}

	pNameElements[0].appendChild(document.createTextNode("generator:"));
	pNameElements[1].appendChild(document.createTextNode("valve 1:"));
	pNameElements[2].appendChild(document.createTextNode("valve 2:"));
	pNameElements[3].appendChild(document.createTextNode("valve 3:"));
	pNameElements[4].appendChild(document.createTextNode("pump 1 (220v):"));
	pNameElements[5].appendChild(document.createTextNode("pump 2 (12v):"));
	pNameElements[6].appendChild(document.createTextNode("pump 3 (acid):"));
	pNameElements[7].appendChild(document.createTextNode("program:"));
	pNameElements[8].appendChild(document.createTextNode("program duration:"));
	pNameElements[9].appendChild(document.createTextNode("cleaning duration:"));
	pNameElements[10].appendChild(document.createTextNode("pump 1 duration:"));
	pNameElements[11].appendChild(document.createTextNode("pump 2 duration:"));
	pNameElements[12].appendChild(document.createTextNode("pump 1 volume:"));
	pNameElements[13].appendChild(document.createTextNode("pump 2 volume:"));
	pNameElements[14].appendChild(document.createTextNode("pump 1 pulses/l:"));
	pNameElements[15].appendChild(document.createTextNode("pump 2 pulses/l:"));
	pNameElements[16].appendChild(document.createTextNode("pump 1 delivered: "));
	pNameElements[17].appendChild(document.createTextNode("pump 2 delivered: "));
	pNameElements[18].appendChild(document.createTextNode("program time: "));
	pNameElements[19].appendChild(document.createTextNode("test: "));
	
	inputElements[16].style.visibility = "hidden";
	inputElements[17].style.visibility = "hidden";
	inputElements[18].style.visibility = "hidden";
}

// var inputElement = document.getElementById("homeInput");
// inputElement.addEventListener("keypress", function(event) { if (event.key === "Enter") { event.preventDefault(); sendClick(); } } );
// var intervalID = setInterval(ethernetLoop, 2000);
var sendBytes;
var filteredArrayIndexCounter = 0;

function sendClick()
{
	// if (inputElement.value === "start")
	// {
		// if (!intervalID)
			// intervalID = setInterval(ethernetLoop, 2000);
	// }
	// else if (inputElement.value === "stop")
	// {
		// if (intervalID)
		// {
			// clearInterval(intervalID); 
			// intervalID = null;
		// }
	// }
		
	if (inputElements[19].value !== "")
	{
		sendBytes = new Uint8Array(7);
		sendBytes[0] = 6;
		sendBytes[1] = 0;
		sendBytes[2] = 0;
		sendBytes[3] = 0;
		sendBytes[4] = 255;
		sendBytes[5] = 0;
		sendBytes[6] = inputElements[19].value;
		filteredArrayIndexCounter = 1;
		inputElements[19].value = "";

		if (!intervalID)
			ethernetLoop();

		return;
	}
		
	filteredArrayIndexCounter = 0;
	var inputElementValue;
	var isAllNumbers;
	var filteredArray = [];

	for (var i = 0; i < inputElements.length; i++)
	{
		inputElementValue = inputElements[i].value.replaceAll(" ", "");
		isAllNumbers = /^\d+$/.test(inputElementValue);
		inputElements[i].value = "";

		if (!isAllNumbers || inputElementValue === "")
			continue;

		switch(i)
		{
			case 0: filteredArray[filteredArrayIndexCounter++] = 0; filteredArray[filteredArrayIndexCounter++] = inputElementValue; break;
			case 1: filteredArray[filteredArrayIndexCounter++] = 1; filteredArray[filteredArrayIndexCounter++] = 1; filteredArray[filteredArrayIndexCounter++] = inputElementValue; break;
			case 2: filteredArray[filteredArrayIndexCounter++] = 1; filteredArray[filteredArrayIndexCounter++] = 2; filteredArray[filteredArrayIndexCounter++] = inputElementValue; break;
			case 3: filteredArray[filteredArrayIndexCounter++] = 1; filteredArray[filteredArrayIndexCounter++] = 3; filteredArray[filteredArrayIndexCounter++] = inputElementValue; break;
			case 4: filteredArray[filteredArrayIndexCounter++] = 2; filteredArray[filteredArrayIndexCounter++] = 1; filteredArray[filteredArrayIndexCounter++] = inputElementValue; break;
			case 5: filteredArray[filteredArrayIndexCounter++] = 2; filteredArray[filteredArrayIndexCounter++] = 2; filteredArray[filteredArrayIndexCounter++] = inputElementValue; break;
			case 6: filteredArray[filteredArrayIndexCounter++] = 2; filteredArray[filteredArrayIndexCounter++] = 3; filteredArray[filteredArrayIndexCounter++] = inputElementValue; break;
			case 7: filteredArray[filteredArrayIndexCounter++] = 3; filteredArray[filteredArrayIndexCounter++] = inputElementValue; break;
			case 8: filteredArray[filteredArrayIndexCounter++] = 4; filteredArray[filteredArrayIndexCounter++] = inputElementValue; break;
			case 9: filteredArray[filteredArrayIndexCounter++] = 5; filteredArray[filteredArrayIndexCounter++] = inputElementValue; break;
			case 10: filteredArray[filteredArrayIndexCounter++] = 6; filteredArray[filteredArrayIndexCounter++] = inputElementValue; break;
			case 11: filteredArray[filteredArrayIndexCounter++] = 7; filteredArray[filteredArrayIndexCounter++] = inputElementValue; break;	
			case 12: filteredArray[filteredArrayIndexCounter++] = 8; filteredArray[filteredArrayIndexCounter++] = inputElementValue / 255; filteredArray[filteredArrayIndexCounter++] = inputElementValue % 255; break;
			case 13: filteredArray[filteredArrayIndexCounter++] = 9; filteredArray[filteredArrayIndexCounter++] = inputElementValue / 255; filteredArray[filteredArrayIndexCounter++] = inputElementValue % 255; break;		
			case 14: filteredArray[filteredArrayIndexCounter++] = 10; filteredArray[filteredArrayIndexCounter++] = inputElementValue / 255; filteredArray[filteredArrayIndexCounter++] = inputElementValue % 255; break;
			case 15: filteredArray[filteredArrayIndexCounter++] = 11; filteredArray[filteredArrayIndexCounter++] = inputElementValue / 255; filteredArray[filteredArrayIndexCounter++] = inputElementValue % 255; break;
		}
	}

	if (filteredArrayIndexCounter === 0)
		return;

	sendBytes = new Uint8Array(filteredArrayIndexCounter + 5); // +5 is for PL, RMID, RSID, SMID and SSID byte
	sendBytes[0] = filteredArrayIndexCounter + 4; // +4 is for RMID, RSID, SMID and SSID byte
	sendBytes[1] = 0;
	sendBytes[2] = 1;
	sendBytes[3] = 0;
	sendBytes[4] = 255;

	for (var i = 0; i < filteredArrayIndexCounter; i++)
		sendBytes[i + 5] = filteredArray[i];

	if (!intervalID)
		ethernetLoop();
}

function clearClick()
{
	for (var i = 0; i < inputElements.length; i++)
		inputElements[i].value = "";
}

function updateClick(butt)
{
	sendBytes = new Uint8Array(6); 
	sendBytes[0] = 5;
	sendBytes[1] = 0;
	sendBytes[2] = 1;
	sendBytes[3] = 0;
	sendBytes[4] = 255;

	if (butt === 1)
		sendBytes[5] = 15;
	else
		sendBytes[5] = 16;

	filteredArrayIndexCounter = 1;

	if (!intervalID)
		ethernetLoop();
}

var intervalID = null;
var button = document.getElementById("homeTopResumeButtonP");

function resumeClick()
{ 
	if (!intervalID)
	{
		intervalID = setInterval(ethernetLoop, 334);
		button.innerText = "PAUSE";
	} 
	else
	{
		clearInterval(intervalID); intervalID = null;
		button.innerText = "RESUME";
	}
}

var receivedBytes;
var programTimeLeft;
var startTime;
var programTimeResult;

function ethernetLoop()
{
	programTimeResult = Math.round(programTimeLeft - ((Date.now() - startTime) / 1000));
	
	if (programTimeResult >= 0)
	{
		pValueElements[18].innerText = programTimeResult;
		pValueElements[18].innerText += "s";
	}
	
	const req = new XMLHttpRequest();
	req.open("POST", "prog.php", true);
	req.responseType = "arraybuffer";
	req.setRequestHeader("Content-type", "application/octet-stream");
	req.onload = (event) =>
	{
		const arrayBuffer = req.response; // Note: not req.responseText
		if (arrayBuffer)
			receivedBytes = new Uint8Array(arrayBuffer);
	};

	if (filteredArrayIndexCounter === 0)
	{
		req.send(new Uint8Array([5, 0, 254, 0, 255, 254]));
		//req.send(new Uint8Array([5, 0, 1, 0, 255, 15]));
	}
	else
	{
		req.send(sendBytes);
		filteredArrayIndexCounter = 0;
	}
	processReceivedBytes();
}

var byteCounter = 0;

function processReceivedBytes()
{
	if (!receivedBytes)
		return;

	while (byteCounter < receivedBytes.length)
	{
		var partLength = receivedBytes[byteCounter++] - 2;
		var SMID = receivedBytes[byteCounter++];
		var SSID = receivedBytes[byteCounter++];
		partLength += byteCounter;

		while (byteCounter < partLength)
		{
			switch(receivedBytes[byteCounter++])
			{
				case 0: if (SSID === 1) { if (receivedBytes[byteCounter++] === 0) { pValueElements[0].innerText = "Isklucen"; } else { pValueElements[0].innerText = "Ukljucen"; } } else { pValueElements[19].innerText = receivedBytes[byteCounter++]; } break;
				case 1: var temp = receivedBytes[byteCounter++]; if (temp === 1) { if (receivedBytes[byteCounter++] === 0) { pValueElements[1].innerText = "Isklucen"; } else { pValueElements[1].innerText = "Ukljucen"; } } else if (temp === 2) { if (receivedBytes[byteCounter++] === 0) { pValueElements[2].innerText = "Isklucen"; } else { pValueElements[2].innerText = "Ukljucen"; } } else if (temp === 3) { if (receivedBytes[byteCounter++] === 0) { pValueElements[3].innerText = "Isklucen"; } else { pValueElements[3].innerText = "Ukljucen"; } } break;
				case 2: var temp = receivedBytes[byteCounter++]; if (temp === 1) { if (receivedBytes[byteCounter++] === 0) { pValueElements[4].innerText = "Isklucen"; } else { pValueElements[4].innerText = "Ukljucen"; } } else if (temp === 2) { if (receivedBytes[byteCounter++] === 0) { pValueElements[5].innerText = "Isklucen"; } else { pValueElements[5].innerText = "Ukljucen"; } } else if (temp === 3) { if (receivedBytes[byteCounter++] === 0) { pValueElements[6].innerText = "Isklucen"; } else { pValueElements[6].innerText = "Ukljucen"; } } break;
				case 3: var temp = receivedBytes[byteCounter++]; if (temp === 0) { pValueElements[7].innerText = "Zavrsen"; } else { pValueElements[7].innerText = temp; } break;
				case 4: pValueElements[8].innerText = receivedBytes[byteCounter++]; pValueElements[8].innerText += "m"; break;
				case 5: pValueElements[9].innerText = receivedBytes[byteCounter++]; pValueElements[9].innerText += "s"; break;
				case 6: pValueElements[10].innerText = receivedBytes[byteCounter++]; pValueElements[10].innerText += "m"; break;
				case 7: pValueElements[11].innerText = receivedBytes[byteCounter++]; pValueElements[11].innerText += "m"; break;
				case 8: pValueElements[12].innerText = receivedBytes[byteCounter++] * 255 + receivedBytes[byteCounter++]; pValueElements[12].innerText += "ml"; break;
				case 9: pValueElements[13].innerText = receivedBytes[byteCounter++] * 255 + receivedBytes[byteCounter++]; pValueElements[13].innerText += "ml"; break;
				case 10: pValueElements[14].innerText = receivedBytes[byteCounter++] * 255 + receivedBytes[byteCounter++]; pValueElements[14].innerText += "/L"; break;
				case 11: pValueElements[15].innerText = receivedBytes[byteCounter++] * 255 + receivedBytes[byteCounter++]; pValueElements[15].innerText += "/L"; break;
				case 12: pValueElements[16].innerText = receivedBytes[byteCounter++] * 255 + receivedBytes[byteCounter++]; pValueElements[16].innerText += "ml"; break;
				case 13: pValueElements[17].innerText = receivedBytes[byteCounter++] * 255 + receivedBytes[byteCounter++]; pValueElements[17].innerText += "ml"; break;
				case 14: programTimeLeft = receivedBytes[byteCounter++] * 255 + receivedBytes[byteCounter++]; startTime = Date.now(); break;
				default: byteCounter += partLength - 1; break;
			}
		}
	}
	receivedBytes[0] = 0;
	byteCounter = 0;
}
