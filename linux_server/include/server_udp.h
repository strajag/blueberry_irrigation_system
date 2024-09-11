#ifndef SERVER_UDP_H
#define SERVER_UDP_H

#include "straykovsky_standard_library.h"

#define INTERNET_LOCAL_PORT (5432)
#define INTERNET_LOCAL_ID (170)
#define INTERNET_ARDUINO_ID (160)

#define INTERNET_PACKET_HEADER_SIZE (1U)
#define INTERNET_PACKET_BODY_DATA_SIZE (64U)
#define INTERNET_PACKET_WRAPPER_SIZE (3U) /* start byte, size byte, end byte */
#define INTERNET_PACKET_WRAPPER_BYTE_START (2)
#define INTERNET_PACKET_WRAPPER_BYTE_SIZE_MAX (INTERNET_PACKET_HEADER_SIZE + INTERNET_PACKET_BODY_DATA_SIZE)
#define INTERNET_PACKET_WRAPPER_BYTE_END (4)
#define INTERNET_PACKET_SIZE_MAX (INTERNET_PACKET_WRAPPER_SIZE + INTERNET_PACKET_WRAPPER_BYTE_SIZE_MAX)

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

extern Internet_packet internet_packet;
extern uint32_t time_received_last;

extern void server_udp_init(void);
extern bool_t server_udp_receive(void);
extern void server_udp_send(void);
extern void server_udp_update(void);

extern void server_print_received_data(void);
extern void server_process_received_data(void);
extern void server_print_irrigation_data(void);
extern uint32_t get_time(void);

#endif /* SERVER_UDP_H */
