#include <Wifi_S08_v2.h>



#define SSID "BLURP"
#define PASSWD "MOMENTUM"

#define POLLPERIOD 0
#define BLOCKCHECK 100

ESP8266 wifi = ESP8266(0,false);

String MAC;
unsigned long lastRequest = 0;

int led = 13;
int light = 20; //PWM pin
int buzzer = 9; //PWM pin
int lightVal, buzzVal;

void setup() {
  Serial.begin(115200);
  wifi.begin();
  MAC = wifi.getMAC();
  wifi.connectWifi(SSID, PASSWD);
  while (!wifi.isConnected()); //wait for connection

  //check blocking
  pinMode(led, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

int count = 0;
int lightL, lightR;
int beginInd, splitInd, endInd;

void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(BLOCKCHECK);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(BLOCKCHECK);               // wait for a second

  
  if (wifi.hasResponse()) {
    String response = wifi.getResponse();
    beginInd = response.indexOf("~");
    splitInd= response.lastIndexOf("~");
    endInd =response.indexOf("`");
    
    lightL = response.substring(beginInd+1,splitInd).toInt();
    lightR = response.substring(splitInd+1, endInd).toInt();
    
    Serial.print("RESPONSE: ");
    //Serial.print(response);
    Serial.print(lightL);
    Serial.print(", ");
    Serial.println(lightR);
    count++;
    Serial.println(count);

    buzzVal = (int)(10.+(log(1+(1024.-lightR)/1024.)/log(2.))*2000.);
    Serial.print("buzz val ");
    Serial.print(buzzVal);
    tone(buzzer, buzzVal);
    lightVal = (int)((log(1+(1024.-lightR)/1024.)/log(2.))*256.);
    Serial.print("light val ");
    Serial.println(lightVal);
    analogWrite(light, lightVal);
  }

  if (!wifi.isBusy() && millis()-lastRequest > POLLPERIOD) {
    String domain = "192.168.4.1";
    String path = "/"; 
    
    wifi.sendRequest(GET, domain, 80, path, "");
    lastRequest = millis();
  }
}
