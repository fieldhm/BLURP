int digLeft = 14;
int analogLeft = 21;
int digRight = 15;
int analogRight = 20;


int SPEED = 60;

void setup() {
  Serial.begin(115200);
  Serial.print("start");
  pinMode(digLeft, OUTPUT);
  pinMode(digRight, OUTPUT);

}

void loop() {
 /* int UD = analogRead(14);
  int LR = analogRead(15);
  Serial.print(UD);
  Serial.print(", ");
  Serial.println(LR); */

    digitalWrite(digLeft, LOW);
    digitalWrite(digRight, HIGH);

    analogWrite(analogLeft, SPEED);
    analogWrite(analogRight, SPEED);
  
/*if( (UD > 750) &&(LR<600) && (LR > 400) ) // going forward
    {
    Serial.println("f");
    }

    else if( (LR > 750) && (UD<600) && (UD > 400)) // turn clockwise
    {
   Serial.println("r");
    }

    else if( (UD < 250 ) && (LR<600) && (LR > 400) ) // going backward
    {
    Serial.println("b");
    }

    else if( (LR < 250) && (UD<600) && (UD > 400)) //turn counterclockwise
    {
    Serial.println("l");
    }*/
delay(100);
}
