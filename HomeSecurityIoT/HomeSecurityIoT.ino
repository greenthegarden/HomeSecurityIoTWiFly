#include <Arduino.h>

#include "config.h"

boolean mqtt_connect()
{
  DEBUG_LOG(1, "MQTT:");
  DEBUG_LOG(1, "   connecting");
  if (mqttClient.connect(mqttClientId)) {
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

  mqtt_init();

  // Configure Ethernet
  DEBUG_LOG(1, "Ethernet:")
  DEBUG_LOG(1, "   configuring ...");
//  ethernet_init();
//  delay(NETWORK_STARTUP_DELAY); // allow some time for Ethernet processor to come out of reset on Arduino power up:
  if (Ethernet.begin(mac) == 0) {
    DEBUG_LOG(1, "IP failed");
  } else {
    DEBUG_LOG(1, Ethernet.localIP());
  }

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
        }
      }
    } else {
      // Client connected
      mqttClient.loop();
    }

  if (now - sensorReadPreviousMillis >= SENSOR_READ_INTERVAL) {
    sensorReadPreviousMillis = now;
    check_sensors();
  }

#if USE_OLED_SHIELD
  if (soundAlarm) {
    alertTone();
  }
#endif

}
