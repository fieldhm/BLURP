#include <Wifi_S08_v2.h>
#include <NewPing.h>

#define SSID "BLURP"
#define PASSWD "MOMENTUM"

#define SONAR_NUM 4 
#define MAX_DISTANCE 300 

#define POLLPERIOD 0
#define BLOCKCHECK 100
#define WAIT 100


ESP8266 wifi = ESP8266(0,false);
String domain = "192.168.4.1";
String path = "/"; 
String MAC;
unsigned long lastRequest = 0;

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(2, 3, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(4, 5, MAX_DISTANCE), 
  NewPing(6, 7, MAX_DISTANCE)
};


/***** pin setup ******/
#define led 13
#define digLeft 14
#define analogLeft 21
#define digRight 15
#define analogRight 20
/********************/

/*********** Other constants used later on*********/
#define SPEED 128

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
}




states oldState;
char dataToSend[500];
  
void loop(){
   if (!wifi.isBusy() && millis()-lastRequest > POLLPERIOD) {
    sprintf(dataToSend, "b=%d,l=%d,r=%d,s=",sonar[0].ping_cm(),sonar[1].ping_cm(),sonar[2].ping_cm(),sonar[3].ping_cm());
      wifi.sendRequest(GET, domain, 80, path, dataToSend);
        Serial.println(dataToSend);
      lastRequest = millis();
    }
    while(!wifi.hasResponse());
    
    oldState = state;
    state =  getState(wifi.getResponse());
    if(oldState != state){
     // Serial.println("New state is: ");
      // Serial.println(state);
      setMotors();
      }
        

}


/**** HELPER ****/
states getState(String response){
      int beginInd = response.indexOf("~");
      int endInd =response.lastIndexOf("~");
      response = response.substring(beginInd+1, endInd);
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

void clockwise(){
      digitalWrite(digLeft, HIGH); //irrelevant
      digitalWrite(digRight, LOW); //irrelevant
  
      analogWrite(analogLeft, SPEED);
      analogWrite(analogRight, SPEED);
}

void counterclockwise(){
      digitalWrite(digLeft, LOW); //irrelevant
      digitalWrite(digRight, HIGH); //irrelevant
  
      analogWrite(analogLeft, SPEED);
      analogWrite(analogRight, SPEED);
}

void stopmotors(){
      digitalWrite(digLeft, HIGH); //irrelevant
      digitalWrite(digRight, HIGH); //irrelevant
  
      analogWrite(analogLeft, 0);
      analogWrite(analogRight, 0);
}
/*********************************************/









