#include "sensor_pressure.h"

Sensor_pressure sensor_pressure_array[SENSOR_PRESSURE_ARRAY_SIZE];

static uint8_t counter;
static uint32_t time_update_last;
static uint16_t raw_value;

void sensor_pressure_init(void)
{
    debug_log(("sensor_pressure_init()\n"));

    /* flow pressure on well */
    sensor_pressure_array[0].pin = SENSOR_PRESSURE_PIN_1;
    sensor_pressure_array[0].pressure = 0;
    pinMode(sensor_pressure_array[0].pin, INPUT);

    /* flow pressure on well */
    sensor_pressure_array[1].pin = SENSOR_PRESSURE_PIN_2;
    sensor_pressure_array[1].pressure = 0;
    pinMode(sensor_pressure_array[1].pin, INPUT);

    /* flow pressure on well */
    sensor_pressure_array[2].pin = SENSOR_PRESSURE_PIN_3;
    sensor_pressure_array[2].pressure = 0;
    pinMode(sensor_pressure_array[2].pin, INPUT);

    debug_log(("sensor_pressure_init(): success\n"));
}

void sensor_pressure_update(void)
{
    if(millis() - time_update_last < SENSOR_PRESSURE_UPDATE_INTERVAL)
        return;

    for(counter = 0; counter < SENSOR_PRESSURE_ARRAY_SIZE; counter++)
    {
        raw_value = analogRead(sensor_pressure_array[counter].pin);
        raw_value = analogRead(sensor_pressure_array[counter].pin);
        raw_value = constrain((int16_t)raw_value, SENSOR_PRESSURE_VALUE_RAW_MINIMUM, SENSOR_PRESSURE_VALUE_RAW_MAXIMUM);
        raw_value = map(raw_value, SENSOR_PRESSURE_VALUE_RAW_MINIMUM, SENSOR_PRESSURE_VALUE_RAW_MAXIMUM, SENSOR_PRESSURE_VALUE_MINIMUM, SENSOR_PRESSURE_VALUE_MAXIMUM);
        sensor_pressure_array[counter].pressure = raw_value;
    }

    time_update_last = millis();
}
