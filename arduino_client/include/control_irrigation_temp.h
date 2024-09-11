#ifndef CONTROL_IRRIGATION_TEMP_H
#define CONTROL_IRRIGATION_TEMP_H

#include "my_library.h"

typedef enum
{
    CONTROL_IRRIGATION_TEMP_PROGRAM_GROUP_1,
    CONTROL_IRRIGATION_TEMP_PROGRAM_GROUP_2,
    CONTROL_IRRIGATION_TEMP_PROGRAM_BOTH,
    CONTROL_IRRIGATION_TEMP_PROGRAM_CLEANING,
    CONTROL_IRRIGATION_TEMP_PROGRAM_GARDEN
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

extern bool_t control_irrigation_temp_is_faulty_detection_active;

extern void control_irrigation_temp_init(void);
extern void control_irrigation_temp_start(Control_irrigation_temp_program program, bool_t is_cleaning);
extern void control_irrigation_temp_force_stop(void);
extern void control_irrigation_temp_resume(void);
extern void control_irrigation_temp_faulty_detection_toggle(bool_t is_active);
extern void control_irrigation_temp_update(void);

#endif /* CONTROL_IRRIGATION_TEMP_H */
