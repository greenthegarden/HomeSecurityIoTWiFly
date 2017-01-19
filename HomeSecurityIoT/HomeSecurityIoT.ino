#include <Arduino.h>

#include "config.h"

boolean mqtt_connect()
{
  DEBUG_LOG(1, "MQTT:");
  DEBUG_LOG(1, "   connecting");
  if (mqttClient.connect(mqttClientId, MQTT_USERNAME, MQTT_PASSWORD)) {
    DEBUG_LOG(1, "   connected");
    // Once connected, publish an announcement ...
    publish_connected();
//    publish_ip_address();
    // ... and subscribe to topics (should have list)
  } else {
    DEBUG_LOG(1, "failed, rc = ");
    DEBUG_LOG(1, mqttClient.state());
  }
  return mqttClient.connected();
}


/*--------------------------------------------------------------------------------------
  setup()
  Called by the Arduino framework once, before the main loop begins
  --------------------------------------------------------------------------------------*/
void setup()
{
#if DEBUG_LEVEL > 0
  Serial.begin(BAUD_RATE);
#endif

if (ethernet_init()) {
  DEBUG_LOG(1, "Ethernet configured");
} else {
  DEBUG_LOG(1, "Ethernet failed");
  DEBUG_LOG(1, "Halting");
  for (;;)
    ;
}

  mqtt_init();

  // set up for PIR sensor
  security_sensor_shield_init();
}

/*--------------------------------------------------------------------------------------
  loop()
  Arduino main loop
  --------------------------------------------------------------------------------------*/
void loop()
{
  unsigned long now = millis();

  if (!mqttClient.connected()) {
    mqttClientConnected = false;
    if (now - lastReconnectAttempt > RECONNECTION_ATTEMPT_INTERVAL) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (mqtt_connect()) {
        lastReconnectAttempt = 0;
        mqttClientConnected = true;
        DEBUG_LOG(2, "mqttClientConnected:");
        DEBUG_LOG(2, mqttClientConnected);
      }
    }
  } else {
    // Client connected
    mqttClient.loop();
  }

  // only take sensor measurements when connected to mqtt broker
  if (mqttClientConnected) {
    if (now - sensorReadPreviousMillis >= SENSOR_READ_INTERVAL) {
      sensorReadPreviousMillis = now;
      check_sensors();
    }
  }
}
