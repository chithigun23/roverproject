/////////////////////////////////////
// Pin Setup
/////////////////////////////////////

// sonar1 pins 
const int echoPin1 = 11; // <-- assign proper pins
const int trigPin1 = 10; 
 
// sonar2 pins 
const int echoPin2 = 13; // <-- assign proper pins
const int trigPin2 = 12; 
 
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
  digitalWrite(enablePinM2, HIGH); 
} 
 
void loop() { 
  for (int i = 0; i < 1; i++) {  // Here we have a loop for testing purposes, for Test 1.2 cond will be i < 1
    // Move forward ~20cm 
    move_forward(); 
    delay(3000);  // values may need tweaking after testing 
    stop(); 
    delay(150);   // small delay 

    turn_right();
    delay(500);  // values may need tweaking after testing 
    stop(); 
    delay(150);   // small delay 

    move_forward(); 
    delay(3000);  // values may need tweaking after testing 
    stop(); 
    delay(150);   // small delay 

    turn_right();
    delay(500);  // values may need tweaking after testing 
    stop(); 
    delay(150);   // small delay 

    move_forward(); 
    delay(3000);  // values may need tweaking after testing 
    stop(); 
    delay(150);   // small delay 

    turn_right();
    delay(500);  // values may need tweaking after testing 
    stop(); 
    delay(150);   // small delay 

 
    // Uncomment if you want to turn:
    // turn_left(); 
    // delay(725);   
    // stop(); 
    // delay(150);
  } 
 
  // Stop after completing the move 
  while (true) { 
    stop(); 
  } 
} 

void update_sonar_distances() { 
  // Trigger sonar 1 
  digitalWrite(trigPin1, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin1, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin1, LOW); 
  sonar1_distance = pulseIn(echoPin1, HIGH) / 58.0; 
 
  delay(50); // Small delay between triggers 
 
  // Trigger sonar 2 
  digitalWrite(trigPin2, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin2, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin2, LOW); 
  sonar2_distance = pulseIn(echoPin2, HIGH) / 58.0; 
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
