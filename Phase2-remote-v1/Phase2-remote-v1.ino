#include <Servo.h>

// CONSTANT BYTE TO NUMBER CONVENTIONS
//servo1
const unsigned int servo1left = 0;
const unsigned int servo1right = 1;
const unsigned int servo1stopped = 2;

//servo2
const unsigned int servo2left = 16;
const unsigned int servo2right = 17;
const unsigned int servo2stopped = 18;

//Roomba
const unsigned int roombaleft = 32;
const unsigned int roombaright = 33;
const unsigned int roombafor = 48;
const unsigned int roombaback = 49;

//laser
const unsigned int firelaser = 64;
const unsigned int offlaser = 65;

// GENERAL DIRECTION CONSTANTS
const int backward = -1;
const int forward =1;
const int stopped = 0;
const int right = 2;
const int left = 3;

// CONSTANT PIN ASSIGNATIONS
const int laser_activation_pin = 3;      // the number of the laster activation pin

// GLOBAL STATUS VARIABLES
bool firing_laser = false; // global variable for current state of laser
int servo_1_dir = 0; // -1 (backward), 0 (static), 1 (forward)
int servo_2_dir = 0; // -1 (backward), 0 (static), 1 (forward)
int roomba_dir = 0;// -1 (backward), 0 (static), 1 (forward), 2 (right), 3 (left)

Servo myServo, myServo2; //The two servo motors. myServo attached to digital pin 9 and myServo2 attached to digital pin 8

int pos_servo1; // angle position of servo 1
int pos_servo2; // angle position of servo 2

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial1.begin(9600);
  
  // initialize laser related pins
  pinMode(laser_activation_pin, OUTPUT);

  // RESETS SERVOS TO INITIAL POSITION
  int pos = 0;
  myServo.attach(9);
  int angle = myServo.read();
  for (; angle < 90 - 1 || pos > 90 + 1;) { // initializes myServo to be at about 90 degrees
    // in steps of 1 degree
    // tell servo to go to around 90 degrees
    if(angle < 90 - 1)
      myServo.write(angle++);  
    if(angle > 90 - 1)
      myServo.write(angle--);  
    delay(10);                       // waits 15ms for the servo to reach the position
    angle = myServo.read();
  }

  myServo2.attach(8);
  angle = myServo2.read();
  for (; angle < 90 - 1 || pos > 90 + 1;) { // initializes the myServo2 to be at about 90 degrees
    // in steps of 1 degree
    // tell servo to go to around 90 degrees
    if(angle < 90 - 1)
      myServo2.write(angle++);  
    if(angle > 90 - 1)
      myServo2.write(angle--);  
    delay(10);                       
    angle = myServo2.read();
  }
}

void loop() {
  // poll for incoming commands
  if(Serial1.available()) {
    int command = Serial1.read();
    Serial.println(command);
    switch(command) {
      case firelaser:
      case offlaser:
        fire_laser(command);
        break;
      
      case servo1left:
        servo_1_dir = backward;
        break;
      case servo1right:
        servo_1_dir = forward;
        break;
      case servo1stopped:
        servo_1_dir = stopped;
        break;

      case servo2left:
        servo_2_dir = backward;
        break;
      case servo2right:
        servo_2_dir = forward;
        break;
      case servo2stopped:
        servo_2_dir = stopped;
        break;
    }   
  }

  if (servo_1_dir == backward) {
    move_servo_left(myServo);
  } else if (servo_1_dir == forward) {
    move_servo_right(myServo);
  }

  if (servo_2_dir == backward) {
    move_servo_left(myServo2);
  } else if (servo_2_dir == forward) {
    move_servo_right(myServo2);
  }

  delay(25); // delay in between reads for stability
}

void fire_laser(int command) {
  if (command == offlaser) {
    digitalWrite(laser_activation_pin, LOW);
  } else if(command == firelaser) {
    digitalWrite(laser_activation_pin, HIGH);
  }
}

void move_servo_left(Servo servo) {
  int pos = servo.read();

  if (pos > 5) {
    servo.write(pos - 1);
  }
}

void move_servo_right(Servo servo) {
  int pos = servo.read();

  if (pos < 170) {
    servo.write(pos + 1);
  }
}


