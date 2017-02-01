#include <Wifi_S08_v2.h>

#define SSID "BLURP"
#define PASSWD "MOMENTUM"

#define POLLPERIOD 0
#define BLOCKCHECK 100
#define WAIT 100

ESP8266 wifi = ESP8266(0,false);
String domain = "192.168.4.1";
String path = "/"; 
String MAC;
unsigned long lastRequest = 0;

/***** pin setup ******/
#define led 13
#define digLeft 14
#define analogLeft 21
#define digRight 15
#define analogRight 20
#define encoder 2 
/********************/

/*********** Other constants used later on*********/
#define SPEED 128
#define NINETY 125

/***** state setup ******/
typedef enum { STOP, FW, BW, CW, CCW } states;
//initialize stationary state
states state = STOP;

/********************/


void setup() {
  pinMode(digLeft, OUTPUT);
  pinMode(digRight, OUTPUT);
  
  Serial.begin(115200);
  wifi.begin();
  MAC = wifi.getMAC();
  wifi.connectWifi(SSID, PASSWD);
  while (!wifi.isConnected()); //wait for connection
 // Serial.println("wifi connection succesfull");

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  pinMode (encoder,INPUT);
}

states oldState;
char dataToSend[15];
char response[15];
void loop(){
  
  while(wifi.isBusy());
  wifi.sendRequest(GET, domain, 80, path, "requesting");
  Serial.println("request sent to client");
  while(!wifi.hasResponse());
  responseExtract(wifi.getResponse()).toCharArray(response, 15);
  Serial.print("response received: ");
  Serial.println(response);
  sprintf(dataToSend, "~%s~", response);
  while(wifi.isBusy());
  wifi.sendRequest(GET, domain, 80, path, dataToSend);
  while(!wifi.hasResponse());
  wifi.getResponse();
  Serial.println("response sent and buffer cleared");

  oldState = state;
  state = getState(response);
  if(oldState != state){
    setMotors();
    Serial.println("motors changed");
  }
 
}

String responseExtract(String s){
  int beginInd = s.indexOf("~");
  int endInd= s.lastIndexOf("~");
  return( s.substring(beginInd+1,endInd));
}

/**** HELPER ****/
states getState(String response){
      if(response == "FW") return(FW);
      if(response == "BW") return(BW);
      if(response == "CW")return(CW);
      if(response == "CCW")return(CCW);
      if(response == "STOP")return(STOP);
      
  }

void blink(){
       if(wifi.hasResponse()){
        wifi.getResponse(); // DO WE NEED THIS????
      }
      digitalWrite(led, LOW);
      delay(500);
      digitalWrite(led, HIGH);
      delay(500);
      digitalWrite(led, LOW);
      delay(500);
      digitalWrite(led, HIGH);
}
/****************/



void setMotors(){
  switch(state){
    case FW:
      forward();
      break;
    case BW:
      backward();
      break;
    case CW:
      clockwise();
      break;
    case CCW:
      counterclockwise();
      break;
    case STOP:
      stopmotors();
      break;
  }
}

/***** STATE FUNCTIONS *******/
void forward(){
      digitalWrite(digLeft, HIGH); //irrelevant
      digitalWrite(digRight, HIGH); //irrelevant
  
      analogWrite(analogLeft, SPEED);
      analogWrite(analogRight, SPEED);
}

void backward(){
      digitalWrite(digLeft, LOW); //irrelevant
      digitalWrite(digRight, LOW); //irrelevant
  
      analogWrite(analogLeft, SPEED);
      analogWrite(analogRight, SPEED);
}

void clockwise(){ //finite turn
      int encoderLast = LOW;
      int n = LOW;
      int encoderPos = 0;
      
      digitalWrite(digLeft, HIGH); 
      digitalWrite(digRight, LOW);
      analogWrite(analogLeft, SPEED);
      analogWrite(analogRight, SPEED);

      while(encoderPos < NINETY){
        n = digitalRead(encoder);
        if ((encoderLast == LOW) && (n == HIGH)) {
          encoderPos++;
        } 
        encoderLast = n;
      }

      stopmotors();
}

  

void counterclockwise(){ //finite turn
      int encoderLast = LOW;
      int n = LOW;
      int encoderPos = 0;
      
      digitalWrite(digLeft, LOW); //irrelevant
      digitalWrite(digRight, HIGH); //irrelevant
  
      analogWrite(analogLeft, SPEED);
      analogWrite(analogRight, SPEED);

      while(encoderPos < NINETY){
        n = digitalRead(encoder);
        if ((encoderLast == LOW) && (n == HIGH)) {
          encoderPos++;
        } 
        encoderLast = n;
      }

      stopmotors();
}

void stopmotors(){
      digitalWrite(digLeft, HIGH); //irrelevant
      digitalWrite(digRight, HIGH); //irrelevant
  
      analogWrite(analogLeft, 0);
      analogWrite(analogRight, 0);
}
/*********************************************/









