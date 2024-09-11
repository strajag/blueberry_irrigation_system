#include "irrigation_data.h"

Device device_array[DEVICE_ARRAY_SIZE];

Sensor_flow sensor_flow_array[SENSOR_FLOW_ARRAY_SIZE];

Sensor_pressure sensor_pressure_array[SENSOR_PRESSURE_ARRAY_SIZE];

Control_irrigation_temp_program control_irrigation_temp_program_current;
Control_irrigation_temp_program control_irrigation_temp_program;
Control_irrigation_temp_status control_irrigation_temp_status;
bool_t control_irrigation_temp_is_cleaning;
bool_t control_irrigation_temp_is_stopped;
uint16_t control_irrigation_temp_deliver_volume; /* ml */
uint16_t control_irrigation_temp_deliver_time; /* s */
uint32_t control_irrigation_temp_time_generator_wait; /* ms */
uint32_t control_irrigation_temp_time_program_duration; /* ms */
uint32_t control_irrigation_temp_time_cleaning_group3_duration; /* ms */
uint32_t control_irrigation_temp_time_cleaning_group12_duration; /* ms */
uint32_t control_irrigation_temp_time_program_start; /* ms */
uint32_t control_irrigation_temp_time_program_stop; /* ms */
uint32_t control_irrigation_temp_time_generator_start; /* ms */

uint32_t control_irrigation_temp_time_current;

bool_t control_irrigation_temp_is_faulty_detection_active;
