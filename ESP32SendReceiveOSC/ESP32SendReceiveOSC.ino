/*---------------------------------------------------------------------------------------------

  Open Sound Control (OSC) library for the ESP8266/ESP32

  Example for receiving open sound control (OSC) messages on the ESP8266/ESP32
  Send integers '0' or '1' to the address "/led" to turn on/off the built-in LED of the esp8266.

  This example code is in the public domain.

--------------------------------------------------------------------------------------------- */
#include <WiFi.h>
#include <WiFiUdp.h>  

#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

int pirPin = 35;              // the pin that the pirPin is atteched to
int presence = LOW;             // by default, no motion detected
int pirStatus = 0;                 // variable to store the pirPin status (pirStatusue)

int trigPin[] = {27,18};
int echoPin[] = {26,19};

int relayPin[] = {5,17,16,4};

int duration[2];
int distance[2];
char ssid[] = "flux1";          // your network SSID (name)
char pass[] = "fluxdaemon";                    // your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress outIp(172,20,10,2);        // remote IP (not needed for receive)
const unsigned int outPort = 9999;          // remote port (not needed for receive)
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)


OSCErrorCode error;
unsigned int ledState = LOW;              // LOW means led is *on*

void setup() {
  Serial.begin(115200);
  for (int m = 0; m<2; m++) {
  pinMode(trigPin[m], OUTPUT); 
  pinMode(echoPin[m], INPUT);
  }

  pinMode(pirPin, INPUT);    // initialize pirPin as an input
  
  pinMode(relayPin[0],OUTPUT);
  pinMode(relayPin[1],OUTPUT);
  pinMode(relayPin[2],OUTPUT);
  pinMode(relayPin[3],OUTPUT);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
#ifdef ESP32
  Serial.println(localPort);
#else
  Serial.println(Udp.localPort());
#endif

}

void loop() {
    sensorRead();
    pirSense();
    sendOSC(float(distance[0]), "/area1/touch1/");
    sendOSC(float(distance[1]), "/area1/touch2/");
    sendOSC(pirStatus, "/area1/touch3/");
    sendOSC(touchRead(T5), "/area1/touch4/");

    oscRead();
  
}

void toggleOnOff1(OSCMessage &msg, int addrOffset){
  ledState = (boolean) msg.getInt(0);
//  digitalWrite(boardLed, (ledState + 1) % 2);   // Onboard LED works the wrong direction (1 = 0 bzw. 0 = 1)
  if (ledState) {
    Serial.println("LED 1 on");
    digitalWrite(relayPin[0],HIGH);
  }
  else {
    Serial.println("LED 1 off");
    digitalWrite(relayPin[0],LOW);
  }
  ledState = !ledState;                         // toggle the state from HIGH to LOW to HIGH to LOW ...
}
void toggleOnOff2(OSCMessage &msg, int addrOffset){
  ledState = (boolean) msg.getInt(0);
//  digitalWrite(boardLed, (ledState + 1) % 2);   // Onboard LED works the wrong direction (1 = 0 bzw. 0 = 1)
  if (ledState) {
    Serial.println("LED 2 on");
    digitalWrite(relayPin[1],HIGH);
  }
  else {
    Serial.println("LED 2 off");
    digitalWrite(relayPin[1],LOW);
  }
  ledState = !ledState;                         // toggle the state from HIGH to LOW to HIGH to LOW ...
}
void toggleOnOff3(OSCMessage &msg, int addrOffset){
  ledState = (boolean) msg.getInt(0);
//  digitalWrite(boardLed, (ledState + 1) % 2);   // Onboard LED works the wrong direction (1 = 0 bzw. 0 = 1)
  if (ledState) {
    Serial.println("LED 3 on");
    digitalWrite(relayPin[2],HIGH);
  }
  else {
    Serial.println("LED 3 off");
    digitalWrite(relayPin[2],LOW);
  }
  ledState = !ledState;                         // toggle the state from HIGH to LOW to HIGH to LOW ...
}
void toggleOnOff4(OSCMessage &msg, int addrOffset){
  ledState = (boolean) msg.getInt(0);
//  digitalWrite(boardLed, (ledState + 1) % 2);   // Onboard LED works the wrong direction (1 = 0 bzw. 0 = 1)
  if (ledState) {
    Serial.println("LED 4 on");
    digitalWrite(relayPin[3],HIGH);
  }
  else {
    Serial.println("LED 4 off");
    digitalWrite(relayPin[3],LOW);
  }
  ledState = !ledState;                         // toggle the state from HIGH to LOW to HIGH to LOW ...
}

void sendOSC(int m, char *n){
        OSCMessage msgOut(n);
        msgOut.add(m);
        Udp.beginPacket(outIp, outPort);
        msgOut.send(Udp);
        Udp.endPacket();
        msgOut.empty();
}

void sensorRead() {
  for (int m = 0; m<2; m++){
  digitalWrite(trigPin[m], LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin[m], HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin[m], LOW);
  
  duration[m] = pulseIn(echoPin[m], HIGH);
  distance[m]= duration[m]*0.034/2;
  }
Serial.print(distance[0]);
Serial.print("---");
Serial.println(distance[1]);
}

void oscRead() {
  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      
      msg.route("/1/toggleLED",toggleOnOff1);
      msg.route("/2/toggleLED",toggleOnOff2);
      msg.route("/3/toggleLED",toggleOnOff3);
      msg.route("/4/toggleLED",toggleOnOff4);
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }

}

void pirSense() {
  pirStatus = digitalRead(pirPin);   // read pirPin pirStatusue
  Serial.println(pirStatus);
  if (pirStatus == HIGH) {           // check if the pirPin is HIGH
    // delay(500);                // delay 100 milliseconds 
    
    if (presence == LOW) {
      Serial.println("Motion detected!"); 
      presence = HIGH;       // update variable presence to HIGH
    }
  } 
  else {
      // delay(500);             // delay 200 milliseconds 
      
      if (presence == HIGH){
        Serial.println("Motion stopped!");
        presence = LOW;       // update variable presence to LOW
    }
  }
}
