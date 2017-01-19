int leftDig = 6;
int leftAn = 4;

int rightDig = 7;
int rightAn = 5;

//set values for joystick ranges

int UD_LOW = 250;
int UD_HIGH = 900;
int LR_LOW = 400;
int LR_HIGH = 600;

void setup() {
  Serial.begin(115200);
  Serial.print("start");
  pinMode(leftDig, OUTPUT);
  pinMode(rightDig, OUTPUT);

}

void loop() {
  int UD = analogRead( 14);
  int LR = analogRead(15);
  Serial.print(UD);
  Serial.print(", ");
  Serial.println(LR);
if( (UD > UD_HIGH) &&(LR<LR_HIGH) && (LR > LR_LOW) ) // going forward
    {
    Serial.println("f");
    }

    else if( (LR > LR_HIGH) && (UD<UD_HIGH) && (UD > UD_LOW)) // turn clockwise
    {
   Serial.println("r");
    }

    else if( (UD < UD_LOW ) && (LR<LR_HIGH) && (LR > LR_LOW) ) // going backward
    {
    Serial.println("b");
    }

    else if( (LR < LR_LOW) && (UD<UD_HIGH) && (UD > UD_LOW)) //turn counterclockwise
    {
    Serial.println("l");
    }
delay(100);
}
