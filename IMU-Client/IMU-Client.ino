#include <Wifi_S08_v2.h>



#define SSID "BLURP"
#define PASSWD "MOMENTUM"

#define POLLPERIOD 0
#define BLOCKCHECK 100
#define WAIT 100

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
  Serial.println("wifi connection succesfull");

  //check blocking
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
}

int count = 0;

int SPEED = 128;

int beginInd, endInd;

bool forward, backward, counterclockwise, clockwise;

void loop() {

  getValues();
  
    if(forward) // going forward
    {
      while(!backward){
       //Serial.println("f");
        setMotors(1,1);
        getValues();
        }
      stopAndWait();
    }

    else if(clockwise) // turn clockwise
    {
      while(!counterclockwise){
     //   Serial.println("r");
        setMotors(1,0);
              getValues();
        }
        stopAndWait();
    }

    else if(backward) // going backward
    {
      while(!forward){
      //  Serial.println("b");
        setMotors(0,0);
        getValues();
        }
        stopAndWait();
    }

    else if(counterclockwise){ //turn counterclockwise
      while(!clockwise){
     //   Serial.println("l");
        setMotors(0,1);
        getValues();
        }
        stopAndWait();
      } 

    else {
      digitalWrite(digLeft, LOW); //irrelevant
      digitalWrite(digRight, LOW); //irrelevant
      analogWrite(analogLeft, 0);
      analogWrite(analogRight, 0);
      }
      
    
 }



/*********************** helper functions ********************/

char values[500];
void getValues(){ 
  
    String response;
    
    forward = 0;
    backward = 0;
    clockwise = 0;
    counterclockwise = 0;
    
  if(wifi.hasResponse()){
      response = wifi.getResponse();
      
      beginInd = response.indexOf("~");
      endInd =response.lastIndexOf("~");
      response = response.substring(beginInd+1, endInd);
      forward = response.charAt(0) == '1';
      backward = response.charAt(1) == '1';
      clockwise = response.charAt(2) == '1';
      counterclockwise = response.charAt(3) == '1';      
      Serial.println(response);
  }
    
    if (!wifi.isBusy() && millis()-lastRequest > POLLPERIOD) {
      String domain = "192.168.4.1";
      String path = "/"; 
  
      sprintf(values, "BoolValuesSentFromClient:f=%d,b=%d,cw=%d,ccw=%d", forward, backward, clockwise, counterclockwise);
      Serial.println(values);
      wifi.sendRequest(GET, domain, 80, path, values);
      lastRequest = millis();
    }      

}

void setMotors(int left, int right){
      digitalWrite(digLeft, left); //irrelevant
      digitalWrite(digRight, right); //irrelevant
  
      analogWrite(analogLeft, SPEED);
      analogWrite(analogRight, SPEED);
      }

void stopAndWait(){
  Serial.println("stop and wait");
      digitalWrite(digLeft, LOW); //irrelevant
      digitalWrite(digRight, LOW); //irrelevant
      analogWrite(analogLeft, 0);
      analogWrite(analogRight, 0);
      delay(WAIT);
      if(wifi.hasResponse()){
        wifi.getResponse();
      }
      Serial.println("wait over, returning to loop");
      digitalWrite(led, LOW);
      delay(500);
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      delay(500);
      digitalWrite(led, HIGH);
      
}

