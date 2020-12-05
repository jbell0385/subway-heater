#include <Arduino.h>

/*
 * PIR sensor tester
 */

int powerSupplyPin = D0; // choose the pin for the LED
int pirPin = D2;
int tempPin = A0;

float tempTotal = 0.0;
int tempCounter = 0;
float tempVal = 0.0;
float temp = 0.0;
float tempAvg = 0.0;
bool tempTrigger = false;
unsigned long myTime = millis();
int timeShutOff = 300000;
int pirState = LOW;
int pirVal = 0;
bool enteredLow = true;
bool initialCalibration = true;
bool initialSetup = true;

void setup()
{
  pinMode(powerSupplyPin, OUTPUT); // declare LED as output
  pinMode(pirPin, INPUT);
  pinMode(tempPin, INPUT); // declare sensor as input

  Serial.begin(9600);
}

void loop()
{
  pirVal = digitalRead(pirPin);
  tempVal = analogRead(tempPin);
  temp = tempVal / 1024.0;
  temp = temp * 3.3;
  temp = temp - 0.5;
  temp = temp * 100.0;
  temp = temp * 9.0 / 5.0 + 32.0;
  Serial.println(pirVal);

  tempTotal += temp;
  tempCounter++;
  if (tempCounter == 25)
  {
    tempAvg = tempTotal / tempCounter;
    tempTotal = 0;
    tempCounter = 0;
  }
  if (tempAvg > 89)
  {
    tempTrigger = false;
  }
  else if (tempAvg < 82)
  {
    tempTrigger = true;
  }

  if (!tempTrigger)
  {
    digitalWrite(powerSupplyPin, LOW);
  }

  Serial.println(tempAvg);
  if (pirVal && tempTrigger)
  {
    digitalWrite(powerSupplyPin, HIGH);
    enteredLow = true;
  }
  else if (!pirVal)
  {
    if (enteredLow)
    {
      myTime = millis();
      enteredLow = false;
    }
    Serial.println(millis() - myTime);
    if (millis() - myTime > timeShutOff)
    {
      digitalWrite(powerSupplyPin, LOW);
    }
  }
}