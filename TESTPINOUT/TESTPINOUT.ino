#include <HCSR04.h>

byte triggerPin = 27;
byte echoCount = 2;
byte* echoPins = new byte[echoCount] { 26, 13 };

void setup () {
  Serial.begin(115200);
  HCSR04.begin(triggerPin, echoPins, echoCount);
}

void loop () {
    int time = millis();

  double* distances = HCSR04.measureDistanceCm();

  for (int i = 0; i < echoCount; i++) {
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(distances[i]);
    Serial.println(" cm");
  }
  
  Serial.println("---");
    Serial.println(millis()-time);        

  // delay(250);
}