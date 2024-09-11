#ifndef PINS_H
#define PINS_H

#include "Arduino.h"

/*                  MEGA    NANO
 * I2C
 * SDA              D20     D18/A4
 * SCL              D21     D19/A5
 *
 * SPI/ICSP
 * MISO/CIPO/SDI    D50     D12
 * MOSI/COPI/SDO    D51     D11
 * SCK              D52     D13
 * CS/SS            D53     D10
 */

#define DEVICE_PIN_GENERATOR                    (25)
#define DEVICE_PIN_PUMP_WELL                    (24)
#define DEVICE_PIN_PUMP_ACID                    (23)
#define DEVICE_PIN_PUMP_SUPPLEMENTS             (22)
#define DEVICE_PIN_PUMP_SUPPLEMENTS_MICRO       (29)
#define DEVICE_PIN_PUMP_SUPPLEMENTS_MACRO       (28)
#define DEVICE_PIN_PUMP_CLICK_ON                (27)
#define DEVICE_PIN_PUMP_CLICK_OFF               (26)
#define DEVICE_PIN_VALVE_SUPPLEMENTS_MICRO      (32)
#define DEVICE_PIN_VALVE_SUPPLEMENTS_MACRO      (30)
#define DEVICE_PIN_VALVE_SUPPLEMENTS_WATER      (31)
#define DEVICE_PIN_VALVE_GROUP_1                (37)
#define DEVICE_PIN_VALVE_GROUP_2                (36)
#define DEVICE_PIN_VALVE_GROUP_3                (35)
#define DEVICE_PIN_VALVE_GARDEN                 (34)
#define DEVICE_PIN_LAMP                         (33)

#define SENSOR_FLOW_PIN_PUMP_WELL               (19)
#define SENSOR_FLOW_PIN_PUMP_SUPPLEMENTS_MICRO  (3)
#define SENSOR_FLOW_PIN_PUMP_SUPPLEMENTS_MACRO  (2)
#define SENSOR_FLOW_PIN_PUMP_SUPPLEMENTS_WATER  (18)
#define SENSOR_FLOW_PIN_PUMP_CLICK              (20)

#define SENSOR_PRESSURE_PIN_1                   (54)
#define SENSOR_PRESSURE_PIN_2                   (55)
#define SENSOR_PRESSURE_PIN_3                   (56)

#define INTERNET_PIN_RESET                      (49)
#define INTERNET_PIN_SS                         (53)

#define ARDUINO_PIN_RESET                       (48)

#endif /* PINS_H */
