#include <NewPing.h>
/////////////////////////////////////
// Pin Setup
/////////////////////////////////////

// Distances (cm)
const int minDistF = 10;
const float minDistL = 4;
const float maxDistL = 9;


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
  digitalWrite(enablePinM2, HIGH); 
} 
 
void loop() {

  left_wall_keeping();

  // while (leftDist < distance_L && rightDist > distance_L){

    // Get front distance using NewPing
  // distance_F = frontSonar.ping_median(9) / 58.3;
  // Serial.print("Front Distance: ");
  // Serial.print(distance_F);
  // Serial.println(" cm");

  // if (distance_F == 0 || distance_F > minDist) {
    // move_forward();
  // } 
  // else {
    // stop();
    // Serial.println("Wall too close. Stopping.");
    // while (true); // Stop forever
  // }

  // delay(100); // Small delay to avoid flooding sensor

  //  }
  
}

void print_info(String desc, float dist_F, float dist_L) {
  Serial.print(desc);
  Serial.print(": distance_F = ");
  Serial.print(dist_F);
  Serial.print(", distance_L = ");
  Serial.println(dist_L);
}

void anti_front_collision() {
  distance_F = frontSonar.ping_median(9) / 58.3;
  distance_L = leftSonar.ping_median(9) / 58.3;

  if (distance_F < minDistF && distance_F != 0) {
    print_info(" F < minDistF && distance_F != 0", distance_F, distance_L);

    move_backward();
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
  move_slight_left();
  delay(200);
  stop();
  }

  else if (distance_L > maxDistL && distance_F > minDistF && distance_F != 0) {
  print_info("Too close right. NOT Clear ahead. Reversing and Turning Right", distance_F, distance_L);
  move_backward();
  delay(500);
  turn_right();
  delay(200);
  move_forward();
  delay(200);
  stop();
  }

  else if (distance_L < minDistL && distance_F > minDistF && distance_F != 0 && distance_L != 0 ) {
  print_info("Too close left. Clear ahead. Veering right", distance_F, distance_L);
  move_slight_right();
  delay(200);
  stop();
  }

  else if (distance_L < minDistL && distance_F > minDistF && minDistF != 0) {
  print_info("Too close left. NOT Clear ahead. Reversing and Turning Left", distance_F, distance_L);
  move_backward();
  delay(500);
  turn_left();
  delay(200);
  move_forward();
  delay(200);
  stop();
  }

  else if (distance_L < minDistL && distance_F < minDistF && minDistF != 0) {
  print_info("Right Corner, Turning Sharp Right", distance_F, distance_L);
  turn_right();
  delay(800);
  stop();
  }

  else if (distance_L > maxDistL && distance_F < minDistF && distance_F != 0 ||distance_L == 0 ) {
  print_info("Left Corner, Turning Sharp Left", distance_F, distance_L);
  turn_left();
  delay(800);
  stop();
  }

  else if (distance_L > minDistL && distance_L < maxDistL && distance_F < minDistF && distance_F != 0) {
  print_info("Right Corner, Turning NOT_AS_Sharp Right", distance_F, distance_L);
  turn_right();
  delay(600);
  stop();
  }
  else {
    if (distance_F == 0 || distance_F > minDistF) {
      print_info("All Clear, Moving Forward", distance_F, distance_L);
      move_forward();
      delay(500);
      stop();
    }
    else {
      print_info("object detected, reversing", distance_F, distance_L);
      move_backward();
      delay(500);
      stop();
    }
  }
}

void move_slight_left() {
  turn_left();
  delay(200);
  move_forward();
  delay(500);
}

void move_slight_right() {
  turn_right();
  delay(200);
  move_forward();
  delay(500);
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
