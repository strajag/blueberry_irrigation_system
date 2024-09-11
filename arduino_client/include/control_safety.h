#ifndef CONTROL_SAFETY_H
#define CONTROL_SAFETY_H

#include "my_library.h"

#define CONTROL_SAFETY_SENSOR_FLOW_PULSES_DIFFERENCE_MAXIMUM (50LU)
#define CONTROL_SAFETY_DEVICE_TIME_SWITCH_PASS_MINIMUM (2000LU) /* ms */
#define CONTROL_SAFETY_TIME_UPDATE_INTERVAL (2000LU) /* ms */

extern void control_safety_init(void);
extern void control_safety_devices_stop(void);
extern void control_safety_reset(void);
extern void control_safety_update(void);

#endif /* CONTROL_SAFETY_H */
