#include "control_irrigation.h"

void control_irrigation_init(void)
{

}

#include "device.h"

typedef struct
{
    Device *device;
    uint32_t time_start;
    uint16_t time_duration; /* s */
    uint16_t time_repeat; /* s */
    bool_t is_repeating;
    bool_t is_active;
    bool_t is_paused;
}Control_irrigation;

void control_irrigation_start(void)
{
    Control_irrigation control_irrigation_cleaning;
    Control_irrigation control_irrigation_group_1;
    Control_irrigation control_irrigation_group_2;
    Control_irrigation control_irrigation_supplements[3];

    /* program for cleaning (group 3) */
    control_irrigation_cleaning.device = &device_array[12];
    control_irrigation_cleaning.time_start = millis();
    control_irrigation_cleaning.time_duration = 10;

    /* program for valve group 1 */
    control_irrigation_group_1.device = &device_array[10];
    control_irrigation_group_1.time_start = millis();
    control_irrigation_group_1.time_duration = 900;

    /* program for valve group 2 */
    control_irrigation_group_2.device = &device_array[11];
    control_irrigation_group_2.time_start = millis();
    control_irrigation_group_2.time_duration = 900;

    /* program for irrigation (group 1 and 2) */
    control_irrigation_supplements[0].device = &device_array[3];
    control_irrigation_supplements[0].time_start = millis();
    control_irrigation_supplements[0].time_duration = 720;
    control_irrigation_supplements[1].device = &device_array[7]; /* micro valve control flow */
    control_irrigation_supplements[1].time_start = control_irrigation_supplements[0].time_start;
    control_irrigation_supplements[1].time_duration = 720;
    control_irrigation_supplements[1].device = &device_array[8]; /* macro valve control flow */
    control_irrigation_supplements[1].time_start = control_irrigation_supplements[0].time_start;
    control_irrigation_supplements[1].time_duration = 720;

    (void)control_irrigation_cleaning;
    (void)control_irrigation_group_1;
    (void)control_irrigation_group_2;
    (void)control_irrigation_supplements;
}

void control_irrigation_update(void)
{

}
