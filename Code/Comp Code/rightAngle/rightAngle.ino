#include <NewPing.h>
#include <Servo.h>
/////////////////////////////////////
// Pin Setup
/////////////////////////////////////
Servo myServo;
// Minimum Distance
#define minDist 20
#define leftTurnCal 900
#define rightTurnCal 900

// Set up pin names

// Front Sonar
#define triggerFront 11
#define echoFront 12

// Side Sonar
#define triggerLeft 8
#define echoLeft 7

#define ServoPin 10

#define enablePinM1 9
#define M12A 4
#define M11A 5

#define enablePinM2 3
#define M23A 2
#define M24A 6

NewPing frontSonar(triggerFront,echoFront,200);
NewPing leftSonar(triggerLeft,echoLeft,200);
 
// keep track of distances 
float distance_F;  
float distance_L; 

void setup() { 
  Serial.begin(9600); 
  
  myServo.attach(ServoPin);
  myServo.write(90);
     
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
  distance_F = frontSonar.ping_median(9) / 58.3;
  distance_L = leftSonar.ping_median(9) / 58.3;
  Serial.println(distance_F);
  //if there is a front wall
  if ((distance_F != 0) && (distance_F < minDist)) {
    //if there is a left wall 
    if ((distance_L !=0) && (distance_L > minDist)) {
      turn_right(rightTurnCal);
    }
    if ((distance_L == 0) || (distance_L < minDist)) {
    //if there is not a left wall
      turn_left(leftTurnCal);
    }
  }

  else {
    move_forward();
    delay(500);
  }

 

  //otherwise keep going forward

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

void turn_left(int x) {
  digitalWrite(M11A, HIGH); 
  digitalWrite(M12A, LOW); 
   
  digitalWrite(M23A, LOW); 
  digitalWrite(M24A, HIGH); 

  delay(x);
  stop();
}

void turn_right(int x) {
  digitalWrite(M11A, LOW); 
  digitalWrite(M12A, HIGH); 
   
  digitalWrite(M23A, HIGH); 
  digitalWrite(M24A, LOW); 
  delay(x);
  stop();
}

void stop() { 
  digitalWrite(M11A, LOW); 
  digitalWrite(M12A, LOW); 
   
  digitalWrite(M23A, LOW); 
  digitalWrite(M24A, LOW); 
}
