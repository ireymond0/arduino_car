#include <AFMotor.h>
#include <Servo.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Servo servo2;

const int trigPin = 22;
const int echoPin = 23;
const int trigPin2 = 24;
const int echoPin2 = 25;
long duration,duration2;
int distance,distance2;
bool objectInFront = false;
short currSpeed;

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

  if(distance2 <= 100){
    objectInFront = true;
  }else{
    objectInFront = false;
  }
  
  return distance2;
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

/***********************************************************************
 * setup motors
 ***********************************************************************/
void setupMotors(short spd){
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

}
