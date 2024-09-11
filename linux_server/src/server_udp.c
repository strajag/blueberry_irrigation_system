#include "server_udp.h"
#include "irrigation_data.h"
#include "stdio.h"	/* printf */
#include "string.h" /* memset */
#include "stdlib.h" /* exit(0); */
#include "unistd.h"
#include "errno.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "sys/time.h"

extern bool_t is_print;

static int socket_server;
static struct sockaddr_in address_server, address_client;
static struct timeval read_timeout;
static size_t address_client_size = sizeof(address_client);

Internet_packet internet_packet;
static int32_t size;
static uint8_t buffer[INTERNET_PACKET_WRAPPER_BYTE_SIZE_MAX];
static uint8_t counter;
static uint8_t counter_2;

uint32_t time_received_last;

void die(char *s)
{
    perror(s);
    printf("->%d<-\n", errno);
    exit(1);
}

void server_udp_init(void)
{
    printf("server_udp_init(): start\n");

    /* create socket */
    if((socket_server = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        die("server_udp_init(): create");

    /* set socket */
    address_server.sin_family = AF_INET;
    address_server.sin_port = htons(INTERNET_LOCAL_PORT);
    address_server.sin_addr.s_addr = INADDR_ANY;

    /* bind socket */
    if(bind(socket_server, (struct sockaddr*)&address_server, sizeof(address_server)) == -1)
        die("server_udp_init(): bind");

    /* make recvfrom() non-blocking */
    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    setsockopt(socket_server, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));

    printf("server_udp_init(): end\n");
}

bool_t server_udp_receive(void)
{
    /* receive data */
    /* if recvfrom() returns 0 size then connection is gracefully closed. */

    if((size = recvfrom(socket_server, buffer, INTERNET_PACKET_SIZE_MAX, 0, (struct sockaddr*)&address_client, &address_client_size)) == -1)
    {
        if(errno == EAGAIN)
            return FALSE;
        else
            die("server_udp_receive(): receive");
    }

    /*printf("server_udp_receive(): start\n");*/

    internet_packet.internet_packet_header.ip.dword = address_client.sin_addr.s_addr;
    internet_packet.internet_packet_header.port = address_client.sin_port;
    internet_packet.internet_packet_body.size = size;

    if(internet_packet.internet_packet_body.size > INTERNET_PACKET_SIZE_MAX)
    {
        printf("internet_receive(): size big\n");
        return FALSE;
    }

    if(internet_packet.internet_packet_body.size < INTERNET_PACKET_WRAPPER_SIZE)
    {
        printf("internet_receive(): size small\n");
        return FALSE;
    }

    counter = 0;

    if(buffer[counter++] != INTERNET_PACKET_WRAPPER_BYTE_START)
    {
        printf("internet_receive(): wrong start\n");
        return FALSE;
    }

    internet_packet.internet_packet_body.size -= INTERNET_PACKET_WRAPPER_SIZE;

    if(buffer[counter++] != internet_packet.internet_packet_body.size)
    {
        printf("internet_receive(): wrong size\n");
        return FALSE;
    }

    internet_packet.internet_packet_body.size -= INTERNET_PACKET_HEADER_SIZE;

    if(internet_packet.internet_packet_body.size == 0)
    {
        printf("internet_receive(): size is zero\n");
        return FALSE;
    }

    if((internet_packet.internet_packet_header.id = buffer[counter++]) != INTERNET_ARDUINO_ID)
    {
        printf("internet_receive(): wrong id\n");
        return FALSE;
    }

    for(counter_2 = 0; counter_2 < internet_packet.internet_packet_body.size; counter_2++)
    {
        internet_packet.internet_packet_body.data[counter_2] = buffer[counter++];
        /*printf("%d | ", internet_packet.internet_packet_body.data[counter_2]);*/
    }
    /*printf("\n");*/

    if(buffer[counter] != INTERNET_PACKET_WRAPPER_BYTE_END)
    {
        printf("internet_receive(): wrong end\n");
        return FALSE;
    }

    return TRUE;
}

void server_udp_send(void)
{
    /* send data */
    /*printf("server_udp_send(): start\n");*/

    address_client.sin_addr.s_addr = internet_packet.internet_packet_header.ip.dword;
    address_client.sin_port = internet_packet.internet_packet_header.port;
    internet_packet.internet_packet_header.id = INTERNET_LOCAL_ID;

    buffer[0] = INTERNET_PACKET_WRAPPER_BYTE_START;
    buffer[1] = internet_packet.internet_packet_body.size + INTERNET_PACKET_HEADER_SIZE;
    buffer[2] = internet_packet.internet_packet_header.id;
    memcpy(&buffer[3], internet_packet.internet_packet_body.data, internet_packet.internet_packet_body.size);
    buffer[internet_packet.internet_packet_body.size + INTERNET_PACKET_WRAPPER_SIZE + INTERNET_PACKET_HEADER_SIZE - 1] = INTERNET_PACKET_WRAPPER_BYTE_END;

    if(sendto(socket_server, buffer, internet_packet.internet_packet_body.size + INTERNET_PACKET_WRAPPER_SIZE + INTERNET_PACKET_HEADER_SIZE, 0, (struct sockaddr*)&address_client, address_client_size) == -1)
        die("server_udp_send(): send");
}

void server_udp_update(void)
{
    /* listen */
    if(!server_udp_receive())
        return;

    /* received correctly */
    /*server_print_received_data();*/
    time_received_last = get_time();

    /* processing data */
    server_process_received_data();
}

extern void server_print_received_data(void)
{
    printf("last received time: %u|%us ago\n", time_received_last, (get_time() - time_received_last) / 1000);
    printf("received ip: %u.%u.%u.%u:%u\n", internet_packet.internet_packet_header.ip.array[0], internet_packet.internet_packet_header.ip.array[1], internet_packet.internet_packet_header.ip.array[2], internet_packet.internet_packet_header.ip.array[3], internet_packet.internet_packet_header.port);
    printf("received id: %u\n", internet_packet.internet_packet_header.id);
    printf("received data: ");
    for(counter = 0; counter < internet_packet.internet_packet_body.size; counter++)
        printf("%d | ", internet_packet.internet_packet_body.data[counter]);
    printf("\n");
}

extern void server_process_received_data(void)
{
    counter = 0;

    switch(internet_packet.internet_packet_body.data[counter++])
    {
        case INTERFACE_COMMAND_DEVICE_INFO:
        {
            /*printf("server_process_received_data(): INTERFACE_COMMAND_DEVICE_INFO\n");*/
            if(counter + DEVICE_ARRAY_SIZE * sizeof(uint8_t) != internet_packet.internet_packet_body.size)
                printf("server_process_received_data(): wrong size\n");
            else
            {
                for(counter_2 = 0; counter_2 < DEVICE_ARRAY_SIZE; counter_2++)
                    device_array[counter_2].is_active = internet_packet.internet_packet_body.data[counter++];
            }
            break;
        }
        case INTERFACE_COMMAND_SENSOR_FLOW_INFO:
        {
            /*printf("server_process_received_data(): INTERFACE_COMMAND_SENSOR_FLOW_INFO\n");*/
            if(counter + SENSOR_FLOW_ARRAY_SIZE * sizeof(uint32_t) + SENSOR_FLOW_ARRAY_SIZE * sizeof(uint16_t) != internet_packet.internet_packet_body.size)
                printf("server_process_received_data(): wrong size\n");
            else
            {
                for(counter_2 = 0; counter_2 < SENSOR_FLOW_ARRAY_SIZE; counter_2++)
                {
                    sensor_flow_array[counter_2].pulses_total = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
                    counter += sizeof(uint32_t);
                    sensor_flow_array[counter_2].pulses_per_liter = *(uint16_t *)&internet_packet.internet_packet_body.data[counter];
                    counter += sizeof(uint16_t);
                }
            }
            break;
        }
        case INTERFACE_COMMAND_SENSOR_PRESSURE_INFO:
        {
            /*printf("server_process_received_data(): INTERFACE_COMMAND_SENSOR_PRESSURE_INFO\n");*/
            if(counter + SENSOR_PRESSURE_ARRAY_SIZE * sizeof(uint8_t) != internet_packet.internet_packet_body.size)
                printf("server_process_received_data(): wrong size\n");
            else
            {
                for(counter_2 = 0; counter_2 < SENSOR_PRESSURE_ARRAY_SIZE; counter_2++)
                    sensor_pressure_array[counter_2].pressure = internet_packet.internet_packet_body.data[counter++];
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_INFO:
        {
            /*printf("server_process_received_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_INFO\n");*/
            if(counter + 42 != internet_packet.internet_packet_body.size)
                printf("server_process_received_data(): wrong size\n");
            else
            {
                control_irrigation_temp_program = internet_packet.internet_packet_body.data[counter++];
                control_irrigation_temp_program_current = internet_packet.internet_packet_body.data[counter++];
                control_irrigation_temp_status = internet_packet.internet_packet_body.data[counter++];
                control_irrigation_temp_is_cleaning = internet_packet.internet_packet_body.data[counter++];
                control_irrigation_temp_is_stopped = internet_packet.internet_packet_body.data[counter++];
                control_irrigation_temp_is_faulty_detection_active = internet_packet.internet_packet_body.data[counter++];
		control_irrigation_temp_deliver_volume = *(uint16_t *)&internet_packet.internet_packet_body.data[counter];
                counter += sizeof(uint16_t);
                control_irrigation_temp_deliver_time = *(uint16_t *)&internet_packet.internet_packet_body.data[counter];
                counter += sizeof(uint16_t);
                control_irrigation_temp_time_generator_wait = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
                counter += sizeof(uint32_t);
                control_irrigation_temp_time_program_duration = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
                counter += sizeof(uint32_t);
                control_irrigation_temp_time_cleaning_group3_duration = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
                counter += sizeof(uint32_t);
                control_irrigation_temp_time_cleaning_group12_duration = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
                counter += sizeof(uint32_t);
                control_irrigation_temp_time_program_start = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
                counter += sizeof(uint32_t);
                control_irrigation_temp_time_program_stop = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
                counter += sizeof(uint32_t);
                control_irrigation_temp_time_generator_start = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
                counter += sizeof(uint32_t);
                control_irrigation_temp_time_current = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
                counter += sizeof(uint32_t);
            }
            break;
        }
        case INTERNET_COMMAND_PING:
        {
            /*printf("server_process_received_data(): INTERNET_COMMAND_PING\n");*/
            if(counter + sizeof(uint8_t) != internet_packet.internet_packet_body.size)
                printf("server_process_received_data(): wrong size\n");
            else
                if(internet_packet.internet_packet_body.data[counter] == INTERNET_COMMAND_PING_VALUE_REQUEST)
                {
					if(!is_print)
                    	printf("server_process_received_data(): INTERNET_COMMAND_PING REQUEST\n");
                    internet_packet.internet_packet_body.data[counter] = INTERNET_COMMAND_PING_VALUE_ANSWER;
                    server_udp_send();
                }
                else
                {
                    printf("server_process_received_data(): INTERNET_COMMAND_PING ANSWER\n");
                }
            break;
        }
        default:
        {
            printf("server_process_received_data(): pic: unknown command\n");
            break;
        }
    }
}

uint32_t get_time(void)
{
    struct timeval te;
    uint32_t milliseconds;
    gettimeofday(&te, NULL);
    milliseconds = te.tv_sec * 1000LU + te.tv_usec / 1000;
    return milliseconds;
}
