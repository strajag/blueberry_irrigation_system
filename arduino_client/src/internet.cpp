#include "internet.h"
#include "pins.h"
#include "Ethernet.h"
#include "EthernetUdp.h"
#include "Dns.h"

#undef UDP_TX_PACKET_MAX_SIZE
#define UDP_TX_PACKET_MAX_SIZE INTERNET_PACKET_SIZE_MAX

static DNSClient dns;
static EthernetUDP udp;
static uint8_t mac[] = {INTERNET_MAC};

static Internet_status status;
static uint32_t time_restart_last; /* ms */
static uint32_t time_receive_last; /* ms */
static uint32_t time_ping_send_last; /* ms */

static uint8_t counter;
IPAddress internet_ip_remote;
Internet_packet internet_packet;
bool_t internet_is_receive_wait = FALSE;

void internet_init(void)
{
    debug_log(("internet_init()\n"));

    pinMode(INTERNET_PIN_RESET, INPUT_PULLUP);
    pinMode(INTERNET_PIN_RESET, OUTPUT);
    digitalWrite(INTERNET_PIN_RESET, HIGH);

    internet_packet.internet_packet_header.port = INTERNET_REMOTE_PORT;
    internet_packet.internet_packet_header.id = INTERNET_LOCAL_ID;

    Ethernet.init(INTERNET_PIN_SS);

    internet_restart_soft();

    debug_log(("internet_init(): success\n"));
}

void internet_restart_hard(void)
{
    debug_log(("internet_restart_hard()\n"));

    digitalWrite(INTERNET_PIN_RESET, LOW);
    time_restart_last = millis();
    status = INTERNET_STATUS_RESTARTING;

    debug_log(("internet_restart_hard(): success\n"));
}

void internet_restart_soft(void)
{
    debug_log(("internet_restart_soft()\n"));

    time_receive_last = millis();
    status = INTERNET_STATUS_UNINITIALIZED;

    if (!Ethernet.begin(mac, INTERNET_TIME_CONNECTION_MAX, INTERNET_TIME_CONNECTION_MAX))
    {
        if (Ethernet.hardwareStatus() == EthernetNoHardware)
        {
            debug_log(("internet_restart_soft(): no hardware\n"));
            return;
        }

        if (Ethernet.linkStatus() == LinkOFF)
        {
            debug_log(("internet_restart_soft(): no cable\n"));
            return;
        }

        debug_log(("internet_restart_soft(): dhcp failed\n"));
        return;
    }

    Ethernet.setRetransmissionCount(INTERNET_RETRANSMISSION_COUNT);
    Ethernet.setRetransmissionTimeout(INTERNET_TIME_CONNECTION_MAX);

    dns.begin(Ethernet.dnsServerIP());

    udp.stop();
    if(!udp.begin(INTERNET_LOCAL_PORT))
    {
        debug_log(("internet_restart_soft(): udp failed\n"));
        return;
    }

    status = INTERNET_STATUS_READY;

    if(!internet_ip_remote_update())
    {
        debug_log(("internet_restart_soft(): dns failed\n"));
        return;
    }

    time_receive_last = millis();
    debug_log(("internet_restart_soft(): success\n"));
}

bool_t internet_ip_remote_update(void)
{
    debug_log(("internet_ip_remote_update()\n"));

    if(status != INTERNET_STATUS_READY)
    {
        debug_log(("internet_ip_remote_update(): status isn't ready\n"));
        return FALSE;
    }

    if (dns.getHostByName(INTERNET_REMOTE_HOSTNAME, internet_ip_remote, INTERNET_TIME_CONNECTION_MAX) != 1)
    {
        debug_log(("internet_ip_remote_update(): failed\n"));
        return FALSE;
    }

    internet_packet.internet_packet_header.ip.dword = internet_ip_remote;

    debug_log(("internet_ip_remote_update(): success\n"));
    return TRUE;
}

void internet_send(void)
{
    debug_log(("internet_send()\n"));

    if(status != INTERNET_STATUS_READY)
    {
        debug_log(("internet_send(): status isn't ready\n"));
        return;
    }

    if(!udp.beginPacket(internet_packet.internet_packet_header.ip.dword, internet_packet.internet_packet_header.port))
    {
        debug_log(("internet_send(): begin packet fail\n"));
        return;
    }

    udp.write(INTERNET_PACKET_WRAPPER_BYTE_START);
    udp.write(internet_packet.internet_packet_body.size + INTERNET_PACKET_HEADER_SIZE);
    udp.write(internet_packet.internet_packet_header.id);
    udp.write(internet_packet.internet_packet_body.data, internet_packet.internet_packet_body.size);
    udp.write(INTERNET_PACKET_WRAPPER_BYTE_END);

    if(!udp.endPacket())
    {
        debug_log(("internet_send(): end packet fail\n"));
        return;
    }

    debug_log(("internet_send(): success\n"));
}

void internet_receive(void)
{
    debug_log(("internet_receive()\n"));

    if(udp.remoteIP() != internet_ip_remote)
    {
        debug_log(("internet_receive(): wrong ip\n"));
        udp.flush();
        return;
    }

    if(udp.remotePort() != INTERNET_REMOTE_PORT)
    {
        debug_log(("internet_receive(): wrong port\n"));
        udp.flush();
        return;
    }

    time_receive_last = millis();

    if(internet_packet.internet_packet_body.size > INTERNET_PACKET_SIZE_MAX)
    {
        debug_log(("internet_receive(): size big\n"));
        udp.flush();
        return;
    }

    if(internet_packet.internet_packet_body.size < INTERNET_PACKET_WRAPPER_SIZE)
    {
        debug_log(("internet_receive(): size small\n"));
        udp.flush();
        return;
    }

    if(udp.read() != INTERNET_PACKET_WRAPPER_BYTE_START)
    {
        debug_log(("internet_receive(): wrong start\n"));
        udp.flush();
        return;
    }

    internet_packet.internet_packet_body.size -= INTERNET_PACKET_WRAPPER_SIZE;

    if(udp.read() != internet_packet.internet_packet_body.size)
    {
        debug_log(("internet_receive(): wrong size\n"));
        udp.flush();
        return;
    }

    internet_packet.internet_packet_body.size -= INTERNET_PACKET_HEADER_SIZE;

    if(internet_packet.internet_packet_body.size == 0)
    {
        debug_log(("internet_receive(): size is zero\n"));
        udp.flush();
        return;
    }

    if(udp.read() != INTERNET_REMOTE_ID)
    {
        debug_log(("internet_receive(): wrong id\n"));
        udp.flush();
        return;
    }

    for(counter = 0; counter < internet_packet.internet_packet_body.size; counter++)
    {
        internet_packet.internet_packet_body.data[counter] = udp.read();
        debug_log(("%d | ", internet_packet.internet_packet_body.data[counter]));
    }
    debug_log(("\n"));

    if(udp.read() != INTERNET_PACKET_WRAPPER_BYTE_END)
    {
        debug_log(("internet_receive(): wrong end\n"));
        udp.flush();
        return;
    }

    udp.flush();
    internet_is_receive_wait = TRUE;
    debug_log(("internet_receive(): success\n"));
}

void internet_troubleshoot(void)
{
    debug_log(("internet_troubleshoot()\n"));

    /*if(!internet_ip_remote_update())
    {
         debug_log(("internet_troubleshoot(): dns or local error\n"));
         internet_restart_hard();
         return;
    }

    time_receive_last = millis();*/

    internet_restart_hard();

    debug_log(("internet_troubleshoot(): success\n"));
}

void internet_update(void)
{
    if(status == INTERNET_STATUS_RESTARTING)
    {
        if(millis() - time_restart_last < INTERNET_TIME_RESTART_WAIT)
            return;

        digitalWrite(INTERNET_PIN_RESET, HIGH);
        debug_log(("internet_update(): restarting\n"));
        status = INTERNET_STATUS_INITIALIZING;
        time_restart_last = millis();
        return;
    }
    
    if(status == INTERNET_STATUS_INITIALIZING)
    {
        if(millis() - time_restart_last < INTERNET_TIME_RESTART_WAIT)
            return;

        debug_log(("internet_update(): initializing\n"));
        internet_restart_soft();
        return;
    }

    if(millis() - time_receive_last >= INTERNET_TIME_RECEIVE_LAST_MAX)
    {
        internet_troubleshoot();
        return;
    }

    if(status == INTERNET_STATUS_UNINITIALIZED)
        return;

    if(internet_is_receive_wait)
        return;

    if(millis() - time_ping_send_last >= INTERNET_TIME_PING_SEND_INTERVAL)
    {
        internet_packet.internet_packet_body.data[0] = INTERNET_COMMAND_PING;
        internet_packet.internet_packet_body.data[1] = INTERNET_COMMAND_PING_VALUE_REQUEST;
        internet_packet.internet_packet_body.size = 2;
        internet_send();
        time_ping_send_last = millis();
    }

    if((internet_packet.internet_packet_body.size = udp.parsePacket()))
        internet_receive();
}
