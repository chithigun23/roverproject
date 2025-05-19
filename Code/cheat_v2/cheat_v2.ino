#include <NewPing.h>
#include <Servo.h>

/////////////////////////////////////
// Pin Setup
/////////////////////////////////////
Servo myServo;

// Minimum Distance
#define minDistL 8
#define minDistF 12
#define leftTurnCal 995
#define rightTurnCal 995
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
float dist_F_R;

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

  Serial.print("Front Distance: ");
  Serial.println(distance_F);

  if ((distance_F != 0) && (distance_F < minDistF)) {
    // Obstacle ahead, check side distances
    float distRight = getFrontDistanceAtAngle(0);     // right check
    delay(3000);
    float distLeft  = getFrontDistanceAtAngle(180);   // left check

    if (distLeft < minDistL && distRight < minDistL) {
      // Both sides blocked = dead end
      Serial.println("Dead end detected.");
      dead_end();
    } else {
      // One side is open, use turning decisions
      Serial.println("Turning decision based on side distances.");
      turning_decisions();
    }
  } else {
    // Path is clear, move forward
    move_forward();
    delay(500);
  }
}

float getFrontDistanceAtAngle(int angle) {
  myServo.write(angle);
  delay(3000); // wait for servo to rotate
  float distance = frontSonar.ping_median(9) / 58.3;
  delay(2000);
  return (distance == 0) ? 200.0 : distance; // 200 cm fallback if no reading
}

void turning_decisions() {
  float distRight = getFrontDistanceAtAngle(0);    // Measure distance at right
  float distLeft  = getFrontDistanceAtAngle(180);  // Measure distance at left

  if (distRight < distLeft) {
    Serial.println("distRight < distLeft: Turning LEFT");
    turn_left(leftTurnCal);  // You can adjust timing as needed
  } else {
    Serial.println("distLeft <= distRight: Turning RIGHT");
    turn_right(rightTurnCal);
  }
}

void dead_end() {
  float distRight = getFrontDistanceAtAngle(0);    // Right side
  float distLeft  = getFrontDistanceAtAngle(180);  // Left side

  if (distLeft < 12 && distRight < 12) {
    Serial.println("Dead end detected. Executing double left turn.");
    turn_left(995);      // First turn left
    move_backward();     // Move back a little
    delay(300);
    stop();
    turn_left(995);      // Second turn left (U-turn)
    stop();
  } else if (distRight >= distLeft) {
    Serial.println("Turning right to escape dead end.");
    turn_right(995);
  } else {
    Serial.println("Turning left to escape dead end.");
    turn_left(995);
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
