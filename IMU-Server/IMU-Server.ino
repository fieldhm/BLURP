/* Server side computation of direcitons */


#include <Wifi_S08_v2.h>
#include "MPU9250.h"
MPU9250 imu;

#define SSID "BLURP"
#define PASSWD "MOMENTUM"
#define UPDATEPERIOD 10



//variables needed for getting and computing direction
int UDval;
int LRval;
float SCALE = 500;
int HIT_VAL = 40;
int UD_LOW = -1*HIT_VAL;
int UD_HIGH = HIT_VAL;
int LR_LOW = -1*HIT_VAL;
int LR_HIGH = HIT_VAL;

bool forward, backward, counterclockwise, clockwise;
//************************

ESP8266 wifi = ESP8266(1,false);
String html = "<html>\n<title>It works!</title>\n<body>\n<h1>Congrats</h1>\n<p>You have successfully interneted.</p>\n</body>\n</html>";
int val = 0;

int light = 0; 
int led = 13;

int lastupdate = millis();
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

char getDirectionValue[500];

void loop() {

  int m = millis() - lastupdate;
  if(m > UPDATEPERIOD){
    val++;

    strcpy(getDirectionValue,getDirection());
    sprintf(html2,"<html>\n<body>\n<p>%s</p>\n</body>\n</html>", getDirectionValue);
    wifi.setPage("/",html2);
    Serial.println(getDirectionValue);
    lastupdate = millis();
  //Serial.println(wifi.getData());
  }
}

char* getDirection(){

     char boolValues[500]; 
     
     imu.readGyroData(imu.gyroCount);
     UDval = imu.gyroCount[0];
     UDval = (-1)*(int)UDval/SCALE;
     LRval = imu.gyroCount[2];
     LRval = (-1)*(int)LRval/SCALE;

     forward = (UDval > UD_HIGH) &&(LRval<LR_HIGH) && (LRval > LR_LOW) ;
     clockwise = (LRval > LR_HIGH) && (UDval<UD_HIGH) && (UDval > UD_LOW);
     backward = (UDval < UD_LOW ) && (LRval<LR_HIGH) && (LRval > LR_LOW);
     counterclockwise = (LRval < LR_LOW) && (UDval<UD_HIGH) && (UDval > UD_LOW);

    // sprintf(boolValues, "BoolValuesf=%d,b=%d,cw=%d,ccw=%d", forward, backward, clockwise, counterclockwise);
     sprintf(boolValues, "~%d%d%d%d~", forward, backward, clockwise, counterclockwise);
     return boolValues;
}

