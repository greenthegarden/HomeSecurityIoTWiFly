#ifndef HOMESECURITYIOT_SECURITYSHIELDCONFIG_H_
#define HOMESECURITYIOT_SECURITYSHIELDCONFIG_H_


// Macros
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

// Use analog inputs 0 through 3 for sensor connections A through D
const byte CHANNEL_A_INPUT = A0;
const byte CHANNEL_B_INPUT = A1;
const byte CHANNEL_C_INPUT = A2;
const byte CHANNEL_D_INPUT = A3;

// Use digital outputs 4 through 7 for status indicator LEDs A through D
const byte CHANNEL_A_LED = 4;
const byte CHANNEL_B_LED = 5;
const byte CHANNEL_C_LED = 6;
const byte CHANNEL_D_LED = 7;


const byte SENSOR_INPUTS[] = { CHANNEL_A_INPUT, CHANNEL_B_INPUT, CHANNEL_C_INPUT, CHANNEL_D_INPUT };
const byte SENSOR_LEDS[] = { CHANNEL_A_LED, CHANNEL_B_LED, CHANNEL_C_LED, CHANNEL_D_LED };
const char SENSOR_REFS[] = { 'A', 'B', 'C', 'D' };
byte sensorStates[] = { 0, 0, 0, 0 };

// sensor states
const byte WIRE_SHORTED = 0;
const byte NORMAL_STATE = 1;
const byte SENSOR_TRIGGERED = 2;
const byte TAMPERED = 3;

unsigned long sensorReadPreviousMillis   = 0UL;

const unsigned long SENSOR_READ_INTERVAL = 500UL;           // interval at which to take measurement (milliseconds)

boolean soundAlarm                       = false;

void securitySensorShieldSetup()
{
  for (byte idx = 0; idx < ARRAY_SIZE(SENSOR_INPUTS); idx++) {
    pinMode(SENSOR_INPUTS[idx], INPUT);
  }
  for (byte idx = 0; idx < ARRAY_SIZE(SENSOR_LEDS); idx++) {
    pinMode(SENSOR_LEDS[idx], OUTPUT);
    digitalWrite(SENSOR_LEDS[idx], LOW);
  }
}


/**
 * Checks the state of a sensor and reports it to the connected host
 */
byte checkSensor(byte sensorInput, byte statusOutput)
{
  int sensorReading = analogRead(sensorInput);
  DEBUG_LOG(1, "Sensor reading: ");
  DEBUG_LOG(1, sensorReading);
  if ( sensorReading < 400 ) {
    // Wire shorted. Possible tampering.
    DEBUG_LOG(1, "WIRE SHORTED");
#if USE_OLED_SHIELD
    soundAlarm = false;
#endif
    digitalWrite(statusOutput, HIGH); // Turn the associated status LED on
    return WIRE_SHORTED;
  } else if ( sensorReading >= 400 && sensorReading < 590 ) {
    // Normal state, sensor not triggered
    DEBUG_LOG(1, "NORMAL STATE");
#if USE_OLED_SHIELD
    soundAlarm = false;
#endif
    digitalWrite(statusOutput, LOW); // Turn the associated status LED off
    return NORMAL_STATE;
  } else if ( sensorReading >= 590 && sensorReading < 800 ) {
    // Sensor triggered.
    DEBUG_LOG(1, "SENSOR TRIGGERED");
#if USE_OLED_SHIELD
    alertTone();
    soundAlarm = true;
#endif
    digitalWrite(statusOutput, HIGH); // Turn the associated status LED on
    return SENSOR_TRIGGERED;
  } else {
    // Open circuit. Cut or tamper triggered.
    DEBUG_LOG(1, "TAMPERED");
#if USE_OLED_SHIELD
    soundAlarm = false;
#endif
    digitalWrite(statusOutput, HIGH); // Turn the associated status LED on
    return TAMPERED; 
  }
}

void check_sensors() {
  for (byte idx = 0; idx < ARRAY_SIZE(SENSOR_INPUTS); idx++) {
    byte state = checkSensor(SENSOR_INPUTS[idx], SENSOR_LEDS[idx]);
    if (state != sensorStates[idx]) {
      sensorStates[idx] = state;
      if(mqttClientConnected) {
        publish_sensor_state(SENSOR_REFS[idx], state);
      }
    }
  }
}


#endif   /* HOMESECURITYIOT_SECURITYSHIELDCONFIG_H_ */

