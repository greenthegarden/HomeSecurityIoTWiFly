#ifndef HOMESECURITYIOT_ETHERNETCONFIG_H_
#define HOMESECURITYIOT_ETHERNETCONFIG_H_


#include <SPI.h>
#include <Ethernet.h>


const unsigned long NETWORK_STARTUP_DELAY = 5000UL;

const byte          ETHERNET_CS_PIN       = 10;

// the media access control (ethernet hardware) address for the shield:
byte mac[]                                = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient      networkClient;

void ethernet_init() {
  pinMode(ETHERNET_CS_PIN, OUTPUT);    // set Ethernet chip select as output:
}


#endif   /* HOMESECURITYIOT_ETHERNETCONFIG_H_ */
