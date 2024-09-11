#include "control_flow.h"
#include "sensor_pressure.h"

Control_flow control_flow_array[CONTROL_FLOW_ARRAY_SIZE];

Control_flow_faulty_detection_status control_flow_faulty_detection_status;
uint32_t control_flow_faulty_detection_time_last;

static uint8_t counter;

void control_flow_init(void)
{
    debug_log(("control_flow_init()\n"));

    /* auto irrigation for well */
    control_flow_array[0].is_active = FALSE;
    control_flow_array[0].is_pause = FALSE;
    control_flow_array[0].device = &device_array[1];
    control_flow_array[0].sensor_flow = &sensor_flow_array[0];

    /* auto irrigation for supplements micro */
    control_flow_array[1].is_active = FALSE;
    control_flow_array[1].is_pause = FALSE;
    control_flow_array[1].device = &device_array[7];
    control_flow_array[1].sensor_flow = &sensor_flow_array[1];

    /* auto irrigation for supplements macro */
    control_flow_array[2].is_active = FALSE;
    control_flow_array[2].is_pause = FALSE;
    control_flow_array[2].device = &device_array[8];
    control_flow_array[2].sensor_flow = &sensor_flow_array[2];

    /* auto irrigation for supplements water */
    control_flow_array[3].is_active = FALSE;
    control_flow_array[3].is_pause = FALSE;
    control_flow_array[3].device = &device_array[9];
    control_flow_array[3].sensor_flow = &sensor_flow_array[3];

    /* auto irrigation for click */
    control_flow_array[4].is_active = FALSE;
    control_flow_array[4].is_pause = FALSE;
    control_flow_array[4].device = &device_array[6];
    control_flow_array[4].sensor_flow = &sensor_flow_array[4];

    /* auto irrigation for garden */
    control_flow_array[5].is_active = FALSE;
    control_flow_array[5].is_pause = FALSE;
    control_flow_array[5].device = &device_array[13];
    control_flow_array[5].sensor_flow = &sensor_flow_array[0];

    control_flow_faulty_detection_status = CONTROL_FLOW_FAULTY_DETECTION_STATUS_NOT_DETECTED;

    debug_log(("control_flow_init(): success\n"));
}

void control_flow_start(Control_flow *control_flow, uint16_t volume_to_deliver, uint16_t time_to_deliver)
{
    debug_log(("control_flow_start(%d, %d, %d)\n", control_flow->device->pin, volume_to_deliver, time_to_deliver));

    control_flow->sensor_flow->pulses_total = control_flow->pulses_last = 0;
    control_flow->pulses_to_deliver = control_flow->sensor_flow->pulses_per_liter / 1000.0 * volume_to_deliver;
    control_flow->time_to_deliver = time_to_deliver;
    control_flow->time_start = millis();
    control_flow->is_active = TRUE;
    control_flow->is_pause = FALSE;

    control_flow_faulty_detection_time_last = millis();
    control_flow_faulty_detection_status = CONTROL_FLOW_FAULTY_DETECTION_STATUS_NOT_DETECTED;

    debug_log(("control_flow_start(): success\n"));
}

void control_flow_stop(Control_flow *control_flow)
{
    debug_log(("control_flow_stop(%d);\n", control_flow->device->pin));

    device_toggle(control_flow->device, FALSE);
    control_flow->is_active = FALSE;
    control_flow->is_pause = FALSE;

    /* if control_flow_array is micro, macro or water then supplements pump needs to be toggled off too */
    if(control_flow->sensor_flow == &sensor_flow_array[1] || control_flow->sensor_flow == &sensor_flow_array[2] || control_flow->sensor_flow == &sensor_flow_array[3])
        if(!control_flow_array[1].is_active && !control_flow_array[2].is_active && !control_flow_array[3].is_active)
        {
            device_toggle(&device_array[3], FALSE);
            device_toggle(&device_array[9], FALSE);
        }

    debug_log(("control_flow_stop(): success\n"));
}

void control_flow_pause(Control_flow *control_flow, bool_t is_pause)
{
    debug_log(("control_flow_pause(%d, %d);\n", control_flow->device->pin, is_pause));

    if(is_pause)
    {
        device_toggle(control_flow->device, FALSE);
        control_flow->time_pause = millis();
    }
    else
        control_flow->time_start += millis() - control_flow->time_pause;

    control_flow->is_pause = is_pause;

    debug_log(("control_flow_pause(): success\n"));
}

void control_flow_update(void)
{
    for(counter = 0; counter < CONTROL_FLOW_ARRAY_SIZE; counter++)
    {
        if(!control_flow_array[counter].is_active || control_flow_array[counter].is_pause)
            continue;

        if((control_flow_array[counter].pulses_to_deliver && control_flow_array[counter].sensor_flow->pulses_total >= control_flow_array[counter].pulses_to_deliver) || (control_flow_array[counter].time_to_deliver && millis() - control_flow_array[counter].time_start >= control_flow_array[counter].time_to_deliver * 1000LU))
        {
            /* if pulses are delivered or time passed else if both pulses and time are greater than zero then flow control */
            control_flow_stop(&control_flow_array[counter]);
            debug_log(("control_flow_update(): finished %d\n", control_flow_array[counter].device->pin));
        }
        else if(!control_flow_array[counter].pulses_to_deliver || !control_flow_array[counter].time_to_deliver)
        {
            /* if pulses_to_deliver or time_to_deliver = 0 it's unlimited program */
            if(device_is_time_switch_minimum_passed(control_flow_array[counter].device) && !control_flow_array[counter].device->is_active)
                device_toggle(control_flow_array[counter].device, TRUE);
        }
        else if(control_flow_array[counter].pulses_to_deliver / (float)control_flow_array[counter].time_to_deliver * ((millis() - control_flow_array[counter].time_start) / 1000.0) > control_flow_array[counter].sensor_flow->pulses_total)
        {
            if(device_is_time_switch_minimum_passed(control_flow_array[counter].device) && !control_flow_array[counter].device->is_active)
                device_toggle(control_flow_array[counter].device, TRUE);
        }
        else
        {
            if(device_is_time_switch_minimum_passed(control_flow_array[counter].device) && control_flow_array[counter].device->is_active)
                device_toggle(control_flow_array[counter].device, FALSE);
        }
    }

    /* if control_flow_array is micro, macro or water, and they are active then supplements pump needs to be toggled on too */
    if(control_flow_array[1].is_active || control_flow_array[2].is_active || control_flow_array[3].is_active)
        if(!device_array[3].is_active && device_is_time_switch_minimum_passed(&device_array[3]))
            device_toggle(&device_array[3], TRUE);

    /* if control_flow_array micro or macro are active and devices are not active then water valve pump needs to be toggled on else off */
    if(control_flow_array[1].is_active || control_flow_array[2].is_active)
    {
        if(!control_flow_array[1].device->is_active && !control_flow_array[2].device->is_active)
        {
            if(!device_array[9].is_active && device_is_time_switch_minimum_passed(&device_array[9]))
                device_toggle(&device_array[9], TRUE);
        }
        else if(!control_flow_array[3].is_active)
        {
            if(device_array[9].is_active && device_is_time_switch_minimum_passed(&device_array[9]))
                device_toggle(&device_array[9], FALSE);
        }
    }

    /* faulty flow detection */
    if(millis() - control_flow_faulty_detection_time_last > CONTROL_FLOW_FAULTY_DETECTION_TIME_INTERVAL)
    {
        for(counter = 0; counter < CONTROL_FLOW_ARRAY_SIZE; counter++)
        {
            if(!control_flow_array[counter].is_active || control_flow_array[counter].is_pause)
                continue;

            /* don't do faulty flow detection on garden program */
            if(counter != 5)
            {
                if(control_flow_array[counter].sensor_flow->pulses_total - control_flow_array[counter].pulses_last < CONTROL_FLOW_FAULTY_DETECTION_MINIMUM_PULSES)
                    control_flow_faulty_detection_status = CONTROL_FLOW_FAULTY_DETECTION_STATUS_DETECTED_FLOW_LESS;
                else if(control_flow_array[counter].sensor_flow->pulses_total - control_flow_array[counter].pulses_last > CONTROL_FLOW_FAULTY_DETECTION_MAXIMUM_PULSES)
                    control_flow_faulty_detection_status = CONTROL_FLOW_FAULTY_DETECTION_STATUS_DETECTED_FLOW_MORE;

                control_flow_array[counter].pulses_last = control_flow_array[counter].sensor_flow->pulses_total;
            }
            /* faulty pressure detection */
            if(sensor_pressure_array[2].pressure == 0)
                control_flow_faulty_detection_status = CONTROL_FLOW_FAULTY_DETECTION_STATUS_DETECTED_PRESSURE_ZERO;
        }

        control_flow_faulty_detection_time_last = millis();
    }
}
