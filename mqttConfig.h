#ifndef HOMESECURITYIOT_MQTTCONFIG_H_
#define HOMESECURITYIOT_MQTTCONFIG_H_


#include <PubSubClient.h>


// MQTT parameters
IPAddress mqttServerAddr(192, 168, 1, 55);        // Pi eth0 interface                      
char* mqttClientId                                = "homesecurity";
int mqttPort                                      = 1883;
//#define MQTT_MAX_PACKET_SIZE                      168
//#define MQTT_KEEPALIVE                            300

boolean mqttClientConnected = false;

unsigned long lastReconnectAttempt                = 0UL;
const unsigned long RECONNECTION_ATTEMPT_INTERVAL = 30UL * 1000UL;  // attempt to reconnect every 30 seconds

const char COMMAND_SEPARATOR                      = ',';

char message[BUFFER_SIZE];


// Status topics

const char WIFLY_STATUS[]       PROGMEM = "homesecurity/status/wifly";
const char IP_ADDR_STATUS[]     PROGMEM = "homesecurity/status/ip_addr";
const char UPTIME_STATUS[]      PROGMEM = "homesecurity/status/uptime";
const char MEMORY_STATUS[]      PROGMEM = "homesecurity/status/memory";
const char SENSOR_STATUS[]      PROGMEM = "homesecurity/status/sensor";

PGM_P const STATUS_TOPICS[]     PROGMEM = { WIFLY_STATUS,        // idx = 0
                                            IP_ADDR_STATUS,      // idx = 1
                                            UPTIME_STATUS,       // idx = 2
                                            MEMORY_STATUS,       // idx = 3
                                            SENSOR_STATUS,       // idx = 4
                                           };

// MQTT payloads
const char MQTT_PAYLOAD_CONNECTED[]   PROGMEM = "CONNECTED";
const char MQTT_PAYLOAD_ERROR[]       PROGMEM = "ERROR";
const char MQTT_PAYLOAD_START[]       PROGMEM = "START";
const char MQTT_PAYLOAD_END[]         PROGMEM = "END";

PGM_P const MQTT_PAYLOADS[]           PROGMEM = { MQTT_PAYLOAD_CONNECTED,   // idx = 0
                                                  MQTT_PAYLOAD_ERROR,       // idx = 1
                                                  MQTT_PAYLOAD_START,       // idx = 2
                                                  MQTT_PAYLOAD_END,         // idx = 3
                                                };
                                                
// Control topics

const char SENSOR_CONTROL[]     PROGMEM = "homesecurity/control/sensor";

PGM_P const CONTROL_TOPICS[]    PROGMEM = { SENSOR_CONTROL,      // idx = 0
                                          };

#if USE_SDCARD
boolean readMqttConfiguration() {
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
    
    if (cfg.nameIs("mqttBrokerIP")) {
      // Dynamically allocate a copy of the string.
      char* str = cfg.copyValue();
      mqttServerAddr.fromString(str);
      DEBUG_LOG(1, "Read mqttBrokerIP: ");
      DEBUG_LOG(1, str);
    } else if (cfg.nameIs("mqttClientId")) { // mqttClientId string (char *)
      // Dynamically allocate a copy of the string.
      mqttClientId = cfg.copyValue();
      DEBUG_LOG(1, "Read mqttClientId: ");
      DEBUG_LOG(1, mqttClientId);
    } else if (cfg.nameIs("mqttPort")) { // mqttPort integer
      mqttPort = cfg.getIntValue();
      DEBUG_LOG(1, "Read mqttPort: ");
      DEBUG_LOG(1, mqttPort);
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

// callback function definition
void callback(char* topic, uint8_t* payload, unsigned int length);

PubSubClient   mqttClient(mqttServerAddr, mqttPort, callback, networkClient);

void publish_connected()
{
  messBuffer[0] = '\0';
  strcpy_P(messBuffer, (char*)pgm_read_word(&(MQTT_PAYLOADS[0])));
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[0])));
  mqttClient.publish(progBuffer, messBuffer);
}

void publish_ip_address()
{
//  const byte IP_ADDRESS_BUFFER_SIZE = 16; // "255.255.255.255\0"
//  static char ipString[IP_ADDRESS_BUFFER_SIZE] = "";
  messBuffer[0] = '\0';
  strcpy(messBuffer, WiFly.ip());
//  sprintf(ipString, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[1])));
  mqttClient.publish(progBuffer, messBuffer);
}

void publish_uptime()
{
  messBuffer[0] = '\0';
  ltoa(millis(), messBuffer, 10);
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[2])));
  mqttClient.publish(progBuffer, messBuffer);
}

void publish_sensor_state(byte idx, byte state)
{  
  // create message in format "idx,ON"
  messBuffer[0] = '\0';
  sprintf(messBuffer, "%i%c%i", idx + 1, COMMAND_SEPARATOR, state);
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[4])));
  DEBUG_LOG(1, "progBuffer: ");
  DEBUG_LOG(1, progBuffer);
  mqttClient.publish(progBuffer, messBuffer);
}


#endif   /* HOMESECURITYIOT_MQTTCONFIG_H_ */
