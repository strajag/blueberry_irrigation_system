#include "device.h"
#include "pins.h"

Device device_array[DEVICE_ARRAY_SIZE];
Device_extension device_extension_array[DEVICE_EXTENSION_ARRAY_SIZE];
Device *device_extended_array[DEVICE_EXTENSION_ARRAY_SIZE];

static uint8_t index_device;
static uint8_t index_buffer;
static uint8_t index_buffer_total;

void device_init(void)
{
    debug_log(("device_init()\n"));

    /* device generator */
    device_array[0].pin = DEVICE_PIN_GENERATOR;
    device_array[0].is_active = FALSE;
    device_array[0].time_switch_last = 0;
    device_array[0].time_switch_minimum = 100;
    device_array[0].device_extension = NULL;
    pinMode(device_array[0].pin, INPUT_PULLUP);
    pinMode(device_array[0].pin, OUTPUT);
    digitalWrite(device_array[0].pin, DEVICE_RELAY_OFF);

    /* device pump well */
    device_array[1].pin = DEVICE_PIN_PUMP_WELL;
    device_array[1].is_active = FALSE;
    device_array[1].time_switch_last = 0;
    device_array[1].time_switch_minimum = 100;
    device_array[1].device_extension = NULL;
    pinMode(device_array[1].pin, INPUT_PULLUP);
    pinMode(device_array[1].pin, OUTPUT);
    digitalWrite(device_array[1].pin, DEVICE_RELAY_OFF);

    /* device pump acid */
    device_array[2].pin = DEVICE_PIN_PUMP_ACID;
    device_array[2].is_active = FALSE;
    device_array[2].time_switch_last = 0;
    device_array[2].time_switch_minimum = 50;
    device_array[2].device_extension = NULL;
    pinMode(device_array[2].pin, INPUT_PULLUP);
    pinMode(device_array[2].pin, OUTPUT);
    digitalWrite(device_array[2].pin, DEVICE_RELAY_OFF);

    /* device pump supplements */
    device_array[3].pin = DEVICE_PIN_PUMP_SUPPLEMENTS;
    device_array[3].is_active = FALSE;
    device_array[3].time_switch_last = 0;
    device_array[3].time_switch_minimum = 50;
    device_array[3].device_extension = NULL;
    pinMode(device_array[3].pin, INPUT_PULLUP);
    pinMode(device_array[3].pin, OUTPUT);
    digitalWrite(device_array[3].pin, DEVICE_RELAY_OFF);

    /* device pump supplements micro */
    device_array[4].pin = DEVICE_PIN_PUMP_SUPPLEMENTS_MICRO;
    device_array[4].is_active = FALSE;
    device_array[4].time_switch_last = 0;
    device_array[4].time_switch_minimum = 50;
    device_array[4].device_extension = NULL;
    pinMode(device_array[4].pin, INPUT_PULLUP);
    pinMode(device_array[4].pin, OUTPUT);
    digitalWrite(device_array[4].pin, DEVICE_RELAY_OFF);

    /* device pump supplements macro */
    device_array[5].pin = DEVICE_PIN_PUMP_SUPPLEMENTS_MACRO;
    device_array[5].is_active = FALSE;
    device_array[5].time_switch_last = 0;
    device_array[5].time_switch_minimum = 50;
    device_array[5].device_extension = NULL;
    pinMode(device_array[5].pin, INPUT_PULLUP);
    pinMode(device_array[5].pin, OUTPUT);
    digitalWrite(device_array[5].pin, DEVICE_RELAY_OFF);

    /* device pump click */
    device_extension_array[0].pin = DEVICE_PIN_PUMP_CLICK_OFF;
    device_extension_array[0].is_hold = FALSE;
    device_extension_array[0].time_switch_hold_minimum = 5;
    device_extension_array[0].time_switch_hold_last = 0;
    pinMode(device_extension_array[0].pin, INPUT_PULLUP);
    pinMode(device_extension_array[0].pin, OUTPUT);
    digitalWrite(device_extension_array[0].pin, DEVICE_RELAY_OFF);
    device_array[6].pin = DEVICE_PIN_PUMP_CLICK_ON;
    device_array[6].is_active = FALSE;
    device_array[6].time_switch_last = 0;
    device_array[6].time_switch_minimum = 150;
    device_array[6].device_extension = &device_extension_array[0];
    pinMode(device_array[6].pin, INPUT_PULLUP);
    pinMode(device_array[6].pin, OUTPUT);
    digitalWrite(device_array[6].pin, DEVICE_RELAY_OFF);
    device_extended_array[0] = &device_array[6];

    /* device valve supplements micro */
    device_array[7].pin = DEVICE_PIN_VALVE_SUPPLEMENTS_MICRO;
    device_array[7].is_active = FALSE;
    device_array[7].time_switch_last = 0;
    device_array[7].time_switch_minimum = 5;
    device_array[7].device_extension = NULL;
    pinMode(device_array[7].pin, INPUT_PULLUP);
    pinMode(device_array[7].pin, OUTPUT);
    digitalWrite(device_array[7].pin, DEVICE_RELAY_OFF);

    /* device valve supplements macro */
    device_array[8].pin = DEVICE_PIN_VALVE_SUPPLEMENTS_MACRO;
    device_array[8].is_active = FALSE;
    device_array[8].time_switch_last = 0;
    device_array[8].time_switch_minimum = 5;
    device_array[8].device_extension = NULL;
    pinMode(device_array[8].pin, INPUT_PULLUP);
    pinMode(device_array[8].pin, OUTPUT);
    digitalWrite(device_array[8].pin, DEVICE_RELAY_OFF);

    /* device valve supplements water */
    device_array[9].pin = DEVICE_PIN_VALVE_SUPPLEMENTS_WATER;
    device_array[9].is_active = FALSE;
    device_array[9].time_switch_last = 0;
    device_array[9].time_switch_minimum = 5;
    device_array[9].device_extension = NULL;
    pinMode(device_array[9].pin, INPUT_PULLUP);
    pinMode(device_array[9].pin, OUTPUT);
    digitalWrite(device_array[9].pin, DEVICE_RELAY_OFF);

    /* device valve group 1 */
    device_array[10].pin = DEVICE_PIN_VALVE_GROUP_1;
    device_array[10].is_active = FALSE;
    device_array[10].time_switch_last = 0;
    device_array[10].time_switch_minimum = 20;
    device_array[10].device_extension = NULL;
    pinMode(device_array[10].pin, INPUT_PULLUP);
    pinMode(device_array[10].pin, OUTPUT);
    digitalWrite(device_array[10].pin, DEVICE_RELAY_OFF);

    /* device valve group 2 */
    device_array[11].pin = DEVICE_PIN_VALVE_GROUP_2;
    device_array[11].is_active = FALSE;
    device_array[11].time_switch_last = 0;
    device_array[11].time_switch_minimum = 20;
    device_array[11].device_extension = NULL;
    pinMode(device_array[11].pin, INPUT_PULLUP);
    pinMode(device_array[11].pin, OUTPUT);
    digitalWrite(device_array[11].pin, DEVICE_RELAY_OFF);

    /* device valve group 3 */
    device_array[12].pin = DEVICE_PIN_VALVE_GROUP_3;
    device_array[12].is_active = FALSE;
    device_array[12].time_switch_last = 0;
    device_array[12].time_switch_minimum = 20;
    device_array[12].device_extension = NULL;
    pinMode(device_array[12].pin, INPUT_PULLUP);
    pinMode(device_array[12].pin, OUTPUT);
    digitalWrite(device_array[12].pin, DEVICE_RELAY_OFF);

    /* device valve garden */
    device_array[13].pin = DEVICE_PIN_VALVE_GARDEN;
    device_array[13].is_active = FALSE;
    device_array[13].time_switch_last = 0;
    device_array[13].time_switch_minimum = 20;
    device_array[13].device_extension = NULL;
    pinMode(device_array[13].pin, INPUT_PULLUP);
    pinMode(device_array[13].pin, OUTPUT);
    digitalWrite(device_array[13].pin, DEVICE_RELAY_OFF);

    /* device lamp */
    device_array[14].pin = DEVICE_PIN_LAMP;
    device_array[14].is_active = FALSE;
    device_array[14].time_switch_last = 0;
    device_array[14].time_switch_minimum = 20;
    device_array[14].device_extension = NULL;
    pinMode(device_array[14].pin, INPUT_PULLUP);
    pinMode(device_array[14].pin, OUTPUT);
    digitalWrite(device_array[14].pin, DEVICE_RELAY_OFF);

    debug_log(("device_init(): success\n"));
}

void device_toggle(Device *device, bool_t is_active)
{
    debug_log(("device_toggle(%d, %d)\n", device->pin, is_active));

    if(device->device_extension)
    {
        if(device->device_extension->is_hold)
            digitalWrite(device->is_active ? device->pin : device->device_extension->pin, DEVICE_RELAY_OFF);
        else
            device->device_extension->is_hold = TRUE;

        digitalWrite(is_active ? device->pin : device->device_extension->pin, DEVICE_RELAY_ON);
        device->device_extension->time_switch_hold_last = millis();
    }
    else
        digitalWrite(device->pin, is_active ? DEVICE_RELAY_ON : DEVICE_RELAY_OFF);

    device->time_switch_last = millis();
    device->is_active = is_active;

    debug_log(("device_toggle(): success\n"));
}

bool_t device_is_time_switch_minimum_passed(Device *device)
{
    return millis() - device->time_switch_last >= device->time_switch_minimum * 100LU;
}

void device_update(void)
{
    for(index_device = 0; index_device < DEVICE_EXTENSION_ARRAY_SIZE; index_device++)
    {
        if(!device_extension_array[index_device].is_hold)
            continue;

        if(millis() - device_extension_array[index_device].time_switch_hold_last < device_extension_array[index_device].time_switch_hold_minimum * 100LU)
            continue;

        debug_log(("device_update(): %d\n", device_extended_array[index_device]->pin));
        digitalWrite(device_extended_array[index_device]->is_active ? device_extended_array[index_device]->pin : device_extension_array[index_device].pin, DEVICE_RELAY_OFF);
        device_extension_array[index_device].is_hold = FALSE;
    }
}

extern uint8_t device_save_to_array(Device *device, uint8_t *buffer)
{
    index_buffer = 0;

    buffer[index_buffer] = device->pin; index_buffer++;
    buffer[index_buffer] = device->is_active; index_buffer++;
    buffer[index_buffer] = device->time_switch_minimum; index_buffer++;
    buffer[index_buffer] = ((uint8_t *)&device->time_switch_last)[0]; index_buffer++;
    buffer[index_buffer] = ((uint8_t *)&device->time_switch_last)[1]; index_buffer++;
    buffer[index_buffer] = ((uint8_t *)&device->time_switch_last)[2]; index_buffer++;
    buffer[index_buffer] = ((uint8_t *)&device->time_switch_last)[3]; index_buffer++;
    buffer[index_buffer] = device->device_extension != NULL; index_buffer++;

    if(device->device_extension)
    {
        buffer[index_buffer] = device->device_extension->pin; index_buffer++;
        buffer[index_buffer] = device->device_extension->is_hold; index_buffer++;
        buffer[index_buffer] = device->device_extension->time_switch_hold_minimum; index_buffer++;
        buffer[index_buffer] = ((uint8_t *)&device->device_extension->time_switch_hold_last)[0]; index_buffer++;
        buffer[index_buffer] = ((uint8_t *)&device->device_extension->time_switch_hold_last)[1]; index_buffer++;
        buffer[index_buffer] = ((uint8_t *)&device->device_extension->time_switch_hold_last)[2]; index_buffer++;
        buffer[index_buffer] = ((uint8_t *)&device->device_extension->time_switch_hold_last)[3]; index_buffer++;
    }

    return index_buffer;
}

uint8_t device_save_to_array_all(uint8_t *buffer)
{
     index_buffer_total = 0;

    for(index_device = 0; index_device < DEVICE_EXTENSION_ARRAY_SIZE; index_device++)
        index_buffer_total += device_save_to_array(&device_array[index_device], buffer + index_buffer_total);

    return index_buffer;
}
