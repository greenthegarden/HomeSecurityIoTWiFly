#ifndef HOMESECURITYIOT_CONFIG_H_
#define HOMESECURITYIOT_CONFIG_H_


#include "debug.h"

// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// global variable definitions
const int BAUD_RATE               = 9600;

const byte BUFFER_SIZE            = 60;
char topicBuffer[BUFFER_SIZE];
char messageBuffer[BUFFER_SIZE];

#define USE_WIFLY                   true
#define USE_ETHERNET                false

#if USE_WIFLY
  #include "wiFlyConfig.h"
  #define USE_MQTT true
#elif USE_ETHERNET
  #include "ethernetConfig.h"
  #define USE_MQTT                  true
#else
  #define USE_MQTT                  false
#endif

#define USE_SD                      false

#if (USE_MQTT && USE_SD)
  #include "sdConfig.h"
#endif

// include MQTT configuration if using a networked system
#if USE_MQTT
  #include "mqttConfig.h"
#endif

#define USE_OLED_SHIELD             false

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

