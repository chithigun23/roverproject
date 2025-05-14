#include <NewPing.h>
#include <Servo.h> //remember to install libary

// Pin Setup
const int echoPin1 = 12; // Front Dist Sensor
const int trigPin1 = 11; // Front Dist Sensor
const int echoPin2 = 7; // Left Dist Sensor
const int trigPin2 = 8; // Left Dist Sensor
const int enablePinM1 = 9; // H-bridge enable pin 
const int M12A = 5;        // Motor one (left) pin 1 -- forwards 
const int M11A = 4;        // Motor one (left) pin 2 -- backwards  
const int enablePinM2 = 3; // H-bridge enable pin 
const int M23A = 2;        // Motor two (right) pin 1 -- forwards 
const int M24A = 6;        // Motor two (right) pin 2 -- backwards 
const int servoPin = 10;  //Servo pin

// Distances (cm)
const int minDistF = 5;
const float minDistL = 3;
const float maxDistL = 6.5;

// keep track of distances 
float distance_F;  
float distance_L; 

float leftOption;
float rightOption;

Servo myservo; //servo object to control servo
// store servo position
int servo_pos = 0;

NewPing frontSonar(trigPin1,echoPin1,200);
NewPing leftSonar(trigPin2,echoPin2,200);

bool checking_sides = false;

void setup() { 
  Serial.begin(9600); 
 
  // enable sonar1 pins   
  pinMode(trigPin1, OUTPUT); 
  pinMode(echoPin1, INPUT); 
 
  // enable sonar2 pins 
  pinMode(trigPin2, OUTPUT); 
  pinMode(echoPin2, INPUT); 
     
  // enable motor 1 and its pins 
  pinMode(enablePinM1, OUTPUT); 
  pinMode(M12A, OUTPUT); 
  pinMode(M11A, OUTPUT); 
  analogWrite(enablePinM1, 255); 
 
  // enable motor 2 and its pins 
  pinMode(enablePinM2, OUTPUT); 
  pinMode(M23A, OUTPUT); 
  pinMode(M24A, OUTPUT); 
  analogWrite(enablePinM2, 255); 

  myservo.attach(servoPin);
  myservo.write(90);
} 
 
void loop() {
  distance_F = frontSonar.ping_median(9) / 58.3;
  distance_L = leftSonar.ping_median(9) / 58.3;

  Serial.println(distance_F);
  delay(1000);
  move_forward(255);



  //If distance_F < 10 then stop 
  if (distance_F < 10 && distance_F != 0){
    
    stop();
    myservo.write(180);
    delay(1000);
    leftOption = readFront();
    delay(1000);
    myservo.write(0);
    delay(1000);
    rightOption = readFront();
    delay(1000);
    myservo.write(90);
    delay(1000);

    if(abs(leftOption - rightOption) < 2) { //This is the condition where left and right options are about the same length
      turn_left(600); 
      while(true){
        stop();
      }

    } 

    else if (leftOption > rightOption) {  //Its not the same distance, and theres more room to the left. Thus turn left.
      turn_left(600); 
      while(true){
        stop();
      }
    } 

    else{                                 //Its not the same distance, and theres more room to the right. Thus turn right.
      turn_right(600);
      while(true){
        stop();
      }
    }


    


  }

  

}

void print_info(String desc, float dist_F, float dist_L) {
  Serial.print(desc);
  Serial.print(": distance_F = ");
  Serial.print(dist_F);
  Serial.print(", distance_L = ");
  Serial.println(dist_L);
}

void swerve_servo() {
  for (servo_pos = 0; servo_pos <= 180; servo_pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(servo_pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  for (servo_pos = 180; servo_pos >= 0; servo_pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(servo_pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position

  }
}
//swerve_servo references the swerve example in (https://docs.arduino.cc/learn/electronics/servo-motors/)




void move_forward(int time) { 
  analogWrite(M11A, 0); 
  analogWrite(M12A, 255); 
 
  analogWrite(M23A, 255); 
  analogWrite(M24A, 0); 
} 

void move_backward(int speed) { 
  analogWrite(M11A, 255); 
  analogWrite(M12A, 0); 
 
  analogWrite(M23A, 0); 
  analogWrite(M24A, 255); 
} 


void turn_right(int time) {
  analogWrite(M11A, 0); 
  analogWrite(M12A, 255); 
   
  analogWrite(M23A, 0); 
  analogWrite(M24A, 255); 

  delay(time);
  stop();
}


void turn_left(int time) {
  analogWrite(M11A, 255); 
  analogWrite(M12A, 0); 
   
  analogWrite(M23A, 255); 
  analogWrite(M24A, 0); 

  delay(time);
  stop();
}

void stop() { 
  analogWrite(M11A, 0); 
  analogWrite(M12A, 0); 
   
  analogWrite(M23A, 0); 
  analogWrite(M24A, 0); 
}

float readFront(){
  return frontSonar.ping_median(9) / 58.3;
}

float readLeft(){
  return leftSonar.ping_median(9) / 58.3;
} 




