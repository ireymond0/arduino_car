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
long duration, duration2;
int distance, distance2;
int currSpeed;
int maxSpeed = 255;

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
  Serial.println(distance2);
  return distance2;
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
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void motorStop(){
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

int checkLeft(){
  servo2.write(140);
  delay(1000);
  servo2.write(80);
}
int checkRight(){
  servo2.write(50);
  delay(1000);
  servo2.write(80);
}

int findGreatest(int one, int two){
  if(one > two){
    return 1;
  }
  else if (one < two){
    return 2;
  }
  else{
    return 3;
  }
}

/************************************************************************
 * When objects are very close make some of the wheels go backwards to 
 *  make a hard turn to the left.
 ************************************************************************/
void hardLeft(){
  setMotorSpeed(maxSpeed);
  motor1.run(FORWARD);
  motor4.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  setMotorSpeed(currSpeed);
}

/************************************************************************
 * When objects are very close make some of the wheels go backwards to 
 *  make a hard turn to the right.
 ************************************************************************/
void hardRight(){
  setMotorSpeed(maxSpeed);
  motor1.run(BACKWARD);
  motor4.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  setMotorSpeed(currSpeed);
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
  setupMotors(200);
}

void loop() {
  if(getFrontObject() < 15){
    while(getFrontObject() < 30){
      int left = checkLeft();
      int right = checkRight();
      int dir = findGreatest(left,right);
      if(dir == 1){
        hardLeft();
      }
      else if(dir == 2){
        hardRight();
      }
      else{
        //dosomething...
      }
      runBack();
    }
  }
  else{
    runFor();
  }
}
