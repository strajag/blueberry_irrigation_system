#include "my_library.h"
#include "avr/wdt.h"
#include "device.h"
#include "sensor_flow.h"
#include "control_flow.h"
#include "control_irrigation.h"
#include "control_irrigation_temp.h"
#include "internet.h"
#include "interface.h"
#include "control_safety.h"
#include "sensor_pressure.h"

extern void led_switch(void);

void debug_printf_definition(char *string) { Serial.print(string); }

static bool_t led_is_active;

void setup(void)
{
    wdt_disable();
    delay(3000);
    wdt_enable(WDTO_8S);

    pinMode(13, OUTPUT);

    device_init();
    sensor_flow_init();
    sensor_pressure_init();
    control_flow_init();
    control_irrigation_init();
    control_irrigation_temp_init();
    internet_init();
    interface_init();
    control_safety_init();

    led_switch();
}

void loop(void)
{
    device_update();
    led_switch();
    sensor_flow_update();
    led_switch();
    sensor_pressure_update();
    led_switch();
    control_flow_update();
    led_switch();
    control_irrigation_update();
    led_switch();
    control_irrigation_temp_update();
    led_switch();
    internet_update();
    led_switch();
    interface_update();
    led_switch();
    control_safety_update();
    led_switch();
    wdt_reset();
}

void led_switch(void)
{
    led_is_active = !led_is_active;
    digitalWrite(13, led_is_active);
}
