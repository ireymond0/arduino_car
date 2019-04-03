#include <AFMotor.h>
#include <Servo.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Servo servo2;

const int trigPin = 22;
const int echoPin = 23;
int pos = 40;

// defines variables
long duration;
int distance;
int currSpeed;

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
 * Use ultrasonic sensor to get the distance to the nearest object.
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

  //Calculating the distance
  distance = duration*0.034/2;

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
      motor1.setSpeed(currSpeed*2);
      motor4.setSpeed(currSpeed*2);
    }
    else{
      motor2.setSpeed(currSpeed*2);
      motor3.setSpeed(currSpeed*2);
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
    motor1.run(FORWARD);
    motor1.setSpeed(currSpeed/2);
    motor4.run(FORWARD);
    motor4.setSpeed(currSpeed/2);
    motor2.run(BACKWARD);
//    motor2.setSpeed(currSpeed*2);
    motor3.run(BACKWARD);
//    motor3.setSpeed(currSpeed*2);
}

/************************************************************************
 * When objects are very close make some of the wheels go backwards to 
 *  make a hard turn to the right.
 ************************************************************************/
void hardRight(){
    motor1.run(BACKWARD);
//    motor1.setSpeed(currSpeed*2);
    motor4.run(BACKWARD);
//    motor4.setSpeed(currSpeed*2);
    motor2.run(FORWARD);
    motor2.setSpeed(currSpeed/2);
    motor3.run(FORWARD);
    motor3.setSpeed(currSpeed/2);
}

/************************************************************************
 * Setup Arduino
 ************************************************************************/
void setup() {
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(9600);
  servo2.attach(9);
  setMotorSpeed(50);
}

/************************************************************************
 * Main Routine
 ************************************************************************/
void loop() {
  currSpeed = 250;

  setMotorSpeed(currSpeed);
  /*
   * - Start with the servo pointing straight ahead (pos = 70).
   * - Start to sweep servo to the left and check with every step if something is in the way.
   */
  servo2.write(80);
  delay(100);
  for( pos=80; pos <= 130; pos++){
    servo2.write(pos);
    delay(10);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    checkForObjects(pos);
    
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
    checkForObjects(pos);
    
  }
}
