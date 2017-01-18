#ifndef HOMESECURITYIOT_OLEDSHIELDCONFIG_H_
#define HOMESECURITYIOT_OLEDSHIELDCONFIG_H_


#include "pitches.h"

const byte BUZZER_PIN = 9;

void oledShieldSetup()
{
  pinMode(BUZZER_PIN, OUTPUT);
}

void alertTone()
{
  tone(BUZZER_PIN, NOTE_C4, 500);
  delay(500);
  tone(BUZZER_PIN, NOTE_C5, 500);
  delay(500);
  tone(BUZZER_PIN, NOTE_C6, 500);
  delay(500);
  noTone(BUZZER_PIN);
  delay(500);
}

const byte TONE_REPEATS = 5;

void repeatTone()
{
  for(byte idx=0; idx<TONE_REPEATS; idx++) {
    alertTone();
  }
}


#endif   /* HOMESECURITYIOT_OLEDSHIELDCONFIG_H_ */

