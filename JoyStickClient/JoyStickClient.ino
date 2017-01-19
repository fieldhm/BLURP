#include <Wifi_S08_v2.h>



#define SSID "BLURP"
#define PASSWD "MOMENTUM"

#define POLLPERIOD 0
#define BLOCKCHECK 100

ESP8266 wifi = ESP8266(0,false);

String MAC;
unsigned long lastRequest = 0;

int led = 13;

int digLeft = 14;
int analogLeft = 21;
int digRight = 15;
int analogRight = 20;

void setup() {
  pinMode(digLeft, OUTPUT);
  pinMode(digRight, OUTPUT);
  
  Serial.begin(115200);
  wifi.begin();
  MAC = wifi.getMAC();
  wifi.connectWifi(SSID, PASSWD);
  while (!wifi.isConnected()); //wait for connection

  //check blocking
  pinMode(led, OUTPUT);
}

int count = 0;
int UD, LR;

//values for joystick control ranges
int UD_LOW = 250;
int UD_HIGH = 900;
int LR_LOW = 400;
int LR_HIGH = 600;

int SPEED = 128;

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
    
    UD = response.substring(beginInd+1,splitInd).toInt();
    LR = response.substring(splitInd+1, endInd).toInt();
    
    Serial.print("RESPONSE: ");
    //Serial.print(response);
    Serial.print(UD);
    Serial.print(", ");
    Serial.println(LR);
    count++;
    Serial.println(count);

    
    if( (UD > UD_HIGH) &&(LR<LR_HIGH) && (LR > LR_LOW) ) // going forward
    {
    Serial.println("f");
    digitalWrite(digLeft, HIGH);
    digitalWrite(digRight, HIGH);

    analogWrite(analogLeft, SPEED);
    analogWrite(analogRight, SPEED);
    }

    else if( (LR > LR_HIGH) && (UD<UD_HIGH) && (UD > UD_LOW)) // turn clockwise
    {
    Serial.println("r");
    digitalWrite(digLeft, HIGH);
    digitalWrite(digRight, LOW);

    analogWrite(analogLeft, SPEED);
    analogWrite(analogRight, SPEED);
    }

    else if( (UD < UD_LOW ) && (LR<LR_HIGH) && (LR > LR_LOW) ) // going backward
    {
    Serial.println("b");
    digitalWrite(digLeft, LOW);
    digitalWrite(digRight, LOW);

    analogWrite(analogLeft, SPEED);
    analogWrite(analogRight, SPEED);
    }

    else if( (LR < LR_LOW) && (UD<UD_HIGH) && (UD > UD_LOW)) //turn counterclockwise
    {
    Serial.println("l");
    digitalWrite(digLeft, LOW);
    digitalWrite(digRight, HIGH);

    analogWrite(analogLeft, SPEED);
    analogWrite(analogRight, SPEED);
    } 

    else {
    digitalWrite(digLeft, LOW); //irrelevant
    digitalWrite(digRight, LOW); //irrelevant

    analogWrite(analogLeft, 0);
    analogWrite(analogRight, 0);
    }
  }

  if (!wifi.isBusy() && millis()-lastRequest > POLLPERIOD) {
    String domain = "192.168.4.1";
    String path = "/"; 
    
    wifi.sendRequest(GET, domain, 80, path, "");
    lastRequest = millis();
  }
}
