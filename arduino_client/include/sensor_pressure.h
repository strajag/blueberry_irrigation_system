#ifndef SENSOR_PRESSURE_H
#define SENSOR_PRESSURE_H

#include "my_library.h"
#include "pins.h"

#define SENSOR_PRESSURE_ARRAY_SIZE (3U)
#define SENSOR_PRESSURE_UPDATE_INTERVAL (1000UL) /* ms */
#define SENSOR_PRESSURE_VALUE_RAW_MINIMUM (0)
#define SENSOR_PRESSURE_VALUE_RAW_MAXIMUM (1023)
#define SENSOR_PRESSURE_VALUE_MINIMUM (0)
#define SENSOR_PRESSURE_VALUE_MAXIMUM (120)

typedef struct
{
    uint8_t pin;
    uint8_t pressure;
}Sensor_pressure;

extern Sensor_pressure sensor_pressure_array[SENSOR_PRESSURE_ARRAY_SIZE]; /* 0 well 1 before filters 2 after filters */

extern void sensor_pressure_init(void);
extern void sensor_pressure_update(void);

#endif /* SENSOR_PRESSURE_H */
