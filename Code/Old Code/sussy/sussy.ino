// C++ code
//

#include <Servo.h>

Servo servo;

int m ;
int x ;
double duration ;
double distance ;
double duration2 ;
double distance2 ;
double l;
double r;
int pos ;

int incomingByte = 0 ;


// Set up pin names

// Front Sonar
#define SonarIN 12
#define SonarOUT 11

// Side Sonar
#define SonarIN2 7
#define SonarOUT2 8

#define ServoPin 10

#define LeftMotorEnable 9
#define LeftPin2 5
#define LeftPin1 4

#define RightMotorEnable 3
#define RightPin1 2
#define RightPin2 6




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
  //Sonar ping
  digitalWrite(SonarIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SonarIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SonarIN, LOW);
  

  duration = pulseIn(SonarOUT, HIGH);
  distance = (duration * 0.0343)/2;

  //Print distance - FOR TESTING SONAR
  Serial.println(distance);
}


void sonar2(){
  //Sonar ping
  digitalWrite(SonarIN2, LOW);
  delayMicroseconds(2);
  digitalWrite(SonarIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(SonarIN2, LOW);
  

  duration2 = pulseIn(SonarOUT2, HIGH);
  distance2 = (duration2 * 0.0343)/2;

  //Print distance - FOR TESTING SONAR
  Serial.println(distance2);
}

void sonarTurn() {
  l = 0;
  r = 0;
  sonar();
  delay(100);
  if(distance < 6) {
    stop();
    delay(500);
    servo.write(0);
    delay(500);
    sonar();
    r = distance;
    delay(200);
    servo.write(180);
    delay(500);
    sonar();
    l = distance;
    delay(200);
    servo.write(90);
    delay(1000);

    if (abs(l-r) < 3 ) {
      leftonspot();
      delay(1800);
    }
    
    
    else if (l>r){
      leftonspot();
      delay(800);
    }
    else {
      rightonspot();
      delay(800);
    }
    }
  else {
    sonarDrive();
  }
  
}


void sonarDrive() {
  sonar2();
  
  if (distance2 > 6) {
    semileft();
  }
  else if (distance2 < 8) {
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

  //Set up the sonar pins
  pinMode(SonarIN, OUTPUT);
  pinMode(SonarOUT, INPUT);
  pinMode(SonarIN2, OUTPUT);
  pinMode(SonarOUT2, INPUT);


  //Set up servo pins
  servo.attach(ServoPin);
  servo.write(90);
  pos = 0;

  //Initialise Serial
  Serial.begin(9600);
  Serial.println("setup is complete");
}

void loop()
{
 sonarTurn();
}







  
