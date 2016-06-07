#ifndef HOMESECURITYIOT_CONFIG_H_
#define HOMESECURITYIOT_CONFIG_H_


#include "debug.h"

// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


const byte BUFFER_SIZE            = 32;
char progBuffer[BUFFER_SIZE];
char messBuffer[BUFFER_SIZE];

const int FLOAT_DECIMAL_PLACES    = 1;

const int BAUD_RATE               = 9600;


#define USE_SDCARD false
#define USE_OLED_SHIELD false


#if USE_SDCARD
#include "sdConfig.h"
#endif

#include "wiFlyConfig.h"
//#include "ethernetConfig.h"
#include "mqttConfig.h"

#if USE_OLED_SHIELD
#include "oledShieldConfig.h"
#endif
#include "securityShieldConfig.h"



void no_network_behaviour() {
  check_sensors();
//  if(checkSensor(SENSOR_INPUTS[0], SENSOR_LEDS[0]) != NORMAL_STATE) {
//    repeatTone();
//  }
}


#endif   /* HOMESECURITYIOT_CONFIG_H_ */

