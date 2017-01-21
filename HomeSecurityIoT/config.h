#ifndef HOMESECURITYIOT_CONFIG_H_
#define HOMESECURITYIOT_CONFIG_H_


#include "debug.h"

// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// global variable definitions
#if DEBUG_LEVEL > 0
const int BAUD_RATE               = 9600;
#endif

const byte BUFFER_SIZE            = 60;
char topicBuffer[BUFFER_SIZE];
char payloadBuffer[BUFFER_SIZE];

const unsigned long STATUS_UPDATE_INTERVAL = 5UL * 60UL * 1000UL; // 5 minutes
unsigned long statusPreviousMillis = 0UL;

// internal libraries
#include "ethernetConfig.h"
#include "mqttConfig.h"
#include "securityShieldConfig.h"

void no_network_behaviour() {}


#endif   /* HOMESECURITYIOT_CONFIG_H_ */
