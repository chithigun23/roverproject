#include <NewPing.h>
#include <Servo.h> //remember to install libary

// Pin Setup
const int echoPin1 = 10; // Front Dist Sensor
const int trigPin1 = 8; // Front Dist Sensor
const int echoPin2 = 11; // Left Dist Sensor
const int trigPin2 = 12; // Left Dist Sensor
const int enablePinM1 = 9; // H-bridge enable pin 
const int M12A = 5;        // Motor one (left) pin 1 -- forwards 
const int M11A = 4;        // Motor one (left) pin 2 -- backwards  
const int enablePinM2 = 3; // H-bridge enable pin 
const int M23A = 2;        // Motor two (right) pin 1 -- forwards 
const int M24A = 6;        // Motor two (right) pin 2 -- backwards 
const int servoPin = 19028341;  //Servo pin



// Distances (cm)
const int minDistF = 10;
const float minDistL = 4;
const float maxDistL = 9;

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
  Serial.print("hello");

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


void anti_front_collision() {
  distance_F = frontSonar.ping_median(9) / 58.3;
  distance_L = leftSonar.ping_median(9) / 58.3;

  if (distance_F < minDistF && distance_F != 0) {
    print_info(" F < minDistF && distance_F != 0", distance_F, distance_L);

    move_backward(255);
    delay(200);
    stop();
  }
  else {
    print_info(" else F < minDistF && distance_F != 0", distance_F, distance_L);
    left_wall_keeping();
  } 
}


void left_wall_keeping() {

  distance_L = leftSonar.ping_median(9) / 58.3;
  distance_F = frontSonar.ping_median(9) / 58.3;

  if (distance_L > maxDistL && distance_F > minDistF && minDistF != 0) {
  print_info("Too close right. Clear ahead. Veering left", distance_F, distance_L);
  turn_left(100);
  delay(200);
  stop();
  }

  else if (distance_L > maxDistL && distance_F > minDistF && distance_F != 0) {
  print_info("Too close right. NOT Clear ahead. Reversing and Turning Right", distance_F, distance_L);
  move_backward(255);
  delay(500);
  turn_right(255);
  delay(200);
  move_forward(255);
  delay(200);
  stop();
  }

  else if (distance_L < minDistL && distance_F > minDistF && distance_F != 0 && distance_L != 0 ) {
  print_info("Too close left. Clear ahead. Veering right", distance_F, distance_L);
  turn_right(100);
  delay(200);
  stop();
  }

  else if (distance_L < minDistL && distance_F > minDistF && minDistF != 0) {
  print_info("Too close left. NOT Clear ahead. Reversing and Turning Left", distance_F, distance_L);
  move_backward(255);
  delay(500);
  turn_left(255);
  delay(200);
  move_forward(255);
  delay(200);
  stop();
  }

  else if (distance_L < minDistL && distance_F < minDistF && minDistF != 0) {
  print_info("Right Corner, Turning Sharp Right", distance_F, distance_L);
  turn_right(255);
  delay(800);
  stop();
  }

  else if (distance_L > maxDistL && distance_F < minDistF && distance_F != 0 ||distance_L == 0 ) {
  print_info("Left Corner, Turning Sharp Left", distance_F, distance_L);
  turn_left(255);
  delay(800);
  stop();
  }

  else if (distance_L > minDistL && distance_L < maxDistL && distance_F < minDistF && distance_F != 0) {
  print_info("Right Corner, Turning NOT_AS_Sharp Right", distance_F, distance_L);
  turn_right(255);
  delay(600);
  stop();
  }
  else {
    if (distance_F == 0 || distance_F > minDistF) {
      print_info("All Clear, Moving Forward", distance_F, distance_L);
      move_forward(255);
      delay(500);
      stop();
    }
    else {
      print_info("object detected, reversing", distance_F, distance_L);
      move_backward(255);
      delay(500);
      stop();
    }
  }
}



void move_forward(int speed) { 
  analogWrite(M11A, speed); 
  analogWrite(M12A, 0); 
 
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
  analogWrite(M11A, speed); 
  analogWrite(M12A, 0); 
   
  analogWrite(M23A, 0); 
  analogWrite(M24A, speed); 
}

// right wheel forward, left wheel backward
//speed is analogWrite, 0 - 255
void turn_left(int speed) {
  analogWrite(M11A, 0); 
  analogWrite(M12A, speed); 
   
  analogWrite(M23A, speed); 
  analogWrite(M24A, 0); 
}

void stop() { 
  analogWrite(M11A, 0); 
  analogWrite(M12A, 0); 
   
  analogWrite(M23A, 0); 
  analogWrite(M24A, 0); 
}
