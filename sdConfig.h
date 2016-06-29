#ifndef HOMESECURITYIOT_SDCONFIG_H_
#define HOMESECURITYIOT_SDCONFIG_H_


#include <SPI.h>
#include <SD.h>
#include <SDConfigFile.h>


const byte SDCARD_CS_PIN = 4;  // define SD Card chip select pin

boolean use_default_settings = true;

boolean sdCardSetup()
{
  // configure chip select pins for SD card and Ethernet
  pinMode(SDCARD_CS_PIN, OUTPUT);      // set SD card chip select as output:
  // Setup the SD card
  DEBUG_LOG(1, "Calling SD.begin() ...");
  if (!SD.begin(SDCARD_CS_PIN)) {
    DEBUG_LOG(1, "SD.begin() failed.");
    DEBUG_LOG(1, "Will use default settings");
    return 0;
  }
  DEBUG_LOG(1, "... succeeded.");
  use_default_settings = false;
  return 1;
}




#endif   /* HOMESECURITYIOT_SDCONFIG_H_ */

