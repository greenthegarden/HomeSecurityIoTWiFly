#ifndef HOMESECURITYIOT_ETHERNETCONFIG_H_
#define HOMESECURITYIOT_ETHERNETCONFIG_H_


// Use NetEEPROM library read MAC address from EEPROM
#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
#include <NetEEPROM.h>


const unsigned long NETWORK_STARTUP_DELAY = 5000UL;

const byte          ETHERNET_CS_PIN       = 10;


//byte mac[]                                = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };

//IPAddress           ip(192, 168, 1, 90);

EthernetClient      networkClient;

void ethernet_init()
{
  pinMode(ETHERNET_CS_PIN, OUTPUT);    // set Ethernet chip select as output:
}


#endif   /* HOMESECURITYIOT_ETHERNETCONFIG_H_ */
