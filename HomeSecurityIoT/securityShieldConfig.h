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

// sensor states
//const byte WIRE_SHORTED = 0;
//const byte NORMAL_STATE = 1;
//const byte SENSOR_TRIGGERED = 2;
//const byte TAMPERED = 3;

typedef enum {
  WIRE_SHORTED = 0,
  NORMAL       = 1,
  TRIGGERED    = 2,
  TAMPERED     = 3,
} sensorStates_t;


unsigned long sensorReadPreviousMillis   = 0UL;

const unsigned long SENSOR_READ_INTERVAL = 500UL;           // interval at which to take measurement (milliseconds)

boolean soundAlarm                       = false;

// see http://playground.arduino.cc/Code/ResourceFriendlyStructs
typedef struct {
   byte            input;
   byte            led;
   char            ref;
   sensorStates_t  state;
} sensor_t;

sensor_t sensors[] = { { CHANNEL_A_INPUT, CHANNEL_A_LED, 'A', NORMAL },
                       { CHANNEL_B_INPUT, CHANNEL_B_LED, 'B', NORMAL },
                       { CHANNEL_C_INPUT, CHANNEL_C_LED, 'C', NORMAL },
                       { CHANNEL_D_INPUT, CHANNEL_D_LED, 'D', NORMAL },
                     };

void security_sensor_shield_init()
{
  for (byte idx = 0; idx < ARRAY_SIZE(sensors); idx++) {
    pinMode(sensors[idx].input, INPUT);
  }
  for (byte idx = 0; idx < ARRAY_SIZE(sensors); idx++) {
    pinMode(sensors[idx].led, OUTPUT);
    digitalWrite(sensors[idx].led, HIGH);
  }
}

/**
 * Returns the state of a sensor and sets led status
 */
//byte checkSensor(byte sensorInput, byte statusOutput)
sensorStates_t check_sensor(byte sensorInput, byte statusOutput)
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
    digitalWrite(statusOutput, LOW); // Turn the associated status LED off
    return WIRE_SHORTED;
  } else if ( sensorReading >= 400 && sensorReading < 590 ) {
    // Normal state, sensor not triggered
    DEBUG_LOG(1, "NORMAL STATE");
#if USE_OLED_SHIELD
    soundAlarm = false;
#endif
    digitalWrite(statusOutput, LOW); // Turn the associated status LED off
    return NORMAL;
  } else if ( sensorReading >= 590 && sensorReading < 800 ) {
    // Sensor triggered.
    DEBUG_LOG(1, "SENSOR TRIGGERED");
#if USE_OLED_SHIELD
    alertTone();
    soundAlarm = true;
#endif
    digitalWrite(statusOutput, HIGH); // Turn the associated status LED on
    return TRIGGERED;
  } else {
    // Open circuit. Cut or tamper triggered.
    DEBUG_LOG(1, "TAMPERED");
#if USE_OLED_SHIELD
    soundAlarm = false;
#endif
    digitalWrite(statusOutput, LOW); // Turn the associated status LED off
    return TAMPERED;
  }
}

void check_sensors() {
  for (byte idx = 0; idx < ARRAY_SIZE(sensors); idx++) {
    sensorStates_t state = check_sensor(sensors[idx].input, sensors[idx].led);
    if (state != sensors[idx].state) {
      sensors[idx].state = state;
#if USE_MQTT
      if(mqttClientConnected) {
        publish_sensor_state(sensors[idx].ref, state);
      }
#endif
    }
  }
}


#endif   /* HOMESECURITYIOT_SECURITYSHIELDCONFIG_H_ */
