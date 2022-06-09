#include <Wire.h>
#include <Servo.h>

float rawAccX, rawAccY, rawAccZ;
float yaw, pitch, roll;
float rawGyroX, rawGyroY, rawGyroZ;
float deltaT, currentT, pastT;

int yawPin = 7;
int pitchPin = 8;
Servo yawServo, pitchServo;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);  
  Wire.beginTransmission(0x68); 
  Wire.write(0x1C); //configure accelerometer
  Wire.write(0x10);
  Wire.write(0x1B); //configure gyroscope       
  Wire.write(0x10); 
  Wire.endTransmission(true); 
  yawServo.attach(yawPin);
  pitchServo.attach(pitchPin);
  delay(100);
} 

void loop() {
  // put your main code here, to run repeatedly:
  //accelorometer data
  Wire.beginTransmission(0x68); //I2C address of the MPU
  Wire.write(0x3B); //starting accelerometer data
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6, true); 
  rawAccX = (Wire.read()<<8|Wire.read())/16384.0;
  rawAccY = (Wire.read()<<8|Wire.read())/16384.0; 
  rawAccZ = (Wire.read()<<8|Wire.read())/16384.0; 
  
  //calc time to convert from deg/s to deg
  pastT = currentT;
  currentT = millis();
  deltaT = (currentT - pastT)/1000;
  

  //gyroscope data
  Wire.beginTransmission(0x68);
  Wire.write(0x43); //starting gyro data
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6, true);
  rawGyroX = (Wire.read()<<8|Wire.read())/ 131.0;
  rawGyroY = (Wire.read()<<8|Wire.read())/ 131.0;
  rawGyroZ = (Wire.read()<<8|Wire.read())/ 131.0;
  yaw = yaw + rawGyroZ*deltaT;
  pitch = (atan(-1 * rawAccX / sqrt(pow(rawAccY, 2) + pow(rawAccZ, 2))) * 180 / PI) + rawGyroY*deltaT;
  roll = (atan(rawAccY / sqrt(pow(rawAccX, 2) + pow(rawAccZ, 2))) * 180 / PI) + rawGyroX*deltaT;
  
  
  
  Serial.print("Yaw: ");
  Serial.print(yaw);
  Serial.print(" pitch: ");
  Serial.print(pitch);
  Serial.print(" roll: ");
  Serial.println(roll);
  yawServo.write(yaw);
  pitchServo.write(pitch);
  
  
}
