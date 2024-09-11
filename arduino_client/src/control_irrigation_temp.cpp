#include "my_library.h"
#include "control_irrigation_temp.h"
#include "device.h"
#include "control_flow.h"
#include "control_safety.h"

Control_irrigation_temp_program control_irrigation_temp_program;
Control_irrigation_temp_program control_irrigation_temp_program_current;
Control_irrigation_temp_status control_irrigation_temp_status;
bool_t control_irrigation_temp_is_cleaning;
bool_t control_irrigation_temp_is_stopped;
uint16_t control_irrigation_temp_deliver_volume = 5000;
uint16_t control_irrigation_temp_deliver_time = 720;
uint32_t control_irrigation_temp_time_generator_wait = 5000;
uint32_t control_irrigation_temp_time_program_duration = 1000LU * 60LU * 15LU; /* 15m */
uint32_t control_irrigation_temp_time_cleaning_group3_duration = 10000;
uint32_t control_irrigation_temp_time_cleaning_group12_duration = 3000;
uint32_t control_irrigation_temp_time_program_start;
uint32_t control_irrigation_temp_time_program_stop;
uint32_t control_irrigation_temp_time_generator_start;

bool_t control_irrigation_temp_is_faulty_detection_active;

static uint32_t control_irrigation_temp_time_cleaning_duration;

void control_irrigation_temp_init(void)
{
    control_irrigation_temp_is_faulty_detection_active = TRUE;
}

void control_irrigation_temp_start(Control_irrigation_temp_program program, bool_t is_cleaning)
{
    control_irrigation_temp_program = control_irrigation_temp_program_current = program;
    control_irrigation_temp_is_cleaning = is_cleaning;
    control_irrigation_temp_time_cleaning_duration = control_irrigation_temp_time_cleaning_group3_duration;
    control_irrigation_temp_is_stopped = FALSE;

    if(control_irrigation_temp_status != CONTROL_IRRIGATION_TEMP_STATUS_READY)
    {
        device_toggle(&device_array[0], TRUE);

        control_irrigation_temp_time_generator_start = millis();
        control_irrigation_temp_status = CONTROL_IRRIGATION_TEMP_STATUS_PREPARING;
        return;
    }

    if(control_irrigation_temp_program == CONTROL_IRRIGATION_TEMP_PROGRAM_GROUP_1)
    {
        device_toggle(&device_array[2], TRUE);
        device_toggle(&device_array[10], TRUE);
        if(control_irrigation_temp_deliver_volume != 1)
        {
            control_flow_start(&control_flow_array[1], control_irrigation_temp_deliver_volume, control_irrigation_temp_deliver_time);
            control_flow_start(&control_flow_array[2], control_irrigation_temp_deliver_volume, control_irrigation_temp_deliver_time);
            control_flow_array[3].sensor_flow->pulses_total = 0;
        }
        control_irrigation_temp_time_program_start = millis();
        return;
    }

    if(control_irrigation_temp_program == CONTROL_IRRIGATION_TEMP_PROGRAM_GROUP_2)
    {
        device_toggle(&device_array[2], TRUE);
        device_toggle(&device_array[11], TRUE);
        if(control_irrigation_temp_deliver_volume != 1)
        {
            control_flow_start(&control_flow_array[1], control_irrigation_temp_deliver_volume, control_irrigation_temp_deliver_time);
            control_flow_start(&control_flow_array[2], control_irrigation_temp_deliver_volume, control_irrigation_temp_deliver_time);
            control_flow_array[3].sensor_flow->pulses_total = 0;
        }
        control_irrigation_temp_time_program_start = millis();
        return;
    }

    if(control_irrigation_temp_program == CONTROL_IRRIGATION_TEMP_PROGRAM_BOTH)
    {
        control_irrigation_temp_start(CONTROL_IRRIGATION_TEMP_PROGRAM_GROUP_1, control_irrigation_temp_is_cleaning);
        control_irrigation_temp_program = CONTROL_IRRIGATION_TEMP_PROGRAM_BOTH;
        return;
    }

    if(control_irrigation_temp_program == CONTROL_IRRIGATION_TEMP_PROGRAM_CLEANING)
    {
        device_toggle(&device_array[12], TRUE);
        control_irrigation_temp_time_program_start = millis();
        return;
    }

    if(control_irrigation_temp_program == CONTROL_IRRIGATION_TEMP_PROGRAM_GARDEN)
    {
        control_flow_start(&control_flow_array[5], 0, control_irrigation_temp_time_program_duration / 1000);
        control_irrigation_temp_time_program_start = millis();
        return;
    }
}

void control_irrigation_temp_force_stop(void)
{
    control_safety_devices_stop();

    control_flow_stop(&control_flow_array[1]);
    control_flow_stop(&control_flow_array[2]);
    control_flow_stop(&control_flow_array[5]);

    control_irrigation_temp_time_program_stop = millis();

    if(control_irrigation_temp_status != CONTROL_IRRIGATION_TEMP_STATUS_PREPARING && !control_irrigation_temp_is_stopped)
    {
        if(control_irrigation_temp_is_faulty_detection_active && control_flow_faulty_detection_status != CONTROL_FLOW_FAULTY_DETECTION_STATUS_NOT_DETECTED)
            control_irrigation_temp_is_stopped = control_flow_faulty_detection_status;
        else
            control_irrigation_temp_is_stopped = CONTROL_FLOW_FAULTY_DETECTION_STATUS_STOPPED_BY_USER;
    }

    control_irrigation_temp_status = CONTROL_IRRIGATION_TEMP_STATUS_FINISHED;
}

void control_irrigation_temp_resume(void)
{
    if(!control_irrigation_temp_is_stopped)
        return;

    if(control_irrigation_temp_status != CONTROL_IRRIGATION_TEMP_STATUS_READY)
    {
        device_toggle(&device_array[0], TRUE);

        control_irrigation_temp_time_generator_start = millis();
        control_irrigation_temp_status = CONTROL_IRRIGATION_TEMP_STATUS_PREPARING;
        return;
    }

    control_irrigation_temp_is_stopped = FALSE;

    if(control_irrigation_temp_program_current == CONTROL_IRRIGATION_TEMP_PROGRAM_GROUP_1)
    {
        control_irrigation_temp_time_program_start = millis() - (control_irrigation_temp_time_program_stop - control_irrigation_temp_time_program_start);
        device_toggle(&device_array[2], TRUE);
        device_toggle(&device_array[10], TRUE);
        if(control_irrigation_temp_deliver_volume != 1)
        {
            control_flow_faulty_detection_status = CONTROL_FLOW_FAULTY_DETECTION_STATUS_NOT_DETECTED;
            control_flow_faulty_detection_time_last = millis();
            control_flow_array[1].is_active = TRUE;
            control_flow_array[2].is_active = TRUE;
            control_flow_array[1].time_start = control_irrigation_temp_time_program_start;
            control_flow_array[2].time_start = control_irrigation_temp_time_program_start;
        }
        return;
    }

    if(control_irrigation_temp_program_current == CONTROL_IRRIGATION_TEMP_PROGRAM_GROUP_2)
    {
        control_irrigation_temp_time_program_start = millis() - (control_irrigation_temp_time_program_stop - control_irrigation_temp_time_program_start);
        device_toggle(&device_array[2], TRUE);
        device_toggle(&device_array[11], TRUE);
        if(control_irrigation_temp_deliver_volume != 1)
        {
            control_flow_faulty_detection_status = CONTROL_FLOW_FAULTY_DETECTION_STATUS_NOT_DETECTED;
            control_flow_faulty_detection_time_last = millis();
            control_flow_array[1].is_active = TRUE;
            control_flow_array[2].is_active = TRUE;
            control_flow_array[1].time_start = control_irrigation_temp_time_program_start;
            control_flow_array[2].time_start = control_irrigation_temp_time_program_start;
        }
        return;
    }

    if(control_irrigation_temp_program_current == CONTROL_IRRIGATION_TEMP_PROGRAM_CLEANING)
    {
        control_irrigation_temp_time_cleaning_duration = control_irrigation_temp_time_cleaning_group3_duration;
        device_toggle(&device_array[12], TRUE);
        control_irrigation_temp_time_program_start = millis();
        return;
    }

    if(control_irrigation_temp_program_current == CONTROL_IRRIGATION_TEMP_PROGRAM_GARDEN)
    {
        control_irrigation_temp_time_program_start = millis() - (control_irrigation_temp_time_program_stop - control_irrigation_temp_time_program_start);
        control_flow_array[5].is_active = TRUE;
        control_flow_array[5].time_start = control_irrigation_temp_time_program_start;
        return;
    }
}

void control_irrigation_temp_faulty_detection_toggle(bool_t is_active)
{
    control_irrigation_temp_is_faulty_detection_active = is_active;
    control_flow_faulty_detection_time_last = millis();
    control_flow_faulty_detection_status = CONTROL_FLOW_FAULTY_DETECTION_STATUS_NOT_DETECTED;
}

void control_irrigation_temp_update(void)
{
    if(control_irrigation_temp_status == CONTROL_IRRIGATION_TEMP_STATUS_FINISHED)
        return;

    if(control_irrigation_temp_status == CONTROL_IRRIGATION_TEMP_STATUS_PREPARING)
    {
        if(millis() - control_irrigation_temp_time_generator_start < control_irrigation_temp_time_generator_wait)
            return;

        device_toggle(&device_array[1], TRUE);

        control_irrigation_temp_status = CONTROL_IRRIGATION_TEMP_STATUS_READY;

        if(!control_irrigation_temp_is_stopped)
            control_irrigation_temp_start(control_irrigation_temp_program, control_irrigation_temp_is_cleaning);
        else
            control_irrigation_temp_resume();
            
        return;
    }

    /* faulty flow detection */
    if(control_irrigation_temp_is_faulty_detection_active && control_flow_faulty_detection_status != CONTROL_FLOW_FAULTY_DETECTION_STATUS_NOT_DETECTED)
    {
        control_irrigation_temp_force_stop();
        debug_log(("control_irrigation_temp_update(): faulty flow %d\n", control_flow_faulty_detection_status));
        return;
    }

    if(control_irrigation_temp_status == CONTROL_IRRIGATION_TEMP_STATUS_FINISHING)
    {
        if(millis() - control_irrigation_temp_time_generator_start < control_irrigation_temp_time_generator_wait)
            return;

        device_toggle(&device_array[0], FALSE);
        control_irrigation_temp_status = CONTROL_IRRIGATION_TEMP_STATUS_FINISHED;

        return;
    }

    if(control_irrigation_temp_program == CONTROL_IRRIGATION_TEMP_PROGRAM_GROUP_1)
    {
        if(millis() - control_irrigation_temp_time_program_start < control_irrigation_temp_time_program_duration)
            return;

        device_toggle(&device_array[2], FALSE);
        device_toggle(&device_array[10], FALSE);

        if(control_irrigation_temp_is_cleaning)
        {
            control_irrigation_temp_start(CONTROL_IRRIGATION_TEMP_PROGRAM_CLEANING, TRUE);
            return;
        }

        device_toggle(&device_array[1], FALSE);

        control_irrigation_temp_time_generator_start = millis();
        control_irrigation_temp_status = CONTROL_IRRIGATION_TEMP_STATUS_FINISHING;

        return;
    }

    if(control_irrigation_temp_program == CONTROL_IRRIGATION_TEMP_PROGRAM_GROUP_2)
    {
        if(millis() - control_irrigation_temp_time_program_start < control_irrigation_temp_time_program_duration)
            return;

        device_toggle(&device_array[2], FALSE);
        device_toggle(&device_array[11], FALSE);

        if(control_irrigation_temp_is_cleaning)
        {
            control_irrigation_temp_start(CONTROL_IRRIGATION_TEMP_PROGRAM_CLEANING, TRUE);
            return;
        }

        device_toggle(&device_array[1], FALSE);

        control_irrigation_temp_time_generator_start = millis();
        control_irrigation_temp_status = CONTROL_IRRIGATION_TEMP_STATUS_FINISHING;

        return;
    }

    if(control_irrigation_temp_program == CONTROL_IRRIGATION_TEMP_PROGRAM_BOTH)
    {
        if(millis() - control_irrigation_temp_time_program_start < control_irrigation_temp_time_program_duration)
            return;

        /* group 1 */
        if(device_array[10].is_active)
        {
            device_toggle(&device_array[10], FALSE);
            control_irrigation_temp_start(CONTROL_IRRIGATION_TEMP_PROGRAM_GROUP_2, control_irrigation_temp_is_cleaning);
            control_irrigation_temp_program = CONTROL_IRRIGATION_TEMP_PROGRAM_BOTH;
            return;
        }

        /* group 2 */
        device_toggle(&device_array[2], FALSE);
        device_toggle(&device_array[11], FALSE);

        if(control_irrigation_temp_is_cleaning)
        {
            control_irrigation_temp_start(CONTROL_IRRIGATION_TEMP_PROGRAM_CLEANING, control_irrigation_temp_is_cleaning);
            return;
        }

        device_toggle(&device_array[1], FALSE);

        control_irrigation_temp_time_generator_start = millis();
        control_irrigation_temp_status = CONTROL_IRRIGATION_TEMP_STATUS_FINISHING;

        return;
    }

    if(control_irrigation_temp_program == CONTROL_IRRIGATION_TEMP_PROGRAM_CLEANING)
    {
        if(millis() - control_irrigation_temp_time_program_start < control_irrigation_temp_time_cleaning_duration)
            return;

        /* group 1 */
        if(device_array[10].is_active)
        {
            device_toggle(&device_array[10], FALSE);
            device_toggle(&device_array[11], TRUE);

            control_irrigation_temp_time_program_start = millis();

            return;
        }

        /* group 2 */
        if(device_array[11].is_active)
        {
            device_toggle(&device_array[1], FALSE);
            device_toggle(&device_array[11], FALSE);

            control_irrigation_temp_time_generator_start = millis();
            control_irrigation_temp_time_cleaning_duration = control_irrigation_temp_time_cleaning_group3_duration;
            control_irrigation_temp_status = CONTROL_IRRIGATION_TEMP_STATUS_FINISHING;

            return;
        }

        /* group 3 */
        device_toggle(&device_array[12], FALSE);
        device_toggle(&device_array[10], TRUE);

        control_irrigation_temp_time_program_start = millis();
        control_irrigation_temp_time_cleaning_duration = control_irrigation_temp_time_cleaning_group12_duration;

        return;
    }

    if(control_irrigation_temp_program == CONTROL_IRRIGATION_TEMP_PROGRAM_GARDEN)
    {
        if(millis() - control_irrigation_temp_time_program_start < control_irrigation_temp_time_program_duration)
            return;

        device_toggle(&device_array[1], FALSE);

        control_irrigation_temp_time_generator_start = millis();
        control_irrigation_temp_status = CONTROL_IRRIGATION_TEMP_STATUS_FINISHING;

        return;
    }
}
