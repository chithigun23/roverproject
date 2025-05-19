#include <NewPing.h>
#include <Servo.h>

/////////////////////////////////////
// Pin Setup
/////////////////////////////////////
Servo myServo;

// Minimum Distance
#define minDistL 8
#define minDistF 12
#define rightTurnCal 1004
#define leftTurnCal 995
int counter = 0;

// Front Sonar
#define triggerFront 11
#define echoFront 12

// Side Sonar
#define triggerLeft 8
#define echoLeft 7

#define ServoPin 10

// Motor 1
#define enablePinM1 9
#define M12A 4
#define M11A 5

// Motor 2
#define enablePinM2 3
#define M23A 2
#define M24A 6

NewPing frontSonar(triggerFront, echoFront, 200);
NewPing leftSonar(triggerLeft, echoLeft, 200);

// Distances
float distance_F;
float distance_L;

void setup() {
  Serial.begin(9600);

  myServo.attach(ServoPin);
  myServo.write(90);

  // Motor 1 setup
  pinMode(enablePinM1, OUTPUT);
  pinMode(M12A, OUTPUT);
  pinMode(M11A, OUTPUT);
  digitalWrite(enablePinM1, HIGH);

  // Motor 2 setup
  pinMode(enablePinM2, OUTPUT);
  pinMode(M23A, OUTPUT);
  pinMode(M24A, OUTPUT);
  digitalWrite(enablePinM2, HIGH);
}

void loop() {
  distance_F = frontSonar.ping_median(9) / 58.3;
  distance_L = leftSonar.ping_median(9) / 58.3;
  Serial.println(distance_F);

  if ((distance_F != 0) && (distance_F < minDistF)) {
    if (counter != 3) {
      if ((distance_L != 0) && (distance_L > minDistL)) {
        turn_right(rightTurnCal);
      } else {
        turn_left(leftTurnCal);
        counter += 1;
      }
    } else {
      turn_right(rightTurnCal);
      counter += 1;
    }
  } else {
    move_forward();
    delay(500);
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
