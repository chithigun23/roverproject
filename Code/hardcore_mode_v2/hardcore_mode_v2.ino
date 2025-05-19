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

Servo myservo; //servo object to control servo
// store servo position
int servo_pos = 0;

NewPing frontSonar(trigPin1,echoPin1,200);
NewPing leftSonar(trigPin2,echoPin2,200);

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
} 
 
void loop() {
  move_forward(255);
  delay(2500);
  stop();
  delay(200);
  turn_right(255);
  delay(970);
  stop();
  delay(200);
  move_forward(255);
  delay(3500);
  stop();
  delay(200);
  turn_left(255);
  delay(970);
  stop();
  delay(200);
  move_forward(255);
  delay(3500);
  stop();
  delay(200);
  turn_right(255);
  delay(970);
  stop();
  move_forward(255);
  delay(3500);
  stop();
  delay(200);
  turn_right(255);
  delay(970);
  stop();
  delay(200);
  while(true){
    move_forward(255);
  }
  
}

void move_forward(int speed) { 
  analogWrite(M11A, 0); 
  analogWrite(M12A, speed); 
 
  analogWrite(M23A, speed); 
  analogWrite(M24A, 0); 
} 

void move_backward(int speed) { 
  analogWrite(M11A, 0); 
  analogWrite(M12A, speed); 
 
  analogWrite(M23A, 0); 
  analogWrite(M24A, speed); 
} 

// left wheel forward, right wheel backward
//speed is analogWrite, 0 - 255
void turn_right(int speed) {
  analogWrite(M11A, 0); 
  analogWrite(M12A, speed); 
   
  analogWrite(M23A, 0); 
  analogWrite(M24A, speed); 
}

// right wheel forward, left wheel backward
//speed is analogWrite, 0 - 255
void turn_left(int speed) {
  analogWrite(M11A, speed); 
  analogWrite(M12A, 0); 
   
  analogWrite(M23A, speed); 
  analogWrite(M24A, 0); 
}

void stop() { 
  analogWrite(M11A, 0); 
  analogWrite(M12A, 0); 
   
  analogWrite(M23A, 0); 
  analogWrite(M24A, 0); 
}