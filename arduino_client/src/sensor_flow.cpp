#include "sensor_flow.h"

Sensor_flow sensor_flow_array[SENSOR_FLOW_ARRAY_SIZE];

static uint8_t counter;
static uint32_t time_update_last;

void sensor_flow_init(void)
{
    debug_log(("sensor_flow_init()\n"));

    /* flow sensor on pump well */
    sensor_flow_array[0].pin_digital = SENSOR_FLOW_PIN_PUMP_WELL;
    sensor_flow_array[0].pin_interrupt = digitalPinToInterrupt(sensor_flow_array[0].pin_digital);
    sensor_flow_array[0].func_interrupt = sensor_flow_interrupt_0;
    sensor_flow_array[0].pulses_per_liter = 5880;
    sensor_flow_array[0].pulses_interrupt = 0;
    sensor_flow_array[0].pulses_total = 0;
    pinMode(sensor_flow_array[0].pin_digital, INPUT_PULLUP);
    attachInterrupt(sensor_flow_array[0].pin_interrupt, sensor_flow_array[0].func_interrupt, FALLING);

    /* flow sensor on pump supplements micro */
    sensor_flow_array[1].pin_digital = SENSOR_FLOW_PIN_PUMP_SUPPLEMENTS_MICRO;
    sensor_flow_array[1].pin_interrupt = digitalPinToInterrupt(sensor_flow_array[1].pin_digital);
    sensor_flow_array[1].func_interrupt = sensor_flow_interrupt_1;
    sensor_flow_array[1].pulses_per_liter = 5715;
    sensor_flow_array[1].pulses_interrupt = 0;
    sensor_flow_array[1].pulses_total = 0;
    pinMode(sensor_flow_array[1].pin_digital, INPUT_PULLUP);
    attachInterrupt(sensor_flow_array[1].pin_interrupt, sensor_flow_array[1].func_interrupt, FALLING);

    /* flow sensor on pump supplements macro */
    sensor_flow_array[2].pin_digital = SENSOR_FLOW_PIN_PUMP_SUPPLEMENTS_MACRO;
    sensor_flow_array[2].pin_interrupt = digitalPinToInterrupt(sensor_flow_array[2].pin_digital);
    sensor_flow_array[2].func_interrupt = sensor_flow_interrupt_2;
    sensor_flow_array[2].pulses_per_liter = 5715;
    sensor_flow_array[2].pulses_interrupt = 0;
    sensor_flow_array[2].pulses_total = 0;
    pinMode(sensor_flow_array[2].pin_digital, INPUT_PULLUP);
    attachInterrupt(sensor_flow_array[2].pin_interrupt, sensor_flow_array[2].func_interrupt, FALLING);

    /* flow sensor on pump supplements water */
    sensor_flow_array[3].pin_digital = SENSOR_FLOW_PIN_PUMP_SUPPLEMENTS_WATER;
    sensor_flow_array[3].pin_interrupt = digitalPinToInterrupt(sensor_flow_array[3].pin_digital);
    sensor_flow_array[3].func_interrupt = sensor_flow_interrupt_3;
    sensor_flow_array[3].pulses_per_liter = 5880;
    sensor_flow_array[3].pulses_interrupt = 0;
    sensor_flow_array[3].pulses_total = 0;
    pinMode(sensor_flow_array[3].pin_digital, INPUT_PULLUP);
    attachInterrupt(sensor_flow_array[3].pin_interrupt, sensor_flow_array[3].func_interrupt, FALLING);

    /* flow sensor on pump click */
    sensor_flow_array[4].pin_digital = SENSOR_FLOW_PIN_PUMP_CLICK;
    sensor_flow_array[4].pin_interrupt = digitalPinToInterrupt(sensor_flow_array[4].pin_digital);
    sensor_flow_array[4].func_interrupt = sensor_flow_interrupt_4;
    sensor_flow_array[4].pulses_per_liter = 5880;
    sensor_flow_array[4].pulses_interrupt = 0;
    sensor_flow_array[4].pulses_total = 0;
    pinMode(sensor_flow_array[4].pin_digital, INPUT_PULLUP);
    attachInterrupt(sensor_flow_array[4].pin_interrupt, sensor_flow_array[4].func_interrupt, FALLING);

    debug_log(("sensor_flow_init(): success\n"));
}

void sensor_flow_pulses_total_set(Sensor_flow *sensor_flow, uint32_t pulses_total)
{
    debug_log(("sensor_flow_pulses_total_set(%d, %d);\n", sensor_flow->pin_digital, pulses_total));
    sensor_flow->pulses_total = pulses_total;
    debug_log(("sensor_flow_pulses_total_set(): success\n"));
}

void sensor_flow_pulses_per_liter_set(Sensor_flow *sensor_flow, uint16_t pulses_per_liter)
{
    debug_log(("sensor_flow_pulses_per_liter_set(%d, %d);\n", sensor_flow->pin_digital, pulses_per_liter));
    sensor_flow->pulses_per_liter = pulses_per_liter;
    debug_log(("sensor_flow_pulses_per_liter_set(): success\n"));
}

void sensor_flow_update(void)
{
    if(millis() - time_update_last < SENSOR_FLOW_UPDATE_INTERVAL)
        return;

    for(counter = 0; counter < SENSOR_FLOW_ARRAY_SIZE; counter++)
    {
        detachInterrupt(sensor_flow_array[counter].pin_interrupt);
        sensor_flow_array[counter].pulses_total += sensor_flow_array[counter].pulses_interrupt;
        sensor_flow_array[counter].pulses_interrupt = 0;
        attachInterrupt(sensor_flow_array[counter].pin_interrupt, sensor_flow_array[counter].func_interrupt, FALLING);
    }

    time_update_last = millis();
}

void sensor_flow_interrupt_0(void)
{
    sensor_flow_array[0].pulses_interrupt++;
}

void sensor_flow_interrupt_1(void)
{
    sensor_flow_array[1].pulses_interrupt++;
}

void sensor_flow_interrupt_2(void)
{
    sensor_flow_array[2].pulses_interrupt++;
}

void sensor_flow_interrupt_3(void)
{
    sensor_flow_array[3].pulses_interrupt++;
}

void sensor_flow_interrupt_4(void)
{
    sensor_flow_array[4].pulses_interrupt++;
}
