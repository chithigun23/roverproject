#include <NewPing.h>
/////////////////////////////////////
// Calibration Values
/////////////////////////////////////
const int side = 4000;
const int turn90 = 825; // Use half for turbo mode
const int zagAngle = 15; // Zag Angle in Degrees

// Minimum Distance
const int minDist = 20;

/////////////////////////////////////
// Pin Setup
/////////////////////////////////////

const int turn = (zagAngle/ 90.0 ) * turn90; 
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
 
// keep track of sonar distances 
float sonar1_distance;  
float sonar2_distance; 

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
  analogWrite(enablePinM2, 200); 
} 
 
void loop() { 
  delay(2000);
  bool first = true;

  while (true) {  
    float distance_F = frontSonar.ping_median(9) / 58.3;
    Serial.print("Distance: ");
    Serial.println(distance_F);

    turn_right();
    if (first) {
      delay(turn); 
    } else {
      delay(turn * 2);
    }

    if (distance_F < minDist && distance_F > 0) {
      // Stop if too close and reading is valid
      break;
    }

    stop(); 
    delay(150);

    move_forward(); 
    delay(side);  
    stop(); 
    delay(150);

    if (distance_F < minDist && distance_F > 0) {
      // Stop if too close and reading is valid
      break;
    }

    turn_left();
    delay(turn * 2);  
    stop(); 
    delay(150);

    if (distance_F < minDist && distance_F > 0) {
      // Stop if too close and reading is valid
      break;
    }

    move_forward(); 
    delay(side);   
    stop(); 
    delay(150);
    
    if (distance_F < minDist && distance_F > 0) {
      // Stop if too close and reading is valid
      break;
    }

    first = false;
  } 

  // Stop after detecting something too close
  while (true) {
    stop(); 
  }
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
