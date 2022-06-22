int pirPin = 2;              // the pin that the pirPin is atteched to
int presence = LOW;             // by default, no motion detected
int pirStatus = 0;                 // variable to store the pirPin status (pirStatusue)

void setup() {
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(pirPin, INPUT);    // initialize pirPin as an input
  Serial.begin(9600);        // initialize serial
}

void loop(){
  pirStatus = digitalRead(pirPin);   // read pirPin pirStatusue
  if (pirStatus == HIGH) {           // check if the pirPin is HIGH
    digitalWrite(led, HIGH);   // turn LED ON
    delay(500);                // delay 100 milliseconds 
    
    if (presence == LOW) {
      Serial.println("Motion detected!"); 
      presence = HIGH;       // update variable presence to HIGH
    }
  } 
  else {
      digitalWrite(led, LOW); // turn LED OFF
      delay(500);             // delay 200 milliseconds 
      
      if (presence == HIGH){
        Serial.println("Motion stopped!");
        presence = LOW;       // update variable presence to LOW
    }
  }
}