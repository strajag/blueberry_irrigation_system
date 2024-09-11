#include "straykovsky_standard_library.h"
#include "server_udp.h"
#include "irrigation_data.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/select.h"

static uint32_t math_power(uint8_t base, uint8_t exponential);
static int user_input_get(void);
static int user_input_available(void);
static void user_input_process_data(void);
static void flow_print(void);
static uint16_t serial_number_read(void);
static uint32_t serial_number_read_32(void);

static f32_t cpu_temp_get(void);

#define TIME_LOOP_INTERVAL (500LU)
static uint32_t time_print_last = 0;
bool_t is_print = TRUE;

static uint16_t number;
static uint32_t number_2;
static uint32_t number_3;
static uint8_t counter;
static uint8_t counter_2;
static uint8_t counter_3;
static int16_t c[10];

int main(void)
{
    server_udp_init();

    for( ; ; )
    {
        server_udp_update();
        user_input_process_data();
        if(is_print)
            flow_print();
    }

    return 0;
}

void user_input_process_data(void)
{
    if(!user_input_available())
        return;

    usleep(100000);

    switch(user_input_get())
    {
        case 'r':
        {
            internet_packet.internet_packet_body.data[0] = INTERFACE_COMMAND_CONTROL_SAFETY_RESET;
            internet_packet.internet_packet_body.size = 1;
            server_udp_send();
            break;
        }
        case 'd':
        {
            internet_packet.internet_packet_body.data[0] = INTERFACE_COMMAND_DEVICE_TOGGLE;
            internet_packet.internet_packet_body.data[1] = serial_number_read();
            internet_packet.internet_packet_body.data[2] = user_input_get() - 48;
            internet_packet.internet_packet_body.size = 3;
            server_udp_send();
            break;
        }
        case 'p':
        {
            is_print = user_input_get() - 48;
            break;
        }
        case 'i':
        {
            internet_packet.internet_packet_body.data[0] = INTERNET_COMMAND_PING;
            internet_packet.internet_packet_body.data[1] = INTERNET_COMMAND_PING_VALUE_REQUEST;
            internet_packet.internet_packet_body.size = 2;
            server_udp_send();
            break;
        }
        case 'c':
        {
            switch(user_input_get())
            {
                case 'a':
                {
                    internet_packet.internet_packet_body.data[0] = INTERFACE_COMMAND_CONTROL_IRRIGATION_START;
                    internet_packet.internet_packet_body.data[1] = user_input_get() - 48;
                    internet_packet.internet_packet_body.data[2] = user_input_get() - 48;
                    internet_packet.internet_packet_body.size = 3;
                    server_udp_send();
                    break;
                }
                case 's':
                {
                    internet_packet.internet_packet_body.data[0] = INTERFACE_COMMAND_CONTROL_IRRIGATION_STOP;
                    internet_packet.internet_packet_body.size = 1;
                    server_udp_send();
                    break;
                }
                case 'r':
                {
                    internet_packet.internet_packet_body.data[0] = INTERFACE_COMMAND_CONTROL_IRRIGATION_RESUME;
                    internet_packet.internet_packet_body.size = 1;
                    server_udp_send();
                    break;
                }
                case 'v':
                {
                    internet_packet.internet_packet_body.data[0] = INTERFACE_COMMAND_CONTROL_IRRIGATION_DELIVER_VOLUME;
                    *(uint16_t *)&internet_packet.internet_packet_body.data[1] = serial_number_read();
                    internet_packet.internet_packet_body.size = 3;
                    server_udp_send();
                    break;
                }
                case 't':
                {
                    internet_packet.internet_packet_body.data[0] = INTERFACE_COMMAND_CONTROL_IRRIGATION_DELIVER_TIME;
                    *(uint16_t *)&internet_packet.internet_packet_body.data[1] = serial_number_read();
                    internet_packet.internet_packet_body.size = 3;
                    server_udp_send();
                    break;
                }
                case 'd':
                {
                    internet_packet.internet_packet_body.data[0] = INTERFACE_COMMAND_CONTROL_IRRIGATION_PROGRAM_DURATION;
                    *(uint32_t *)&internet_packet.internet_packet_body.data[1] = serial_number_read_32();
                    internet_packet.internet_packet_body.size = 5;
                    server_udp_send();
                    break;
                }
                case 'i':
                {
                    internet_packet.internet_packet_body.data[0] = INTERFACE_COMMAND_CONTROL_IRRIGATION_INFO_ALL;
                    internet_packet.internet_packet_body.size = 1;
                    server_udp_send();
                    break;
                }
                case 'p':
                {
                    internet_packet.internet_packet_body.data[0] = INTERFACE_COMMAND_CONTROL_IRRIGATION_IS_SEND_INFO;
                    internet_packet.internet_packet_body.data[1] = user_input_get() - 48;
                    internet_packet.internet_packet_body.size = 2;
                    server_udp_send();
                    break;
                }
		case 'f':
		{
		    internet_packet.internet_packet_body.data[0] = INTERFACE_COMMAND_CONTROL_IRRIGATION_IS_FAULTY_DETECTION_ACTIVE;
		    internet_packet.internet_packet_body.data[1] = user_input_get() - 48;
		    internet_packet.internet_packet_body.size = 2;
		    server_udp_send();
		    break;
	        }
            }
            break;
        }
    }
}

int user_input_available(void)
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

int user_input_get(void)
{
    int r;
    unsigned char ch;
    if ((r = read(0, &ch, sizeof(ch))) < 0)
        return r;
    else
        return ch;
}

uint16_t serial_number_read(void)
{
    number = 0;
    counter = 0;

    while(TRUE)
    {
        c[counter] = user_input_get();

        if(c[counter] < '0' || c[counter] > '9')
            break;

        c[counter] -= 48;
        counter++;
    }

    for(counter_2 = 0; counter_2 < counter; counter_2++)
        number += c[counter - counter_2 - 1] * math_power(10, counter_2);

    return number;
}

uint32_t serial_number_read_32(void)
{
    number_3 = 0;
    counter = 0;

    while(TRUE)
    {
        c[counter] = user_input_get();

        if(c[counter] < '0' || c[counter] > '9')
            break;

        c[counter] -= 48;
        counter++;
    }

    for(counter_2 = 0; counter_2 < counter; counter_2++)
        number_3 += c[counter - counter_2 - 1] * math_power(10, counter_2);

    return number_3;
}

uint32_t math_power(uint8_t base, uint8_t exponential)
{
    if(exponential == 0)
        return 1;

    number_2 = base;

    for(counter_3 = 0; counter_3 < exponential - 1; counter_3++)
        number_2 *= base;

    return number_2;
}

f32_t cpu_temp_get(void)
{
    float systemp, millideg;
    FILE *thermal;

    thermal = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    fscanf(thermal,"%f", &millideg);
    fclose(thermal);
    systemp = millideg / 1000;

    return systemp;
}

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

static uint32_t received_time_ago;

void flow_print(void)
{
	if(get_time() - time_print_last < TIME_LOOP_INTERVAL)
		return;

	/* clear screen */
	printf("\033[2J\033[1;1H");

	/* info */
	received_time_ago = (get_time() - time_received_last) / 1000;
	printf(KWHT "last received time: ");
	if(received_time_ago == 0)
	    printf(KNRM);
	else
        printf(KRED);
	printf("%us " KNRM "ago", received_time_ago);
	printf(KYEL "\n---------------------------------------------------\n");

    /* device */
	printf(KWHT);
    for(counter = 0; counter < DEVICE_ARRAY_SIZE; counter++)
        printf("%-6u|", counter);
    printf("\n%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|\n", "generator", "p-well", "p-acid", "p-supplements", "p-micro", "p-macro", "p-click", "v-micro", "v-macro", "v-water", "v-1group", "v-2group", "v-3group", "v-garden", "lamp");
	for(counter = 0; counter < DEVICE_ARRAY_SIZE; counter++)
	{
		if(device_array[counter].is_active == 1)
			printf(KGRN "%-6.6s" KNRM "|", "on");
		else if(device_array[counter].is_active == 0)
			printf(KNRM "%-6.6s|", "off");
		else
			printf(KRED "%-6u" KNRM "|", device_array[counter].is_active);
	}
	printf(KYEL "\n---------------------------------------------------\n" KNRM);

    /* sensor flow */
    printf(KWHT "%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|\n" KNRM, "well", "micro", "macro", "water", "click");
	for(counter = 0; counter < SENSOR_FLOW_ARRAY_SIZE; counter++)
        printf("%-6u|", (uint32_t)(sensor_flow_array[counter].pulses_total / (sensor_flow_array[counter].pulses_per_liter / 1000.0)));
	printf(KYEL "\n---------------------------------------------------\n" KNRM);

    /* sensor pressure */
    printf(KWHT "%-6.6s|%-6.6s|%-6.6s|\n" KNRM, "well", "before", "after");
	for(counter = 0; counter < SENSOR_PRESSURE_ARRAY_SIZE; counter++)
        printf("%-6.1f|", sensor_pressure_array[counter].pressure / 10.0f);
	printf(KYEL "\n---------------------------------------------------\n" KNRM);

    /* control irrigation */
    printf(KWHT "%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|%-6.6s|\n" KNRM, "program", "progrC", "status", "cleaning", "stopped", "faultyD", "delivV", "delivT", "geWait", "progrD", "grou3D", "gro12D", "startT", "stopT", "genStT", "passeT");
    printf("%-6u|%-6u|%-6u|%-6u|", control_irrigation_temp_program, control_irrigation_temp_program_current, control_irrigation_temp_status, control_irrigation_temp_is_cleaning);
	if(control_irrigation_temp_is_stopped != 0)
		printf(KRED "%-6u" KNRM, control_irrigation_temp_is_stopped);
	else
		printf("%-6u", control_irrigation_temp_is_stopped);
	printf("|%-6u|%-6u|%-6u|%-6u|%-6u|%-6u|%-6u|%-6u|%-6u|%-6u|%-6u|", control_irrigation_temp_is_faulty_detection_active, control_irrigation_temp_deliver_volume, control_irrigation_temp_deliver_time, control_irrigation_temp_time_generator_wait, control_irrigation_temp_time_program_duration, control_irrigation_temp_time_cleaning_group3_duration, control_irrigation_temp_time_cleaning_group12_duration, control_irrigation_temp_time_program_start / 1000, control_irrigation_temp_time_program_stop / 1000, control_irrigation_temp_time_generator_start / 1000, (control_irrigation_temp_time_current - control_irrigation_temp_time_program_start) / 1000);
	printf(KYEL "\n---------------------------------------------------\n" KNRM);

    /* cpu temp and other */
    printf(KWHT "%-6.6s|\n" KNRM, "cpuT");
    printf("%-6.1f|", cpu_temp_get());
    printf(KYEL "\n---------------------------------------------------\n" KNRM);

    time_print_last = get_time();
}
