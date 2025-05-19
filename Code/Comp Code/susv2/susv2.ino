#include <Servo.h>
#include <NewPing.h>

Servo servo;

double distance;
double distance2;
double l;
double r;


// Set up pin names

// Front Sonar
#define SonarIN 11
#define SonarOUT 12

// Side Sonar
#define SonarIN2 8
#define SonarOUT2 7

#define ServoPin 10

#define LeftMotorEnable 9
#define LeftPin2 5
#define LeftPin1 4

#define RightMotorEnable 3
#define RightPin1 2
#define RightPin2 6

NewPing frontSonar(SonarIN,SonarOUT,200);
NewPing leftSonar(SonarIN2,SonarOUT2,200);

// Calibration Values
#define obst_thresh 12
#define turn_delay 650
#define minLeft 5
#define maxLeft 8

void stop(){
  digitalWrite(LeftPin1, LOW);
  digitalWrite(LeftPin2, LOW);
  digitalWrite(RightPin1, LOW);
  digitalWrite(RightPin2, LOW);
}



void anticlockwise(int x, int y){
  digitalWrite(x, LOW);
  digitalWrite(y, HIGH);
}
void clockwise(int x, int y){
  digitalWrite(y, LOW);
  digitalWrite(x, HIGH);
}

void left() {
  clockwise(RightPin1,RightPin2);
  digitalWrite(LeftPin1, LOW);
  digitalWrite(LeftPin2, LOW);
}

void semileft(){
  clockwise(RightPin1,RightPin2);
  analogWrite(LeftPin2, 60);
  digitalWrite(LeftPin1, LOW);
}

void leftonspot() {
  clockwise(RightPin1,RightPin2);
  clockwise(LeftPin1,LeftPin2);
}

void rightonspot() {
  anticlockwise(LeftPin1,LeftPin2);
  anticlockwise(RightPin1,RightPin2);
}


void right() {
  anticlockwise(LeftPin1,LeftPin2);
  digitalWrite(RightPin1, LOW);
  digitalWrite(RightPin2, LOW);
}


void go() { 
  clockwise(RightPin1,RightPin2);
  anticlockwise(LeftPin1,LeftPin2);
}



void sonar(){
  distance =frontSonar.ping_median(9) / 58.3;
  
  //Print distance - FOR TESTING SONAR
  Serial.println(distance);
}


void sonar2(){
  distance2 = leftSonar.ping_median(9) / 58.3;

  //Print distance - FOR TESTING SONAR
  Serial.println(distance2);
}

void sonarTurn() {
  l = 0;
  r = 0;
  sonar();
  delay(300);
  if(distance < obst_thresh && distance != 0) {
    stop();
    delay(800);
    servo.write(0);
    delay(800);
    sonar();
    r = distance;
    delay(800);
    servo.write(180);
    delay(800);
    sonar();
    l = distance;
    delay(800);
    servo.write(90);
    delay(1000);

    if (abs(l-r) < 3 ) {
      leftonspot();
      delay(turn_delay*2);   // U-trun
    }
    
    
    else if (l>r){
      leftonspot();
      delay(turn_delay);  // Left Turn
    }
    else {
      rightonspot();
      delay(turn_delay);  // Right Turn
    }
    }
  else {
    sonarDrive();
  }
  
}


void sonarDrive() {
  sonar2();
  
  if (distance2 > maxLeft) {
    semileft();
  }
  else if (distance2 < maxLeft) {
    right();
  }
  else {
    go();
  }
  
  delay(50);
}



void setup()
{
  //Set up the motor pins
  pinMode(LeftMotorEnable, OUTPUT);
  pinMode(LeftPin1, OUTPUT);
  pinMode(LeftPin2, OUTPUT);
  pinMode(RightMotorEnable, OUTPUT);
  pinMode(RightPin1, OUTPUT);
  pinMode(RightPin2, OUTPUT);

  //Initialise the motor controller
  digitalWrite(LeftMotorEnable, HIGH);
  digitalWrite(RightMotorEnable, HIGH);
  digitalWrite(LeftPin1, LOW);
  digitalWrite(LeftPin2, LOW);
  digitalWrite(RightPin1, LOW);
  digitalWrite(RightPin2, LOW);

  


  //Set up servo pins
  servo.attach(ServoPin);
  servo.write(90);

  //Initialise Serial
  Serial.begin(9600);
  Serial.println("setup is complete");
}

void loop()
{
 sonarTurn();
}







  
