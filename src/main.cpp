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
motor m = motor(&PORTB, 15, 2);
void loop()
{
  if (Serial.available())
  {
    byte t[1];
    Serial.readBytes(t, 1);
    // for (size_t i = 0; i < 3; i++)
    // {
    // }

    // m.CMD(t[0], (uint16_t) * (t + 1));
    switch (t[0])
    {
    //case 'f':
    case 'F':
      m.CMD(Step, 96);
      break;
    //case 'b':
    case 'B':
      m.CMD(Step, -192);
      break;
    default:
      break;
    }
    //Serial.write(m.CMD(GetDegPosition, 0));
  }
  m.update();
}
void BinarySer()
{
  if ((uint8_t)Serial.available() >= (uint8_t)(sizeof(uint8_t) + sizeof(uint16_t)))
  {
    byte t[3];
    Serial.readBytes(t, 3);
    uint16_t val = t[1];
    val = (val << 8) + t[2];
    m.CMD(t[0], t[1] << 8);
  }
  m.update();
}
