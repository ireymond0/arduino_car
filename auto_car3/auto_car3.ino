#include <AFMotor.h>
#include <Servo.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Servo servo2;

int pos = 80;
const int trigPin = 22;
const int echoPin = 23;
const int trigPin2 = 24;
const int echoPin2 = 25;
long duration, duration2;
int distance, distance2;
int currSpeed;
int maxSpeed = 175;
boolean goesForward=false;
int speedSet = 0;

/************************************************************************
 * Use ultrasonic sensor to get the distance to the nearest object in
 *    the front.
 ************************************************************************/
int getFrontObject(){
  //Clears the trigPin
  digitalWrite(trigPin2,LOW);
  delayMicroseconds(2);

  //Set the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin2,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2,LOW);

  //Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echoPin2,HIGH);

  //Calculating the distance in cm
  distance2 = duration2 / 29 / 2;
  return distance2;
}

/************************************************************************
 * Use ultrasonic sensor to get the distance to the nearest object in
 *    180 degrees in front
 ************************************************************************/
int getDistance(){
  //Clears the trigPin
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);

  //Set the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  //Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin,HIGH);

  //Calculating the distance in cm
  distance = duration / 29 / 2;
  return distance;
}

/************************************************************************
 * Check for objects
 * Returns the zone in which the object is
 ************************************************************************/
void checkForObjects(int pos){
  int objDist = getDistance();
  Serial.println(objDist);
  while( (objDist < 70) && (objDist > 40)){
    if(getFrontObject() < 25){
      frontObjectFound();
      break;
    }
    objDist = getDistance();
    if( pos < 80 ){
      motor2.setSpeed(currSpeed/2);
      motor3.setSpeed(currSpeed/2);
    }
    else{
      motor1.setSpeed(currSpeed/2);
      motor4.setSpeed(currSpeed/2);
    }
  }
  setMotorSpeed(currSpeed);
}

/***********************************************************************
 * setup motors
 ***********************************************************************/
void setupMotors(int spd){
  currSpeed = spd;
  motor1.setSpeed(currSpeed);
  motor1.run(RELEASE);
  motor2.setSpeed(currSpeed);
  motor2.run(RELEASE);
  motor3.setSpeed(currSpeed);
  motor3.run(RELEASE);
  motor4.setSpeed(currSpeed);
  motor4.run(RELEASE);
}

void runBack(){
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void runFor(){
  if(!goesForward){
    goesForward=true;
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);     
   for (speedSet = 0; speedSet < currSpeed; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
   {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
   }
   }
  else{
    motor1.run(FORWARD);      
    motor2.run(FORWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD);
  }
}

void motorStop(){
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

int checkLeft(){
  servo2.write(140);
  int ret = getDistance();
  delay(1000);
  return ret;
}
int checkRight(){
  servo2.write(20);
  int ret = getDistance();
  delay(1000);
  return ret;
}

/************************************************************************
 * When objects are very close make some of the wheels go backwards to 
 *  make a hard turn to the left.
 ************************************************************************/
void hardLeft(){
  motor3.run(BACKWARD);     
  motor2.run(BACKWARD);  
  motor1.run(FORWARD);
  motor4.run(FORWARD);   
  delay(500);
  motor1.run(FORWARD);     
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

/************************************************************************
 * When objects are very close make some of the wheels go backwards to 
 *  make a hard turn to the right.
 ************************************************************************/
void hardRight(){
  motor3.run(FORWARD);
  motor2.run(FORWARD);
  motor1.run(BACKWARD);
  motor4.run(BACKWARD);     
  delay(500);
  motor1.run(FORWARD);      
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD); 
}

/************************************************************************
 * change motor speed
 ************************************************************************/
void setMotorSpeed(int sp){
  motor1.setSpeed(sp);
  motor2.setSpeed(sp);
  motor3.setSpeed(sp);
  motor4.setSpeed(sp);
}

void frontObjectFound(){
  int distL = 0, distR = 0;
  distR = checkRight();
  Serial.println("check right");
  distL = checkLeft();
  Serial.println("check left");
  
  if((distL < 25) && (distR < 25)){
    while(getFrontObject() < 25){
      runBack();
    }
    frontObjectFound();
    delay(500);
  }else if(distR > distL){
    Serial.println("test1");
    hardRight();
    motorStop();
    delay(500);
  }else if(distR < distL){
    Serial.println("test2");
    hardLeft();
    motorStop();
    delay(500); 
  }
  else{
    while(getFrontObject() < 25){
      runBack();
    }
    frontObjectFound();
    delay(500);
  }
}

/************************************************************************
 * Setup Arduino
 ************************************************************************/
void setup() {
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);
  Serial.begin(9600);
  servo2.attach(9);
  setupMotors(175);
}

void loop() {
  currSpeed = 175;
  delay(500);
  int dist = getFrontObject();
  if(dist < 25){
    motorStop();
    delay(500);
    frontObjectFound();
  }
  for(pos=80;pos <= 160;pos++){
    servo2.write(pos);
    delay(10);
    checkForObjects(pos);
    runFor();
  }
  for(pos = 80; pos >= 10; pos--){
    servo2.write(pos);
    delay(10);
    checkForObjects(pos);
    runFor();
  }
}
