#ifndef HOMESECURITYIOT_ETHERNETCONFIG_H_
#define HOMESECURITYIOT_ETHERNETCONFIG_H_


// Ethernet libraries
#include <SPI.h>
#include <Ethernet.h>


const unsigned long NETWORK_STARTUP_DELAY = 5000UL;

const byte          ETHERNET_CS_PIN       = 10;


byte mac[]                                = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };

IPAddress ip(192, 168, 1, 90);

EthernetClient networkClient;


#if USE_SDCARD_ETHERNET_CONFIG

const char CONFIG_FILE[] = "ethernet.cfg";

/* FILE FORMAT:
 * <MQTT Broker ip address> (mqttServerAddr)
 * <MQTT Client Id>         (mqttClientId)
 * <MQTT Port>              (mqttPort)
 */
  
boolean readSdCardEthernetConfiguration() {
  /*
   * Length of the longest line expected in the config file.
   * The larger this number, the more memory is used
   * to read the file.
   * You probably won't need to change this number.
   */
  const uint8_t CONFIG_LINE_LENGTH = 32;
  
  // The open configuration file.
  SDConfigFile cfg;
  
  // Open the configuration file.
  if (!cfg.begin(CONFIG_FILE, CONFIG_LINE_LENGTH)) {
    DEBUG_LOG(1, "Failed to open configuration file: ");
    DEBUG_LOG(1, CONFIG_FILE);
    return false;
  }
  
  // Read each setting from the file.
  while (cfg.readNextSetting()) {
    
    // Put a nameIs() block here for each setting you have.
    
    if (cfg.nameIs("ipAddrStr")) { // ipAddr string (char *)
      // Dynamically allocate a copy of the string.
      char* str = cfg.copyValue();
      DEBUG_LOG(1, "Read ipAddrStr: ");
      DEBUG_LOG(1, str);
      ip.fromString(str);
    } else {
      // report unrecognized names.
      DEBUG_LOG(1, "Unknown name in config: ");
      DEBUG_LOG(1, cfg.getName());
    }
  }
  
  // clean up
  cfg.end();
}
#endif



//void readFile()
//{
//  File file;
//
//  file = SD.open(CONFIG_FILE);
//  if (file) {
//    // array for value data:
//    while (file.available()) {       // read the settings file:
//      byte i = 0;
//      boolean paramEnd = false;
//
//      while (!paramEnd) {        // while not at the end of the parameter keep reading:
//        chtr = file.read();
//        if (chtr != ':') {        // if the char from file is not a':' read it in:
//          name[i] = chtr;
//          i++;
//        } else {
//          paramEnd = true;                 // otherwise we have come to the end of our parmeter name:
//        }
//      }
//      byte j = 0;
//      boolean valueEnd = false;
//      value[0] = '\0';                               // reset our array pointer:
//      while (!valueEnd) {
//        chtr = file.read();
//        if (chtr != ',') {     // if the char from file is not a ',' read it in:
//          value[j] = chtr;
//          j++;
//        } else {
//          valueEnd = true;   // otherwise we have come to the end of our value:
//        }
//      }
//
//      if (!strcmp(name, "ip0")) {
//        ip[0] = atoi(value);          // assign value to ip0 first ip octet:
//      }
//      if (!strcmp(name, "ip1")) {
//        ip[1] = atoi(value);          // assign value to ip1 first ip octet:
//      }
//      if (!strcmp(name, "ip2")) {
//        ip[2] = atoi(value);          // assign value to ip2 first ip octet:
//      }
//      if (!strcmp(name, "ip3")) {
//        ip[3] = atoi(value);          // assign value to ip4 first ip octet:
//      }
//    }
//  }
//}


// file in form
// <ip address>
// <DEBUG Level>            (DEBUG_LEVEL)

//void GetNetworkSettingsFromFile(char* fileName) 
//{  
//  byte b = 0;
//  String settings[5];
//  if (SD.exists(fileName) == false)
//  {
//    Serial.println("File not found");
//    return;
//  }
//  File myFile = SD.open(fileName, FILE_READ);
//  if (!myFile)
//  {
//    Serial.println("Cannot open file");
//    return;
//  }
//  myFile.seek(0);
//  while (myFile.available())
//  {
//    settings = myFile.readStringUntil('\n');
//    b++;
//  }  
//  myFile.close();
//  Serial.print("my MAC: ");
//  Serial.println(settings[0]);
//  Serial.print("my IP: ");
//  Serial.println(settings[1]);
//  Serial.print("my MASK: ");
//  Serial.println(settings[2]);
//  Serial.print("my GATEWAY: ");
//  Serial.println(settings[3]);
//  Serial.print("my DNS: ");
//  Serial.println(settings[4]);
//}


#endif   /* HOMESECURITYIOT_ETHERNETCONFIG_H_ */

