#ifndef CONTROL_FLOW_H
#define CONTROL_FLOW_H

#include "my_library.h"
#include "device.h"
#include "sensor_flow.h"

#define CONTROL_FLOW_FAULTY_DETECTION_MINIMUM_PULSES (100LU)
#define CONTROL_FLOW_FAULTY_DETECTION_MAXIMUM_PULSES (1000LU)
#define CONTROL_FLOW_FAULTY_DETECTION_TIME_INTERVAL (10000LU) /* ms */

#define CONTROL_FLOW_ARRAY_SIZE (6U)

typedef enum
{
    CONTROL_FLOW_FAULTY_DETECTION_STATUS_NOT_DETECTED,
    CONTROL_FLOW_FAULTY_DETECTION_STATUS_STOPPED_BY_USER,
    CONTROL_FLOW_FAULTY_DETECTION_STATUS_DETECTED_FLOW_LESS,
    CONTROL_FLOW_FAULTY_DETECTION_STATUS_DETECTED_FLOW_MORE,
    CONTROL_FLOW_FAULTY_DETECTION_STATUS_DETECTED_PRESSURE_ZERO
}Control_flow_faulty_detection_status;

typedef struct
{
    bool_t is_active;
    bool_t is_pause;
    Device *device;
    Sensor_flow *sensor_flow;
    uint16_t time_to_deliver; /* s */
    uint32_t time_start; /* ms */
    uint32_t time_pause; /* ms */
    uint32_t pulses_to_deliver;
    uint32_t pulses_last;
}Control_flow;

extern Control_flow control_flow_array[CONTROL_FLOW_ARRAY_SIZE]; /* 0 well 1 micro 2 macro 3 water 4 click 5 garden */
extern Control_flow_faulty_detection_status control_flow_faulty_detection_status;
extern uint32_t control_flow_faulty_detection_time_last;

extern void control_flow_init(void);
extern void control_flow_start(Control_flow *control_flow, uint16_t volume_to_deliver, uint16_t time_to_deliver); /* volume_to_deliver ml, time_to_deliver s */
extern void control_flow_stop(Control_flow *control_flow);
extern void control_flow_pause(Control_flow *control_flow, bool_t is_pause);
extern void control_flow_update(void);

#endif /* CONTROL_FLOW_H */
