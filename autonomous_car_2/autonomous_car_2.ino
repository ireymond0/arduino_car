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
int pos = 40;

// defines variables
long duration, duration2;
int distance, distance2;
int currSpeed;
int maxSpeed = 175;
/************************************************************************
 * Keep track of objects in the way
 ************************************************************************/
struct obstacle{
  int angle;
  int distance;
};

/************************************************************************
 * Create a new struct to keep track of where the object is... not sure I need that yet
 ************************************************************************/
obstacle obs;

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

/***********************************************************************
 * setup motors
 ***********************************************************************/
void setupMotors(){
  motor1.setSpeed(100);
  motor1.run(RELEASE);
  motor2.setSpeed(100);
  motor2.run(RELEASE);
  motor3.setSpeed(100);
  motor3.run(RELEASE);
  motor4.setSpeed(100);
  motor4.run(RELEASE);
  currSpeed = 100;
}

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
 * Check for objects
 * Returns the zone in which the object is
 ************************************************************************/
void checkForObjects(int pos){
  
  obs.angle = pos;
  int objDist = getDistance();
 Serial.println(objDist);
  while( (objDist < 70) && (objDist > 40)){
    objDist = getDistance();
    if( pos < 70 ){
      motor2.setSpeed(currSpeed/2);
      motor3.setSpeed(currSpeed/2);
    }
    else{
      motor1.setSpeed(currSpeed/2);
      motor4.setSpeed(currSpeed/2);
    }
  }
  while(objDist <= 40){
    objDist = getDistance();
    if(pos < 70){
      hardLeft();
    }
    else{
      hardRight();
    }
  }
 
  obs.distance = objDist;
  setMotorSpeed(currSpeed);
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
 * Back up
 ************************************************************************/
void runBack(){
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

/************************************************************************
 * Setup Arduino
 ************************************************************************/
void setup() {
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(9600);
  servo2.attach(9);
  setMotorSpeed(200);
}

/************************************************************************
 * Main Routine
 ************************************************************************/
void loop() {
  currSpeed = 175;

  setMotorSpeed(currSpeed);
  /*
   * - Start with the servo pointing straight ahead (pos = 80 in this case).
   * - Start to sweep servo to the left and check with every step if something is in the way.
   */
  servo2.write(80);
  delay(100);
  Serial.println(getFrontObject());
  int front = getFrontObject();

  /*
   * Make sure not to crash into objects in the front while the sweep is on the right or left
   */
  for( pos=80; pos <= 160; pos++){
    servo2.write(pos);
    delay(10);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    if(pos >90){
      checkForObjects(pos);
    }
  }
  
  /*
   * - Do the same as above but going to the left (always start from the middle.
   */
  servo2.write(80);
  delay(100);
  for( pos = 80; pos >= 10; pos--){
    servo2.write(pos);
    delay(10);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    if(pos < 60){
      checkForObjects(pos);
    }
    
  }
}
