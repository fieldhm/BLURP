#include <NewPing.h>

/* Server side computation of direcitons */


#include <Wifi_S08_v2.h>
#include "MPU9250.h"
MPU9250 imu;

#define SSID "BLURP"
#define PASSWD "MOMENTUM"
#define UPDATEPERIOD 10
#define STATEPERIOD 1000 //how long to wait after state change before getting new measurment



//variables needed for getting and computing direction
int UDval;
int LRval;
float SCALE = 500;
int HIT_VAL = 40;
int UD_LOW = -1*HIT_VAL;
int UD_HIGH = HIT_VAL;
int LR_LOW = -1*HIT_VAL;
int LR_HIGH = HIT_VAL;

//************************


/***** enum setup ******/
typedef enum { STOP, FW, BW, CW, CCW } states;
//initialize state variable
states state = STOP;

char stateChar[10] = "STOP";

typedef enum { none, fw, bw, cw, ccw } instructs;

instructs instruct = none;

typedef enum { OUTOFRANGE, FAR, KINDAFAR, MEDIUM, CLOSE } distances;

distances left_distance = OUTOFRANGE;
distances right_distance = OUTOFRANGE;
distances back_distance = OUTOFRANGE;
/********************/


ESP8266 wifi = ESP8266(1,false);
String html = "<html>\n<title>It works!</title>\n<body>\n<h1>Congrats</h1>\n<p>You have successfully interneted.</p>\n</body>\n</html>";
int val = 0;

int light = 0; 
int led = 13;

int lastupdate = millis();
int laststateupdate = millis();
int blinktime = millis();

char html2[500];

void setup() {
  Serial.begin(115200);
  
/****** Wifi Setup*****************/
  wifi.begin();
  Serial.println("startserver");
  wifi.startserver(SSID, PASSWD);
  Serial.println("setup complete");
  wifi.setPage("/",html);
/********************************/

  /**************** IMU Setup***********************/
  Wire.begin();
  byte c = imu.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
  Serial.print(" I should be "); Serial.println(0x73, HEX);
  if (c == 0x71) // WHO_AM_I   //IF YOU HAVE A BLUE IMU CHANGE TO 0x73
  {
    Serial.println("MPU9250 is online...");
       Serial.println("MPU9250 is online...");
    // Calibrate gyro and accelerometers, load biases in bias registers
    imu.calibrateMPU9250(imu.gyroBias, imu.accelBias);
    } // if (c == 0x73)
  else
  {
    Serial.print("Could not connect to MPU9250: 0x");
    Serial.println(c, HEX);
    while(1) ; // Loop forever if communication doesn't happen
  }
  /*****************************************/
}

states oldState = STOP;

void loop() {

  int m = millis() - lastupdate;
  int n = millis() - laststateupdate;
  
  if(m > UPDATEPERIOD && n > STATEPERIOD ) //update period counts last time since state changed
  {
    val++;
    updateState();
    lastupdate = millis();
    if (oldState != state)// state changed so should update
    {
      laststateupdate = millis();
      sprintf(html2,"<html>\n<title>Current State</title>\n<body>\n<p>~%s~</p>\n</body>\n</html>", stateChar);
      wifi.setPage("/",html2);
      oldState = state;
      Serial.print("Current state: ");
      Serial.println(stateChar);
      switch (oldState) {
        case FW:
          to_forward();
          break;
        case BW:
          to_backward();
          break;
        case CW:
          to_clockwise();
          break;
        case CCW:
          to_counterclockwise();
          break;
        case STOP:
          to_stop();
          break;
        default:
          break;
      }
    }
   wifi.setPage("/",html2); //why does this have to be here????
  //Serial.println(wifi.getData());
  }
  else {
    getUSstate_R();
    switch (right_distance) {
      case FAR:
        far_vibrate(21);
        break;
      case KINDAFAR:
        kinda_far_vibrate(21);
        break;
      case MEDIUM:
        medium_vibrate(21);
        break;
      case CLOSE:
        close_vibrate(21);
        break;
      default:
        break;
    }
    getUSstate_B();
    switch (back_distance) {
      case FAR:
        far_vibrate(5);
        break;
      case KINDAFAR:
        kinda_far_vibrate(5);
        break;
      case MEDIUM:
        medium_vibrate(5);
        break;
      case CLOSE:
        close_vibrate(5);
        break;
      default:
        break;
    }
    getUSstate_L();
    switch (left_distance) {
      case FAR:
        far_vibrate(22);
        break;
      case KINDAFAR:
        kinda_far_vibrate(22);
        break;
      case MEDIUM:
        medium_vibrate(22);
        break;
      case CLOSE:
        close_vibrate(22);
        break;
      default:
        break;
    }
  }
}

void far_vibrate(motorPin pin){
  digitalWrite(pin, 100);
  delay(500);
  digitalWrite(pin, LOW);
  delay(1000);
}

void kinda_far_vibrate(motorPin pin){
  digitalWrite(pin, 130);
  delay(400);
  digitalWrite(pin, LOW);
  delay(600);
}

void medium_vibrate(motorPin pin){
  digitalWrite(pin, 160);
  delay(300);
  digitalWrite(pin, LOW);
  delay(300);
}

void close_vibrate(motorPin pin){
  digitalWrite(pin, 200);
  delay(200);
  digitalWrite(pin, LOW);
  delay(100);
}

void to_forward(){
  digitalWrite(20, 150);
  digitalWrite(23, 150);
  delay(100);
  digitalWrite(20, LOW);
  digitalWrite(23, LOW);
  delay(100);
  digitalWrite(20, 150);
  digitalWrite(23, 150);
  delay(100);
  digitalWrite(20, LOW);
  digitalWrite(23, LOW);
}

void to_backward(){
  digitalWrite(5, 150);
  delay(100);
  digitalWrite(5, LOW);
  delay(100);
  digitalWrite(5, 150);
  delay(100);
  digitalWrite(5, LOW);
}

void to_clockwise(){
  digitalWrite(21, 150);
  digitalWrite(20, 150);
  delay(100);
  digitalWrite(21, LOW);
  digitalWrite(20, LOW);
  delay(100);
  digitalWrite(21, 150);
  digitalWrite(20, 150);
  delay(100);
  digitalWrite(21, LOW);
  digitalWrite(20, LOW);
}

void to_counterclockwise(){
  digitalWrite(22, 150);
  digitalWrite(23, 150);
  delay(100);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  delay(100);
  digitalWrite(22, 150);
  digitalWrite(23, 150);
  delay(100);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
}

void to_stop(){
  digitalWrite(20, 150);
  digitalWrite(21, 150);
  digitalWrite(22, 150);
  digitalWrite(23, 150);
  digitalWrite(5, 150);
  delay(100);
  digitalWrite(20, LOW);
  digitalWrite(21, LOW);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(5, LOW);
  delay(100);
  digitalWrite(20, 150);
  digitalWrite(21, 150);
  digitalWrite(22, 150);
  digitalWrite(23, 150);
  digitalWrite(5, 150);
  delay(100);
  digitalWrite(20, LOW);
  digitalWrite(21, LOW);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(5, LOW);
}

void getUSstate_R(){
  String rightSensorData = wifi.getData();
  int rposition = rightSensorData.indexOf('r');
  int USval = rightSensorData.substring(rposition + 2).toInt();

  if ( (USval < 300) && (USval >= 80) ) right_distance = FAR;
  else if ( (USval < 80) && (USval >= 20) ) right_distance = KINDAFAR;
  else if ( (USval < 20) && (USval >= 8) ) right_distance = MEDIUM;
  else if ( (USval < 8) && (USval >= 0) ) right_distance = CLOSE;
  else right_distance = OUTOFRANGE;
}

void getUSstate_B(){
  String backSensorData = wifi.getData();
  int lposition = backSensorData.indexOf('l');
  int USval = backSensorData.substring(2, (lposition - 2));

  if ( (USval < 300) && (USval >= 80) ) back_distance = FAR;
  else if ( (USval < 80) && (USval >= 20) ) back_distance = KINDAFAR;
  else if ( (USval < 20) && (USval >= 7) ) back_distance = MEDIUM;
  else if ( (USval < 7) && (USval >= 0) ) back_distance = CLOSE;
  else back_distance = OUTOFRANGE;
}

void getUSstate_L(){
  String leftSensorData = wifi.getData();
  int lposition = leftSensorData.indexOf('l');
  int rposition = leftSensorData.indexOf('r');
  int USval = leftSensorData.substring((lposition + 2), (rposition - 2));
  
  if ( (USval < 300) && (USval >= 80) ) left_distance = FAR;
  else if ( (USval < 80) && (USval >= 20) ) left_distance = KINDAFAR;
  else if ( (USval < 20) && (USval >= 8) ) left_distance = MEDIUM;
  else if ( (USval < 8) && (USval >= 0) ) left_distance = CLOSE;
  else left_distance = OUTOFRANGE;
}

void getIRstate(){
  //fill in with code to get infrared data
}

void getInstruction(){
       
     imu.readGyroData(imu.gyroCount);
     UDval = imu.gyroCount[0];
     UDval = (-1)*(int)UDval/SCALE;
     LRval = imu.gyroCount[2];
     LRval = (-1)*(int)LRval/SCALE;

     if ( (UDval > UD_HIGH) &&(LRval<LR_HIGH) && (LRval > LR_LOW) ) instruct = fw;
     else if ( (LRval > LR_HIGH) && (UDval<UD_HIGH) && (UDval > UD_LOW)) instruct = cw;
     else if  ( (UDval < UD_LOW ) && (LRval<LR_HIGH) && (LRval > LR_LOW)) instruct = bw;
     else if ((LRval < LR_LOW) && (UDval<UD_HIGH) && (UDval > UD_LOW)) instruct = ccw;
     else instruct = none;
}

states updateState(){
  
  getInstruction();
  
  switch (state) {
    case STOP:
      switch (instruct) {
        case fw:
          state = FW;
          break;
        case bw:
          state = BW;
          break;
        case cw:
          state = CW;
          break;
        case ccw:
          state = CCW;
          break;
        default:
          break;
      }
      break;
    case FW:
      switch (instruct) {
        case bw:
          state = STOP;
          break;
        default:
          break;
      }
      break;
    case BW:
      switch (instruct) {
        case fw:
          state = STOP;
          break;
        default:
          break;
      }
      break;
    case CW:
      switch (instruct) {
        case ccw:
          state = STOP;
          break;
        default:
          break;
      }
      break;
    case CCW:
      switch (instruct) {
        case cw:
          state = STOP;
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }   
  stateToChar();  
}

void stateToChar(){
  switch(state){
    case FW:
      strcpy(stateChar,"FW");
      break;
    case BW:
      strcpy(stateChar,"BW");
      break;
    case CW:
      strcpy(stateChar,"CW");
      break;
    case CCW:
      strcpy(stateChar,"CCW");
      break;
    case STOP:
      strcpy(stateChar,"STOP");
      break;
  }



  
}

