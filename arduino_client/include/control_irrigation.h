#ifndef CONTROL_IRRIGATION_H
#define CONTROL_IRRIGATION_H

#include "my_library.h"

extern void control_irrigation_init(void);
extern void control_irrigation_start(void); /* volume_to_deliver ml, time_to_deliver s */
extern void control_irrigation_update(void);

#endif /* CONTROL_IRRIGATION_H */
