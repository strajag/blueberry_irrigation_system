#ifndef DEVICE_H
#define DEVICE_H

#include "my_library.h"

#define DEVICE_RELAY_ON FALSE
#define DEVICE_RELAY_OFF !DEVICE_RELAY_ON

#define DEVICE_ARRAY_SIZE (15U)
#define DEVICE_EXTENSION_ARRAY_SIZE (1U)

typedef struct
{
    uint8_t pin;
    bool_t is_hold;
    uint8_t time_switch_hold_minimum; /* ms * 100 */
    uint32_t time_switch_hold_last; /* ms */
}Device_extension;

typedef struct
{
    uint8_t pin;
    bool_t is_active;
    uint8_t time_switch_minimum; /* ms * 100 */
    uint32_t time_switch_last; /* ms */
    Device_extension *device_extension;
}Device;

extern Device device_array[DEVICE_ARRAY_SIZE]; /* 0 generator 1 well 2 acid 3 supplements 4 micro 5 macro 6 click 7 micro 8 macro 9 water 10 group 1 11 group 2 12 group 3 13 garden 14 lamp */
extern Device_extension device_extension_array[DEVICE_EXTENSION_ARRAY_SIZE]; /* 0 click */
extern Device *device_extended_array[DEVICE_EXTENSION_ARRAY_SIZE]; /* 0 click */

extern void device_init(void);
extern void device_toggle(Device *device, bool_t is_active);
extern bool_t device_is_time_switch_minimum_passed(Device *device);
extern void device_update(void);

extern uint8_t device_save_to_array(Device *device, uint8_t *buffer);
extern uint8_t device_save_to_array_all(uint8_t *buffer);

#endif /* DEVICE_H */
