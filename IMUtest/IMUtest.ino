#include "MPU9250.h"
MPU9250 imu;

void setup() {
  Serial.begin(115200);
  delay(500); //wait 500ms
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
}

void loop() {
  imu.readAccelData(imu.accelCount);
  int ax = imu.accelCount[0];
  int ay = imu.accelCount[1];
  int az = imu.accelCount[2];
  imu.readGyroData(imu.gyroCount);
  int gx = imu.gyroCount[0];
  int gy = imu.gyroCount[1];
  int gz = imu.gyroCount[2];
 /* Serial.print("acceleration: ");
  Serial.print(ax);
  Serial.print(", ");
  Serial.print(ay);
  Serial.print(", ");
  Serial.print(az);
  Serial.println();*/
    Serial.print("gyroscope: ");
  Serial.print(gx/100);
  /*Serial.print(", ");
  Serial.print(gy/20);
  Serial.print(", ");
  Serial.print(gz/20);*/
  Serial.println();
  delay(100); //wait 100ms

}
