#include <Arduino.h>
#include <Wire.h>
#include "MotorControl.cpp"
void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}
motor m = motor((volatile uint8_t *)&PORTB);
void loop()
{
  if ((uint8_t)Serial.available() >= (uint8_t)(sizeof(uint8_t) + sizeof(uint16_t)))
  {
    byte t[sizeof(uint8_t) + sizeof(uint16_t)];
    Serial.readBytes(t, 1);
    // for (size_t i = 0; i < 3; i++)
    // {
      Serial.println(t[0]);
    // }

    // m.CMD(t[0], (uint16_t) * (t + 1));
    switch (t[0])
    {
    case 'F':
      m.CMD(Step, 48);
      break;
    case 'B':
      m.CMD(Step, -48);
      break;
    default:
      break;
    }
  }
  m.update();
}
