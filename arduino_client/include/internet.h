#ifndef INTERNET_H
#define INTERNET_H

#include "my_library.h"
#include "IPAddress.h"

#define INTERNET_RETRANSMISSION_COUNT (1)

#define INTERNET_TIME_CONNECTION_MAX (4000LU) /* ms */
#define INTERNET_TIME_RESTART_WAIT (3000LU) /* ms */
#define INTERNET_TIME_RECEIVE_LAST_MAX (17000LU) /* ms */
#define INTERNET_TIME_PING_SEND_INTERVAL (5000LU) /* ms */

#define INTERNET_MAC 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
#define INTERNET_REMOTE_HOSTNAME "your_server.com"
#define INTERNET_REMOTE_PORT (5432)
#define INTERNET_LOCAL_PORT (8765)
#define INTERNET_LOCAL_ID (160)
#define INTERNET_REMOTE_ID (170)

#define INTERNET_PACKET_HEADER_SIZE (1U)
#define INTERNET_PACKET_BODY_DATA_SIZE (64U)
#define INTERNET_PACKET_WRAPPER_SIZE (3U) /* start byte, size byte, end byte */
#define INTERNET_PACKET_WRAPPER_BYTE_START (2)
#define INTERNET_PACKET_WRAPPER_BYTE_SIZE_MAX (INTERNET_PACKET_HEADER_SIZE + INTERNET_PACKET_BODY_DATA_SIZE)
#define INTERNET_PACKET_WRAPPER_BYTE_END (4)
#define INTERNET_PACKET_SIZE_MAX (INTERNET_PACKET_WRAPPER_SIZE + INTERNET_PACKET_WRAPPER_BYTE_SIZE_MAX)

#define INTERNET_COMMAND_PING (211)
#define INTERNET_COMMAND_PING_VALUE_REQUEST (10)
#define INTERNET_COMMAND_PING_VALUE_ANSWER (20)

typedef enum
{
    INTERNET_STATUS_UNINITIALIZED,
    INTERNET_STATUS_RESTARTING,
    INTERNET_STATUS_INITIALIZING,
    INTERNET_STATUS_READY
}Internet_status;

typedef union
{
    uint8_t array[4];
    uint32_t dword;
}Internet_ip;

typedef struct
{
    Internet_ip ip;
    uint16_t port;
    uint8_t id;
}Internet_packet_header;

typedef struct
{
    uint8_t data[INTERNET_PACKET_BODY_DATA_SIZE];
    uint8_t size;
}Internet_packet_body;

typedef struct
{
    Internet_packet_header internet_packet_header;
    Internet_packet_body internet_packet_body;
}Internet_packet;

extern IPAddress internet_ip_remote;
extern Internet_packet internet_packet;
extern bool_t internet_is_receive_wait;

extern void internet_init(void);
extern void internet_restart_hard(void);
extern void internet_restart_soft(void);
extern bool_t internet_ip_remote_update(void);
extern void internet_send(void);
extern void internet_receive(void);
extern void internet_troubleshoot(void);
extern void internet_update(void);

#endif /* INTERNET_H */
