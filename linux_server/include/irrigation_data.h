#ifndef IRRIGATION_DATA_H
#define IRRIGATION_DATA_H

#include "straykovsky_standard_library.h"

#define INTERFACE_COMMAND_DEVICE_INFO (41)
#define INTERFACE_COMMAND_DEVICE_TOGGLE (42)
#define INTERFACE_COMMAND_DEVICE_TIME_SWITCH_MINIMUM (43)

#define INTERFACE_COMMAND_SENSOR_FLOW_INFO (61)
#define INTERFACE_COMMAND_SENSOR_FLOW_PULSES_PER_LITER (62)

#define INTERFACE_COMMAND_SENSOR_PRESSURE_INFO (121)

#define INTERFACE_COMMAND_CONTROL_IRRIGATION_INFO (81)
#define INTERFACE_COMMAND_CONTROL_IRRIGATION_INFO_ALL (82)
#define INTERFACE_COMMAND_CONTROL_IRRIGATION_START (83)
#define INTERFACE_COMMAND_CONTROL_IRRIGATION_STOP (84)
#define INTERFACE_COMMAND_CONTROL_IRRIGATION_DELIVER_VOLUME (85)
#define INTERFACE_COMMAND_CONTROL_IRRIGATION_DELIVER_TIME (86)
#define INTERFACE_COMMAND_CONTROL_IRRIGATION_GENERATOR_WAIT_TIME (87)
#define INTERFACE_COMMAND_CONTROL_IRRIGATION_PROGRAM_DURATION (88)
#define INTERFACE_COMMAND_CONTROL_IRRIGATION_CLEANING_GROUP3_DURATION (89)
#define INTERFACE_COMMAND_CONTROL_IRRIGATION_CLEANING_GROUP12_DURATION (90)
#define INTERFACE_COMMAND_CONTROL_IRRIGATION_IS_SEND_INFO (91)
#define INTERFACE_COMMAND_CONTROL_IRRIGATION_RESUME (92)
#define INTERFACE_COMMAND_CONTROL_IRRIGATION_IS_FAULTY_DETECTION_ACTIVE (93)

#define INTERFACE_COMMAND_CONTROL_SAFETY_RESET (101)

#define INTERNET_COMMAND_PING (211)
#define INTERNET_COMMAND_PING_VALUE_REQUEST (10)
#define INTERNET_COMMAND_PING_VALUE_ANSWER (20)

#define DEVICE_ARRAY_SIZE (15U)

typedef struct
{
    bool_t is_active;
}Device;

extern Device device_array[DEVICE_ARRAY_SIZE]; /* 0 generator 1 well 2 acid 3 supplements 4 micro 5 macro 6 click 7 micro 8 macro 9 water 10 group 1 11 group 2 12 group 3 13 garden 14 lamp */

#define SENSOR_FLOW_ARRAY_SIZE (5U)

typedef struct
{
    uint16_t pulses_per_liter;
    uint32_t pulses_total;
}Sensor_flow;

extern Sensor_flow sensor_flow_array[SENSOR_FLOW_ARRAY_SIZE]; /* 0 well 1 micro 2 macro 3 water 4 click */

#define SENSOR_PRESSURE_ARRAY_SIZE (3U)

typedef struct
{
    uint8_t pressure;
}Sensor_pressure;

extern Sensor_pressure sensor_pressure_array[SENSOR_PRESSURE_ARRAY_SIZE]; /* 0 well 1 after well 2 after buffer */

typedef enum
{
    CONTROL_IRRIGATION_TEMP_PROGRAM_GROUP_1,
    CONTROL_IRRIGATION_TEMP_PROGRAM_GROUP_2,
    CONTROL_IRRIGATION_TEMP_PROGRAM_BOTH,
    CONTROL_IRRIGATION_TEMP_PROGRAM_CLEANING
}Control_irrigation_temp_program;

typedef enum
{
    CONTROL_IRRIGATION_TEMP_STATUS_FINISHED,
    CONTROL_IRRIGATION_TEMP_STATUS_PREPARING,
    CONTROL_IRRIGATION_TEMP_STATUS_READY,
    CONTROL_IRRIGATION_TEMP_STATUS_FINISHING
}Control_irrigation_temp_status;

extern Control_irrigation_temp_program control_irrigation_temp_program;
extern Control_irrigation_temp_program control_irrigation_temp_program_current;
extern Control_irrigation_temp_status control_irrigation_temp_status;
extern bool_t control_irrigation_temp_is_cleaning;
extern bool_t control_irrigation_temp_is_stopped;
extern uint16_t control_irrigation_temp_deliver_volume; /* ml */
extern uint16_t control_irrigation_temp_deliver_time; /* s */
extern uint32_t control_irrigation_temp_time_generator_wait; /* ms */
extern uint32_t control_irrigation_temp_time_program_duration; /* ms */
extern uint32_t control_irrigation_temp_time_cleaning_group3_duration; /* ms */
extern uint32_t control_irrigation_temp_time_cleaning_group12_duration; /* ms */
extern uint32_t control_irrigation_temp_time_program_start; /* ms */
extern uint32_t control_irrigation_temp_time_program_stop; /* ms */
extern uint32_t control_irrigation_temp_time_generator_start; /* ms */

extern uint32_t control_irrigation_temp_time_current; /* ms */

extern bool_t control_irrigation_temp_is_faulty_detection_active;

#endif /* IRRIGATION_DATA_H */
