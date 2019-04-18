#include <AFMotor.h>

// Define the 4 motors
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// Define the pins for the three ultrasonic sensors
const int trigPinF = 22;
const int trigPinR = 24;
const int trigPinL = 26;
const int echoPinF = 23;
const int echoPinR = 25;
const int echoPinL = 27;
const int ledPinF = 28;
const int ledPinR = 29;
const int ledPinL = 30;

// defines variables used to detect obstacles
long durationF, durationR, durationL;
int distanceF, distanceR, distanceL;
bool objectF, objectR, ObjectL;

// Current speed of the motors
int currSpeed;

// Maximum speed of the motors
// Could be replaced by a potentiometer in the future
int maxSpeed = 200;

/************************************************************************
 * change motor speed
 ************************************************************************/
void setMotorSpeed(int sp){
  motor1.setSpeed(sp);
  motor2.setSpeed(sp);
  motor3.setSpeed(sp);
  motor4.setSpeed(sp);
  currSpeed = sp;
}

/************************************************************************
 * Check for obstacles. Set distance and light LEDs
 ************************************************************************/
void checkForObstacles(){
  //Clears the trigPin
  digitalWrite(trigPinF,LOW);
  digitalWrite(trigPinR,LOW);
  digitalWrite(trigPinL,LOW);
  delayMicroseconds(2);

  //Set the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPinF,HIGH);
  digitalWrite(trigPinR,HIGH);
  digitalWrite(trigPinL,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinF,LOW);
  digitalWrite(trigPinR,LOW);
  digitalWrite(trigPinL,LOW);

  //Reads the echoPin, returns the sound wave travel time in microseconds
  durationF = pulseIn(echoPinF,HIGH);
  durationR = pulseIn(echoPinR,HIGH);
  durationL = pulseIn(echoPinL,HIGH);

  //Calculating the distance in cm and set boolean
  distanceF = durationF / 29 / 2;
  if(distanceF <= 100){
    objectF = true;
  }
  else{
    objectF = false;
  }
  distanceR = durationR / 29 / 2;
  if(distanceR <= 100){
    objectR = true;
  }
  else{
    objectR = false;
  }
  distanceL = durationL / 29 / 2;
  if(distanceL <= 100){
    objectL = true;
  }
  else{
    objectL = false;
  }

  lightLEDs();
}

/************************************************************************
 * LED control
 ************************************************************************/
void lightLEDs(){
  if(objectF){
    digitalWrite(ledPinF, HIGH);
  }
  else{
    digitalWrite(ledPinF, LOW);
  }
  if(objectR){
    digitalWrite(ledPinR, HIGH);
  }
  else{
    digitalWrite(ledPinR, LOW);
  }
  if(objectL){
    digitalWrite(ledPinL, HIGH);
  }
  else{
    digitalWrite(ledPinL, LOW);
  }
}

/************************************************************************
 * Setup Arduino
 ************************************************************************/
void setup() {
  pinMode(trigPinF,OUTPUT);
  pinMode(echoPinF,INPUT);
  pinMode(trigPinR,OUTPUT);
  pinMode(echoPinR,INPUT);
  pinMode(trigPinL,OUTPUT);
  pinMode(echoPinL,INPUT);
  pinMode(ledPinF,OUTPUT);
  pinMode(ledPinR,OUTPUT);
  pinMode(ledPinL,OUTPUT);
  Serial.begin(9600);
  setMotorSpeed(150);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkForObstacles();
}
