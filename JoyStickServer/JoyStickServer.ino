#include <Wifi_S08_v2.h>

#define SSID "BLURP"
#define PASSWD "MOMENTUM"
#define BLINKPERIOD 1000
#define UPDATEPERIOD 10

ESP8266 wifi = ESP8266(1,false);
String html = "<html>\n<title>It works!</title>\n<body>\n<h1>Congrats</h1>\n<p>You have successfully interneted.</p>\n</body>\n</html>";
int val = 0;

int light = 0; 
int led = 13;

//pins for joystick control
int UD = 14;
int LR = 15;


int lastupdate = millis();
int blinktime = millis();

char html2[500];

void setup() {
  pinMode(LR, INPUT);
  pinMode(UD, INPUT);

  Serial.begin(115200);
  wifi.begin();
  Serial.println("startserver");
  wifi.startserver(SSID, PASSWD);
  Serial.println("setup complete");
  wifi.setPage("/",html);
}

int UDval;
int LRval;

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println((millis()-lastupdate));
  int m = millis() - lastupdate;
  if(m > UPDATEPERIOD){
    val++;
    UDval = analogRead(UD);
    LRval = analogRead(LR);
    sprintf(html2,"<html>\n<body>\n<h1>Joystick Values</h1>\n<p>Up/Down: ~%d, Left/Right: ~%d`</p>\n</body>\n</html>",UDval, LRval);
    wifi.setPage("/",html2);
    lastupdate = millis();
    Serial.print("Joystick Values: ");
    Serial.print("UD = ");
    Serial.print(UDval);
    Serial.print("LR = ");
    Serial.println(LRval);
  }
  m = millis() - blinktime;
  if(m > BLINKPERIOD){
    if (light == 1){
      Serial.println("blink");
      light = 0;
      digitalWrite(led, LOW);
    }
    else{
      light = 1;
      digitalWrite(led, HIGH);
    }
    blinktime = millis();
  }
}
