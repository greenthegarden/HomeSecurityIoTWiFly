#ifndef HOMESECURITYIOT_ETHERNETCONFIG_H_
#define HOMESECURITYIOT_ETHERNETCONFIG_H_


#include <SPI.h>
#include <Ethernet.h>


const unsigned long NETWORK_STARTUP_DELAY = 5000UL;

const byte          ETHERNET_CS_PIN       = 10;

// the media access control (ethernet hardware) address for the shield:
byte mac[]                                = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient      networkClient;

byte ethernet_init()
{
  pinMode(ETHERNET_CS_PIN, OUTPUT);    // set Ethernet chip select as output:

  // Configure Ethernet
  delay(NETWORK_STARTUP_DELAY); // allow some time for Ethernet processor to come out of reset on Arduino power up:

  if (Ethernet.begin(mac) == 0) {
    DEBUG_LOG(1, "IP failed!!");
    return 0;
  }
  DEBUG_LOG(1, Ethernet.localIP());
//    delay(NETWORK_STARTUP_DELAY);
  return 1;
}



#endif   /* HOMESECURITYIOT_ETHERNETCONFIG_H_ */
