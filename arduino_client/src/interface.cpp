#include "interface.h"
#include "device.h"
#include "sensor_flow.h"
#include "sensor_pressure.h"
#include "control_irrigation_temp.h"
#include "internet.h"
#include "control_safety.h"

static uint8_t counter;
static uint8_t counter_2;
static uint32_t time_start_info;
static bool_t interface_is_send_info;

void interface_init(void)
{
    debug_log(("interface_init()\n"));
    debug_log(("interface_init(): success\n"));
}

void interface_update(void)
{
    interface_process_data();
    interface_serial_update();
}

void interface_process_data(void)
{
    if(internet_is_receive_wait)
    {
        debug_log(("interface_process_data(): start\n"));

        for(counter = 0; counter < internet_packet.internet_packet_body.size; counter++)
            debug_log(("%d | ", internet_packet.internet_packet_body.data[counter]));
        debug_log(("\n"));
    }
    else
    {
        if(!interface_is_send_info)
            return;

        if(millis() - time_start_info < INTERFACE_COMMAND_CONTROL_IRRIGATION_TIME_INTERVAL_SEND_INFO)
            return;

        time_start_info = millis();
        internet_packet.internet_packet_body.data[0] = INTERFACE_COMMAND_CONTROL_IRRIGATION_INFO_ALL;
        internet_packet.internet_packet_body.size = 1;
    }

    counter = 0;

    switch(internet_packet.internet_packet_body.data[counter++])
    {
        case INTERFACE_COMMAND_CONTROL_SAFETY_RESET:
        {
            if(counter != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_SAFETY_RESET wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_SAFETY_RESET\n"));
                control_safety_reset();
            }
            break;
        }
        case INTERFACE_COMMAND_DEVICE_TOGGLE:
        {
            if(counter + 2 * sizeof(uint8_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_DEVICE_TOGGLE wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_DEVICE_TOGGLE\n"));
                device_toggle(&device_array[internet_packet.internet_packet_body.data[counter]], internet_packet.internet_packet_body.data[counter + 1]);
            }
            break;
        }
        case INTERFACE_COMMAND_DEVICE_TIME_SWITCH_MINIMUM:
        {
            if(counter + 2 * sizeof(uint8_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_DEVICE_TIME_SWITCH_MINIMUM wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_DEVICE_TIME_SWITCH_MINIMUM\n"));
                device_array[internet_packet.internet_packet_body.data[counter]].time_switch_minimum = internet_packet.internet_packet_body.data[counter + 1];
            }
            break;
        }
        case INTERFACE_COMMAND_SENSOR_FLOW_PULSES_PER_LITER:
        {
            if(counter + sizeof(uint8_t) + sizeof(uint16_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_SENSOR_FLOW_PULSES_PER_LITER wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_SENSOR_FLOW_PULSES_PER_LITER\n"));
                sensor_flow_array[internet_packet.internet_packet_body.data[counter]].pulses_per_liter = *(uint16_t *)&internet_packet.internet_packet_body.data[counter + 1];
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_IS_SEND_INFO:
        {
            if(counter + sizeof(uint8_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_IS_SEND_INFO wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_IS_SEND_INFO\n"));
                interface_is_send_info = internet_packet.internet_packet_body.data[counter];
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_IS_FAULTY_DETECTION_ACTIVE:
        {
            if(counter + sizeof(uint8_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_IS_FAULTY_DETECTION_ACTIVE wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_IS_FAULTY_DETECTION_ACTIVE\n"));
                control_irrigation_temp_faulty_detection_toggle(internet_packet.internet_packet_body.data[counter]);
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_START:
        {
            if(counter + 2 * sizeof(uint8_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_START wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_START\n"));
                control_irrigation_temp_start((Control_irrigation_temp_program)internet_packet.internet_packet_body.data[counter], internet_packet.internet_packet_body.data[counter + 1]);
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_STOP:
        {
            if(counter != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_STOP wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_STOP\n"));
                control_irrigation_temp_force_stop();
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_RESUME:
        {
            if(counter != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_RESUME wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_RESUME\n"));
                control_irrigation_temp_resume();
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_DELIVER_VOLUME:
        {
            if(counter + sizeof(uint16_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_DELIVER_VOLUME wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_DELIVER_VOLUME\n"));
                control_irrigation_temp_deliver_volume = *(uint16_t *)&internet_packet.internet_packet_body.data[counter];
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_DELIVER_TIME:
        {
            if(counter + sizeof(uint16_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_DELIVER_TIME wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_DELIVER_TIME\n"));
                control_irrigation_temp_deliver_time = *(uint16_t *)&internet_packet.internet_packet_body.data[counter];
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_GENERATOR_WAIT_TIME:
        {
            if(counter + sizeof(uint32_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_GENERATOR_WAIT_TIME wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_GENERATOR_WAIT_TIME\n"));
                control_irrigation_temp_time_generator_wait = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_PROGRAM_DURATION:
        {
            if(counter + sizeof(uint32_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_PROGRAM_DURATION wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_PROGRAM_DURATION\n"));
                control_irrigation_temp_time_program_duration = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_CLEANING_GROUP3_DURATION:
        {
            if(counter + sizeof(uint32_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_CLEANING_GROUP3_DURATION wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_CLEANING_GROUP3_DURATION\n"));
                control_irrigation_temp_time_cleaning_group3_duration = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_CLEANING_GROUP12_DURATION:
        {
            if(counter + sizeof(uint32_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_CLEANING_GROUP12_DURATION wrong size\n"));
            else
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_CLEANING_GROUP12_DURATION\n"));
                control_irrigation_temp_time_cleaning_group12_duration = *(uint32_t *)&internet_packet.internet_packet_body.data[counter];
            }
            break;
        }
        case INTERFACE_COMMAND_CONTROL_IRRIGATION_INFO_ALL:
        {
            if(counter != internet_packet.internet_packet_body.size)
            {
                debug_log(("interface_process_data(): INTERFACE_COMMAND_CONTROL_IRRIGATION_INFO_ALL wrong size\n"));
                return;
            }

            /* device */
            counter = 0;
            internet_packet.internet_packet_body.data[counter++] = INTERFACE_COMMAND_DEVICE_INFO;
            for(counter_2 = 0; counter_2 < DEVICE_ARRAY_SIZE; counter_2++)
                internet_packet.internet_packet_body.data[counter++] = device_array[counter_2].is_active;
            internet_packet.internet_packet_body.size = counter;
            internet_send();

            /* sensor_flow */
            counter = 0;
            internet_packet.internet_packet_body.data[counter++] = INTERFACE_COMMAND_SENSOR_FLOW_INFO;
            for(counter_2 = 0; counter_2 < SENSOR_FLOW_ARRAY_SIZE; counter_2++)
            {
                *(uint32_t *)&internet_packet.internet_packet_body.data[counter] = sensor_flow_array[counter_2].pulses_total;
                counter += sizeof(uint32_t);
                *(uint16_t *)&internet_packet.internet_packet_body.data[counter] = sensor_flow_array[counter_2].pulses_per_liter;
                counter += sizeof(uint16_t);
            }
            internet_packet.internet_packet_body.size = counter;
            internet_send();

            /* sensor_pressure */
            counter = 0;
            internet_packet.internet_packet_body.data[counter++] = INTERFACE_COMMAND_SENSOR_PRESSURE_INFO;
            for(counter_2 = 0; counter_2 < SENSOR_PRESSURE_ARRAY_SIZE; counter_2++)
                internet_packet.internet_packet_body.data[counter++] = sensor_pressure_array[counter_2].pressure;
            internet_packet.internet_packet_body.size = counter;
            internet_send();

            /* control_irrigation_temp */
            counter = 0;
            internet_packet.internet_packet_body.data[counter++] = INTERFACE_COMMAND_CONTROL_IRRIGATION_INFO;
            internet_packet.internet_packet_body.data[counter++] = control_irrigation_temp_program;
            internet_packet.internet_packet_body.data[counter++] = control_irrigation_temp_program_current;
            internet_packet.internet_packet_body.data[counter++] = control_irrigation_temp_status;
            internet_packet.internet_packet_body.data[counter++] = control_irrigation_temp_is_cleaning;
            internet_packet.internet_packet_body.data[counter++] = control_irrigation_temp_is_stopped;
            internet_packet.internet_packet_body.data[counter++] = control_irrigation_temp_is_faulty_detection_active;
            *(uint16_t *)&internet_packet.internet_packet_body.data[counter] = control_irrigation_temp_deliver_volume;
            counter += sizeof(uint16_t);
            *(uint16_t *)&internet_packet.internet_packet_body.data[counter] = control_irrigation_temp_deliver_time;
            counter += sizeof(uint16_t);
            *(uint32_t *)&internet_packet.internet_packet_body.data[counter] = control_irrigation_temp_time_generator_wait;
            counter += sizeof(uint32_t);
            *(uint32_t *)&internet_packet.internet_packet_body.data[counter] = control_irrigation_temp_time_program_duration;
            counter += sizeof(uint32_t);
            *(uint32_t *)&internet_packet.internet_packet_body.data[counter] = control_irrigation_temp_time_cleaning_group3_duration;
            counter += sizeof(uint32_t);
            *(uint32_t *)&internet_packet.internet_packet_body.data[counter] = control_irrigation_temp_time_cleaning_group12_duration;
            counter += sizeof(uint32_t);
            *(uint32_t *)&internet_packet.internet_packet_body.data[counter] = control_irrigation_temp_time_program_start;
            counter += sizeof(uint32_t);
            *(uint32_t *)&internet_packet.internet_packet_body.data[counter] = control_irrigation_temp_time_program_stop;
            counter += sizeof(uint32_t);
            *(uint32_t *)&internet_packet.internet_packet_body.data[counter] = control_irrigation_temp_time_generator_start;
            counter += sizeof(uint32_t);
            *(uint32_t *)&internet_packet.internet_packet_body.data[counter] = millis();
            counter += sizeof(uint32_t);
            internet_packet.internet_packet_body.size = counter;
            internet_send();

            break;
        }
        case INTERNET_COMMAND_PING:
        {
            if(counter + sizeof(uint8_t) != internet_packet.internet_packet_body.size)
                debug_log(("interface_process_data(): INTERNET_COMMAND_PING wrong size\n"));
            else
            {
                if(internet_packet.internet_packet_body.data[counter] == INTERNET_COMMAND_PING_VALUE_REQUEST)
                {
                    debug_log(("interface_process_data(): INTERNET_COMMAND_PING REQUEST\n"));
                    internet_packet.internet_packet_body.data[counter] = INTERNET_COMMAND_PING_VALUE_ANSWER;
                    internet_send();
                }
                else
                    debug_log(("interface_process_data(): INTERNET_COMMAND_PING ANSWER\n"));
            }
            break;
        }
        default:
        {
            debug_log(("interface_process_data(): unknown command\n"));
            break;
        }
    }

    internet_is_receive_wait = FALSE;
    debug_log(("interface_process_data(): end\n"));
}
