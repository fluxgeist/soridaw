/*---------------------------------------------------------------------------------------------
This is the code from Peri
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

#include <ping.h>
int cmSmooth = 0;
int MeasurementsToAverage = 16;
int period = 300;
unsigned long time_now = 0;
static Ping_Data pingA;
int trigPin2 = D7;    // Trigger
int echoPin2 = D8;    // Echo
int relay1 = D1;    // Lamp
int relay2 = D2;    // LED
int relay3 = D3;    // Yellow
int relay4 = D4;    // None
long duration1, cmSmooth1, duration2, cmSmooth2;
//-----------------------------------------
char ssid[] = "flux1";          // your network SSID (name)
char pass[] = "fluxdaemon";                    // your network password

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress inIp(172,20,10,10);        // remote IP (not needed for receive)
const IPAddress outIp(172,20,10,2);        // remote IP (not needed for receive)
const unsigned int outPort = 9999;          // remote port (not needed for receive)
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)


OSCErrorCode error;
unsigned int ledState = LOW;              // LOW means led is *on*

#ifndef BUILTIN_LED
#ifdef LED_BUILTIN
#define BUILTIN_LED LED_BUILTIN
#else
#define BUILTIN_LED 13
#endif
#endif

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, ledState);    // turn *on* led

  Serial.begin(115200);
  //-----------------------------------------
  int triggerPin = D5;
  int echoPin = D6;
  ping_init(&pingA, triggerPin, echoPin, PING_MM); // set the trigger and echo pin to the same number for one-pin mode
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  

      // Specify a static IP address for NodeMCU - only needeed for receiving messages)
    // If you erase this line, your ESP8266 will get a dynamic IP address
    WiFi.config(IPAddress(inIp),IPAddress(192,168,0,1), IPAddress(255,255,255,0)); 

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


void led(OSCMessage &msg) {
  
  ledState = msg.getInt(1);
  digitalWrite(BUILTIN_LED, ledState);
  Serial.print("/led: ");
  Serial.println(ledState);
}

void loop() {
    sendOSC(touchRead(T2), "/area1/touch1/");
    sendOSC(touchRead(T3), "/area1/touch2/");
    sendOSC(touchRead(T4), "/area1/touch3/");
    sendOSC(touchRead(T5), "/area1/touch4/");

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

void toggleOnOff1(OSCMessage &msg, int addrOffset){
  ledState = (boolean) msg.getInt(0);
//  digitalWrite(boardLed, (ledState + 1) % 2);   // Onboard LED works the wrong direction (1 = 0 bzw. 0 = 1)
  if (ledState) {
    Serial.println("LED 1 on");
  }
  else {
    Serial.println("LED 1 off");
  }
  ledState = !ledState;                         // toggle the state from HIGH to LOW to HIGH to LOW ...
}
void toggleOnOff2(OSCMessage &msg, int addrOffset){
  ledState = (boolean) msg.getInt(0);
//  digitalWrite(boardLed, (ledState + 1) % 2);   // Onboard LED works the wrong direction (1 = 0 bzw. 0 = 1)
  if (ledState) {
    Serial.println("LED 2 on");
  }
  else {
    Serial.println("LED 2 off");
  }
  ledState = !ledState;                         // toggle the state from HIGH to LOW to HIGH to LOW ...
}
void toggleOnOff3(OSCMessage &msg, int addrOffset){
  ledState = (boolean) msg.getInt(0);
//  digitalWrite(boardLed, (ledState + 1) % 2);   // Onboard LED works the wrong direction (1 = 0 bzw. 0 = 1)
  if (ledState) {
    Serial.println("LED 3 on");
  }
  else {
    Serial.println("LED 3 off");
  }
  ledState = !ledState;                         // toggle the state from HIGH to LOW to HIGH to LOW ...
}
void toggleOnOff4(OSCMessage &msg, int addrOffset){
  ledState = (boolean) msg.getInt(0);
//  digitalWrite(boardLed, (ledState + 1) % 2);   // Onboard LED works the wrong direction (1 = 0 bzw. 0 = 1)
  if (ledState) {
    Serial.println("LED 4 on");
  }
  else {
    Serial.println("LED 4 off");
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
//        delay(10);
}