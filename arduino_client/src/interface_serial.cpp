#include "interface.h"
#include "device.h"
#include "sensor_flow.h"
#include "control_flow.h"
#include "control_irrigation_temp.h"
#include "internet.h"
#include "control_safety.h"

static void user_input(void);
static uint16_t serial_number_read(void);
static uint32_t math_power(uint8_t base, uint8_t exponential);
static void flow_print(void);

#define TIME_LOOP_INTERVAL (2000LU)
static uint32_t time_print_last = 0;
static bool_t is_print = FALSE;

static uint16_t number;
static uint32_t number_2;
static uint8_t counter;
static uint8_t counter_2;
static uint8_t counter_3;
static int16_t c[10];

static uint16_t aa;
static uint16_t bb;
static uint16_t cc;

extern void interface_serial_update(void)
{
    user_input();

    if(is_print)
        flow_print();
}

void user_input(void)
{
    if(!Serial.available())
        return;
    delay(100);

    switch(Serial.read())
    {
        case 'p':
        {
            internet_packet.internet_packet_body.data[0] = INTERNET_COMMAND_PING;
            internet_packet.internet_packet_body.data[1] = INTERNET_COMMAND_PING_VALUE_REQUEST;
            internet_packet.internet_packet_body.size = 2;
            internet_send();
            break;
        }
        case 'r': control_safety_reset(); break;
        case 'd': aa = serial_number_read(); bb = Serial.read() - 48; device_toggle(&device_array[aa], bb); break;
        case 'f':
            switch(Serial.read())
            {
                case 'r': aa = Serial.read(); bb = serial_number_read(); sensor_flow_pulses_total_set(&sensor_flow_array[aa], bb); break;
                case 's': aa = Serial.read() - 48; bb = serial_number_read(); sensor_flow_pulses_per_liter_set(&sensor_flow_array[aa], bb); break;
                case 'p': is_print = Serial.read() - 48; break;
                case 'a': aa = Serial.read() - 48; bb = serial_number_read(); sensor_flow_array[aa].pulses_total += bb; break;
            }
            break;
        case 'c':
            switch(Serial.read())
            {
                case 'f':
                    switch(Serial.read())
                    {
                        case 'a': aa = Serial.read() - 48; bb = serial_number_read(); cc = serial_number_read(); control_flow_start(&control_flow_array[aa], bb, cc); break;
                        case 's': control_flow_stop(&control_flow_array[Serial.read() - 48]); break;
                        case 'p': aa = Serial.read() - 48; bb = aa = Serial.read() - 48; control_flow_pause(&control_flow_array[aa], bb); break;
                    }
                    break;
                case 'a':
                    switch(Serial.read())
                    {
                        case 'a': aa = Serial.read() - 48; bb = Serial.read() - 48; control_irrigation_temp_start((Control_irrigation_temp_program)aa, bb); break;
                        case 's': control_irrigation_temp_force_stop(); break;
                        case 'r': control_irrigation_temp_resume(); break;
                    }
                    break;
            }
            break;
        case 'i':
            switch(Serial.read())
            {
                case 'r': internet_restart_hard(); break;
                case 'b': internet_restart_soft(); break;
                case 'p': internet_ip_remote_update(); break;
                case 'o':
                {
                    internet_packet.internet_packet_body.data[0] = Serial.read() - 48;
                    internet_packet.internet_packet_body.size = 1;
                    internet_send();
                    break;
                }
                case 't':
                {
                    internet_packet.internet_packet_body.data[0] = Serial.read() - 48;
                    internet_packet.internet_packet_body.data[1] = Serial.read() - 48;
                    internet_packet.internet_packet_body.size = 2;
                    internet_send();
                    break;
                }
            }
            break;
    }
}

uint16_t serial_number_read(void)
{
    number = 0;
    counter = 0;

    while(TRUE)
    {
        c[counter] = Serial.read();

        if(c[counter] < '0' || c[counter] > '9')
            break;

        c[counter] -= 48;
        counter++;
    }

    for(counter_2 = 0; counter_2 < counter; counter_2++)
        number += c[counter - counter_2 - 1] * math_power(10, counter_2);

    return number;
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

void flow_print(void)
{
    if(millis() - time_print_last < TIME_LOOP_INTERVAL)
        return;
    
    /* print milliliters */
    debug_log(("| %lu | %lu | %lu | %lu | %lu |\n",
                 (uint32_t)(sensor_flow_array[0].pulses_total / (sensor_flow_array[0].pulses_per_liter / 1000.0)),
                 (uint32_t)(sensor_flow_array[1].pulses_total / (sensor_flow_array[1].pulses_per_liter / 1000.0)),
                 (uint32_t)(sensor_flow_array[2].pulses_total / (sensor_flow_array[2].pulses_per_liter / 1000.0)),
                 (uint32_t)(sensor_flow_array[3].pulses_total / (sensor_flow_array[3].pulses_per_liter / 1000.0)),
                 (uint32_t)(sensor_flow_array[4].pulses_total / (sensor_flow_array[4].pulses_per_liter / 1000.0))
    ));


    /* print pulses */
    /*debug_log(("| %lu | %lu | %lu | %lu | %lu |\n",
              (uint32_t)sensor_flow_array[0].pulses_total,
              (uint32_t)sensor_flow_array[1].pulses_total,
              (uint32_t)sensor_flow_array[2].pulses_total,
              (uint32_t)sensor_flow_array[3].pulses_total,
              (uint32_t)sensor_flow_array[4].pulses_total
    ));*/

    time_print_last = millis();
}
