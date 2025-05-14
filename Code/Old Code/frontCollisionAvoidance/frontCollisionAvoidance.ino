#include <NewPing.h>
/////////////////////////////////////
// Pin Setup
/////////////////////////////////////

// Minimum Distance
const int minDist = 20;

// Front Dist Sensor
const int echoPin1 = 10; 
const int trigPin1 = 8; 

NewPing frontSonar(trigPin1,echoPin1,200);

// Left Dist Sensor
const int echoPin2 = 11; 
const int trigPin2 = 12; 
 
NewPing leftSonar(trigPin2,echoPin2,200);


// motor 1 pins (left wheel) 
const int enablePinM1 = 9; // H-bridge enable pin 
const int M12A = 5;        // Motor one pin 1 -- forwards 
const int M11A = 4;        // Motor one pin 2 -- backwards 
 
// motor 2 pins (right wheel) 
const int enablePinM2 = 3; // H-bridge enable pin 
const int M23A = 2;        // Motor two pin 1 -- forwards 
const int M24A = 6;        // Motor two pin 2 -- backwards 
 
// keep track of distances 
float distance_F;  
float distance_L; 

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
  digitalWrite(enablePinM1, HIGH); 
 
  // enable motor 2 and its pins 
  pinMode(enablePinM2, OUTPUT); 
  pinMode(M23A, OUTPUT); 
  pinMode(M24A, OUTPUT); 
  analogWrite(enablePinM2, 190); 
} 
 
void loop() {
  // Get front distance using NewPing
  distance_F = frontSonar.ping_median(9) / 58.3;
  Serial.print("Front Distance: ");
  Serial.print(distance_F);
  Serial.println(" cm");

  if (distance_F == 0 || distance_F > minDist) {
    move_forward();
  } 
  else {
    stop();
    Serial.println("Wall too close. Stopping.");
    while (true); // Stop forever
  }

  delay(100); // Small delay to avoid flooding sensor
}

void move_forward() { 
  digitalWrite(M11A, HIGH); 
  digitalWrite(M12A, LOW); 
 
  digitalWrite(M23A, HIGH); 
  digitalWrite(M24A, LOW); 
} 

void move_backward() { 
  digitalWrite(M11A, LOW); 
  digitalWrite(M12A, HIGH); 
 
  digitalWrite(M23A, LOW); 
  digitalWrite(M24A, HIGH); 
} 

// left wheel forward, right wheel backward
void turn_right() {
  digitalWrite(M11A, HIGH); 
  digitalWrite(M12A, LOW); 
   
  digitalWrite(M23A, LOW); 
  digitalWrite(M24A, HIGH); 
}

// right wheel forward, left wheel backward
void turn_left() {
  digitalWrite(M11A, LOW); 
  digitalWrite(M12A, HIGH); 
   
  digitalWrite(M23A, HIGH); 
  digitalWrite(M24A, LOW); 
}

void stop() { 
  digitalWrite(M11A, LOW); 
  digitalWrite(M12A, LOW); 
   
  digitalWrite(M23A, LOW); 
  digitalWrite(M24A, LOW); 
}
