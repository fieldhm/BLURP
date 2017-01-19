#include <Wifi_S08_v2.h>

//tape means left ( note: ground power wiring flipped for right motor)

#define SSID "BLURP"
#define PASSWD "MOMENTUM"

#define POLLPERIOD 0
#define BLOCKCHECK 100


int led = 13;

int digLeft = 14;
int analogRight = 20;
int digRight = 15;
int analogLeft = 21;

void setup() {
  pinMode(digLeft, OUTPUT);
  pinMode(digRight, OUTPUT);
  pinMode(led, OUTPUT);
    digitalWrite(digLeft, HIGH);
  digitalWrite(digRight, HIGH);
  Serial.begin(115200);
  
}

void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(BLOCKCHECK);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(BLOCKCHECK);      
  
  digitalWrite(digLeft, LOW);
  digitalWrite(digRight, HIGH);

  analogWrite(analogRight, 50);
  analogWrite(analogLeft, 100);
  Serial.print("test");
  
  // wait for a second
  

}


