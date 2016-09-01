#ifndef HOMESECURITYIOT_MQTTCONFIG_H_
#define HOMESECURITYIOT_MQTTCONFIG_H_


#include <PubSubClient.h>


// MQTT parameters
IPAddress mqttServerAddr(192, 168, 1, 50);        // openHAB server
char mqttClientId[BUFFER_SIZE];
//char* mqttClientId                                = "security";
int mqttPort                                      = 1883;

boolean mqttClientConnected                       = false;

unsigned long lastReconnectAttempt                = 0UL;
const unsigned long RECONNECTION_ATTEMPT_INTERVAL = 30UL * 1000UL;  // attempt to reconnect every 30 seconds

const char COMMAND_SEPARATOR                      = ',';

// callback definition for MQTT
void callback(char* topic, uint8_t* payload, unsigned int length)
{
  // nothing to do here!!
}

PubSubClient mqttClient(mqttServerAddr, mqttPort, callback, networkClient);

// MQTT topic definitions

// MQTT payloads
const char MQTT_PAYLOAD_CONNECTED[]   PROGMEM = "CONNECTED";
const char MQTT_PAYLOAD_OK[]          PROGMEM = "OK";
const char MQTT_PAYLOAD_ERROR[]       PROGMEM = "ERROR";
const char MQTT_PAYLOAD_START[]       PROGMEM = "START";
const char MQTT_PAYLOAD_END[]         PROGMEM = "END";
const char MQTT_PAYLOAD_SLEEP[]       PROGMEM = "SLEEP";

PGM_P const MQTT_PAYLOADS[]           PROGMEM = { MQTT_PAYLOAD_CONNECTED,   // idx = 0
                                                  MQTT_PAYLOAD_OK,          // idx = 1
                                                  MQTT_PAYLOAD_ERROR,       // idx = 2
                                                  MQTT_PAYLOAD_START,       // idx = 3
                                                  MQTT_PAYLOAD_END,         // idx = 4
                                                  MQTT_PAYLOAD_SLEEP,       // idx = 5
                                                };

/* MQTT_PAYLOADS indices, must match table above */
typedef enum {
  MQTT_PAYLOAD_CONNECTED_IDX = 0,
  MQTT_PAYLOAD_OK_IDX        = 1,
  MQTT_PAYLOAD_ERROR_IDX     = 2,
  MQTT_PAYLOAD_START_IDX     = 3,
  MQTT_PAYLOAD_END_IDX       = 4,
  MQTT_PAYLOAD_SLEEP_IDX     = 5,
} mqtt_payloads;

// Status topics
const char MQTT_STATUS[]        PROGMEM = "homesecurity/interior/status/mqtt";
const char IP_ADDR_STATUS[]     PROGMEM = "homesecurity/interior/status/ip_addr";   // 36 chars
const char UPTIME_STATUS[]      PROGMEM = "homesecurity/interior/status/uptime";
const char MEMORY_STATUS[]      PROGMEM = "homesecurity/interior/status/memory";
const char SENSOR_STATUS[]      PROGMEM = "homesecurity/interior/status/sensor";

PGM_P const STATUS_TOPICS[]     PROGMEM = { MQTT_STATUS,         // idx = 0
                                            IP_ADDR_STATUS,      // idx = 1
                                            UPTIME_STATUS,       // idx = 2
                                            MEMORY_STATUS,       // idx = 3
                                            SENSOR_STATUS,       // idx = 4
                                           };
                                           
// STATUS_TOPICS indices, must match table above
typedef enum {
  MQTT_STATUS_IDX            = 0,
  IP_ADDR_STATUS_IDX         = 1,
  UPTIME_STATUS_IDX          = 2,
  MEMORY_STATUS_IDX          = 3,
  SENSOR_STATUS_IDX          = 4,
} status_topics;

void publish_connected()
{
  messageBuffer[0] = '\0';
  strcpy_P(messageBuffer, (char*)pgm_read_word(&(MQTT_PAYLOADS[MQTT_PAYLOAD_CONNECTED_IDX])));
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[MQTT_STATUS_IDX])));
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
  // publish an error
  strcpy_P(messageBuffer, (char*)pgm_read_word(&(MQTT_PAYLOADS[MQTT_PAYLOAD_ERROR_IDX])));
#endif
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[IP_ADDR_STATUS_IDX])));
  mqttClient.publish(topicBuffer, messageBuffer);
}

void publish_uptime()
{
  messageBuffer[0] = '\0';
  ltoa(millis(), messageBuffer, 10);
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[UPTIME_STATUS_IDX])));
  mqttClient.publish(topicBuffer, messageBuffer);
}

void publish_sensor_state(char ref, byte state)
{  
  // create message in format "ref,state"
  messageBuffer[0] = '\0';
//  strcpy(messBuffer, ref);
  sprintf(messageBuffer, "%c%c%i", ref, COMMAND_SEPARATOR, state);
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*) pgm_read_word(&(STATUS_TOPICS[SENSOR_STATUS_IDX])));
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  mqttClient.publish(topicBuffer, messageBuffer);
}

#if USE_SDCARD

// expects the following format for elements with the CONFIG_FILE:
// mqttBrokerAddrStr=192.168.1.50
// mqttClientId=security
// mqttPort=1883

boolean mqtt_configuration() {
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
    if (cfg.nameIs("mqttBrokerAddrStr")) {
      // Dynamically allocate a copy of the string.
      char* str = cfg.copyValue();
      mqttServerAddr.fromString(str);
      DEBUG_LOG(1, "Read mqttBrokerAddrStr: ");
      DEBUG_LOG(1, str);
    } else if (cfg.nameIs("mqttClientId")) { // mqttClientId string (char *)
      // Dynamically allocate a copy of the string.
      mqttClientId = cfg.copyValue();
      strcpy(mqttClientId, str);
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

byte mqtt_init()
{
#if !USE_SDCARD
  strcpy(mqttClientId, "security");
  mqttPort = 1883;
#else
  mqtt_configuration();
#endif
}


#endif   /* HOMESECURITYIOT_MQTTCONFIG_H_ */

