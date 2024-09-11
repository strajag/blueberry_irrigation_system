#include "pins.h"
#include "device.h"

static uint8_t counter;

void control_safety_init(void)
{
    debug_log(("control_safety_init()\n"));

    pinMode(ARDUINO_PIN_RESET, INPUT_PULLUP);
    pinMode(ARDUINO_PIN_RESET, OUTPUT);
    digitalWrite(ARDUINO_PIN_RESET, HIGH);

    debug_log(("control_safety_init(): success\n"));
}

void control_safety_devices_stop(void)
{
    for(counter = DEVICE_ARRAY_SIZE; counter-- > 0; )
        device_toggle(&device_array[counter], FALSE);
}

void control_safety_reset(void)
{
    digitalWrite(ARDUINO_PIN_RESET, LOW);
}

void control_safety_update(void)
{

}
