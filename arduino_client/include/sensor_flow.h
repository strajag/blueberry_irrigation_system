#ifndef SENSOR_FLOW_H
#define SENSOR_FLOW_H

#include "my_library.h"
#include "pins.h"

#define SENSOR_FLOW_ARRAY_SIZE (5U)
#define SENSOR_FLOW_UPDATE_INTERVAL (1000UL) /* ms */

typedef struct
{
    uint8_t pin_digital;
    uint8_t pin_interrupt;
    void (*func_interrupt)(void);
    uint16_t pulses_per_liter;
    volatile uint16_t pulses_interrupt;
    uint32_t pulses_total;
}Sensor_flow;

extern Sensor_flow sensor_flow_array[SENSOR_FLOW_ARRAY_SIZE]; /* 0 well 1 micro 2 macro 3 water 4 click */

extern void sensor_flow_init(void);
extern void sensor_flow_pulses_total_set(Sensor_flow *sensor_flow, uint32_t pulses_total);
extern void sensor_flow_pulses_per_liter_set(Sensor_flow *sensor_flow, uint16_t pulses_per_liter);
extern void sensor_flow_update(void);

extern void sensor_flow_interrupt_0(void);
extern void sensor_flow_interrupt_1(void);
extern void sensor_flow_interrupt_2(void);
extern void sensor_flow_interrupt_3(void);
extern void sensor_flow_interrupt_4(void);

#endif /* SENSOR_FLOW_H */
