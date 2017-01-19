#ifndef HOMESECURITYIOT_ETHERNETCONFIG_H_
#define HOMESECURITYIOT_ETHERNETCONFIG_H_


// Ethernet libraries
#include <SPI.h>

#ifndef ETHERNET_SHIELD_VERSION
#define ETHERNET_SHIELD_VERSION 2
#endif

#if ETHERNET_SHIELD_VERSION == 1
#include <Ethernet.h>
#elif ETHERNET_SHIELD_VERSION == 2
#include <Ethernet2.h>
#endif

// Update these with values suitable for your network.
byte mac[] = {0x90, 0xA2, 0xDA,
              0x0F, 0xFC, 0xA9}; // value for Arduino Ethernet Shield 2

EthernetClient networkClient;

const unsigned long NETWORK_STARTUP_DELAY = 1500UL;

byte ethernet_init() {
  delay(NETWORK_STARTUP_DELAY);

  // Configure Ethernet
  if (Ethernet.begin(mac) == 0) {
    DEBUG_LOG(1, "Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    return 0;
  }

  DEBUG_LOG(1, "IP:");
  DEBUG_LOG(1, Ethernet.localIP());

  return 1;
}


#endif   /* HOMESECURITYIOT_ETHERNETCONFIG_H_ */
