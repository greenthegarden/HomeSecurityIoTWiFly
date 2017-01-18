#ifndef HOMESECURITYIOT_CONFIG_H_
#define HOMESECURITYIOT_CONFIG_H_


#include "debug.h"

// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// global variable definitions
const int BAUD_RATE               = 9600;

const byte BUFFER_SIZE            = 60;
char topicBuffer[BUFFER_SIZE];
char payloadBuffer[BUFFER_SIZE];

#include "ethernetConfig.h"
#include "mqttConfig.h"
#include "securityShieldConfig.h"

void no_network_behaviour() {
  check_sensors();
//  if(checkSensor(SENSOR_INPUTS[0], SENSOR_LEDS[0]) != NORMAL_STATE) {
//    repeatTone();
//  }
}


#endif   /* HOMESECURITYIOT_CONFIG_H_ */
