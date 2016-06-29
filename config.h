#ifndef HOMESECURITYIOT_CONFIG_H_
#define HOMESECURITYIOT_CONFIG_H_


#include "debug.h"

// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


const byte BUFFER_SIZE            = 60;
//char progBuffer[BUFFER_SIZE];
char topicBuffer[BUFFER_SIZE];
char messageBuffer[BUFFER_SIZE];

const int FLOAT_DECIMAL_PLACES    = 1;

const int BAUD_RATE               = 9600;

#define USE_WIFLY                   true
#define USE_ETHERNET                false
//#define USE_SDCARD                  true
#define USE_OLED_SHIELD             false

#define USE_SDCARD_MQTT_CONFIG      false
#define USE_SDCARD_ETHERNET_CONFIG  false

#if (USE_SDCARD_MQTT_CONFIG || USE_SDCARD_ETHERNET_CONFIG)
#include "sdConfig.h"
#endif

#if USE_WIFLY
#include "wiFlyConfig.h"
#define USE_MQTT true
#elif USE_ETHERNET
#include "ethernetConfig.h"
#define USE_MQTT true
#else
#define USE_MQTT false
#endif

// include MQTT configuration if using a networked system
#if USE_MQTT
#include "mqttConfig.h"
#endif

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

