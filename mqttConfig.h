#ifndef HOMESECURITYIOT_MQTTCONFIG_H_
#define HOMESECURITYIOT_MQTTCONFIG_H_


#include <PubSubClient.h>


// MQTT parameters
IPAddress mqttServerAddr(192, 168, 1, 50);        // openHAB server
char* mqttClientId                                = "homesecurity";
int mqttPort                                      = 1883;
//#define MQTT_MAX_PACKET_SIZE                      168
//#define MQTT_KEEPALIVE                            300

boolean mqttClientConnected                       = false;

unsigned long lastReconnectAttempt                = 0UL;
const unsigned long RECONNECTION_ATTEMPT_INTERVAL = 30UL * 1000UL;  // attempt to reconnect every 30 seconds

const char COMMAND_SEPARATOR                      = ',';


// Status topics

const char WIFLY_STATUS[]       PROGMEM = "homesecurity/interior/status/wifly";
const char IP_ADDR_STATUS[]     PROGMEM = "homesecurity/interior/status/ip_addr";   // 36 chars
const char UPTIME_STATUS[]      PROGMEM = "homesecurity/interior/status/uptime";
const char MEMORY_STATUS[]      PROGMEM = "homesecurity/interior/status/memory";
const char SENSOR_STATUS[]      PROGMEM = "homesecurity/interior/status/sensor";

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

const char SENSOR_CONTROL[]     PROGMEM = "homesecurity/interior/control/sensor";

PGM_P const CONTROL_TOPICS[]    PROGMEM = { SENSOR_CONTROL,      // idx = 0
                                          };

#if USE_SDCARD_MQTT_CONFIG

const char CONFIG_FILE[] = "mqtt.cfg";

/* FILE FORMAT:
 * <MQTT Broker ip address> (mqttServerAddr)
 * <MQTT Client Id>         (mqttClientId)
 * <MQTT Port>              (mqttPort)
 */

boolean readSdCardMqttConfiguration() {
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
    // Put a nameIs() block here for each setting required
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

PubSubClient mqttClient(mqttServerAddr, mqttPort, callback, networkClient);

void publish_connected()
{
  messageBuffer[0] = '\0';
  strcpy_P(messageBuffer, (char*)pgm_read_word(&(MQTT_PAYLOADS[0])));
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[0])));
  mqttClient.publish(topicBuffer, messageBuffer);
}

void publish_ip_address()
{
  messageBuffer[0] = '\0';
#if USE_WIFLY  
  strcpy(messageBuffer, WiFly.ip());
#elif USE_ETHERNET
  const byte IP_ADDRESS_BUFFER_SIZE = 16; // "255.255.255.255\0"
  static char ipString[IP_ADDRESS_BUFFER_SIZE] = "";
  sprintf(ipString, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  strcpy(messageBuffer, ipString);
#else
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(MQTT_PAYLOADS[1])));
#endif
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[1])));
  mqttClient.publish(topicBuffer, messageBuffer);
}

void publish_uptime()
{
  messageBuffer[0] = '\0';
  ltoa(millis(), messageBuffer, 10);
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[2])));
  mqttClient.publish(topicBuffer, messageBuffer);
}

//void publish_sensor_state(byte idx, byte state)
//{  
//  // create message in format "idx,state"
//  messageBuffer[0] = '\0';
//  sprintf(messageBuffer, "%i%c%i", idx + 1, COMMAND_SEPARATOR, state);
//  topicBuffer[0] = '\0';
//  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[4])));
//  DEBUG_LOG(1, "topicBuffer: ");
//  DEBUG_LOG(1, topicBuffer);
//  mqttClient.publish(topicBuffer, messageBuffer);
//}

void publish_sensor_state(char ref, byte state)
{  
  // create message in format "ref,state"
  messageBuffer[0] = '\0';
//  strcpy(messBuffer, ref);
  sprintf(messageBuffer, "%c%c%i", ref, COMMAND_SEPARATOR, state);
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*) pgm_read_word(&(STATUS_TOPICS[4])));
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  mqttClient.publish(topicBuffer, messageBuffer);
}


#endif   /* HOMESECURITYIOT_MQTTCONFIG_H_ */

