#ifndef HOMESECURITYIOT_MQTTCONFIG_H_
#define HOMESECURITYIOT_MQTTCONFIG_H_


#include <PubSubClient.h>


// MQTT parameters
IPAddress mqttServerAddr(192, 168, 1, 52);        //  mqtt server on emonPi
char mqttClientId[BUFFER_SIZE];
//char* mqttClientId                                = "security";
const char * MQTT_USERNAME                        = "emonpi";
const char * MQTT_PASSWORD                        = "emonpimqtt2016";
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
  payloadBuffer[0] = '\0';
  strcpy_P(payloadBuffer, (char*)pgm_read_word(&(MQTT_PAYLOADS[MQTT_PAYLOAD_CONNECTED_IDX])));
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[MQTT_STATUS_IDX])));
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_ip_address()
{
  payloadBuffer[0] = '\0';
#if USE_WIFLY
  strcpy(payloadBuffer, WiFly.ip());
#elif USE_ETHERNET
  sprintf(payloadBuffer, "%d.%d.%d.%d", Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3]);
#else
  // publish an error
  strcpy_P(payloadBuffer, (char*)pgm_read_word(&(MQTT_PAYLOADS[MQTT_PAYLOAD_ERROR_IDX])));
#endif
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[IP_ADDR_STATUS_IDX])));
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_uptime()
{
  payloadBuffer[0] = '\0';
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[UPTIME_STATUS_IDX])));
  mqttClient.publish(topicBuffer, ltoa(millis(), payloadBuffer, 10));
}

void publish_sensor_state(char ref, byte state)
{
  // create message in format "ref,state"
  payloadBuffer[0] = '\0';
//  strcpy(messBuffer, ref);
  sprintf(payloadBuffer, "%c%c%i", ref, COMMAND_SEPARATOR, state);
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*) pgm_read_word(&(STATUS_TOPICS[SENSOR_STATUS_IDX])));
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

byte mqtt_defaults()
{
  strcpy(mqttClientId, "security");
  mqttPort = 1883;
  return 1;
}

byte mqtt_init()
{
  mqtt_defaults();
  return 1;
}


#endif   /* HOMESECURITYIOT_MQTTCONFIG_H_ */
