#include "Arduino.h"
#include "scheduler.h"
#include <Servo.h>
#include "Roomba_Driver.h"

Roomba r(2, 30);

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
const unsigned int roomba2for = 32;
const unsigned int roomba2back = 33;
const unsigned int roomba2stop = 34;
const unsigned int roomba2forfast = 35;
const unsigned int roomba2backfast = 36;

const unsigned int roomba1for = 48;
const unsigned int roomba1back = 49;
const unsigned int roomba1stop = 50;
const unsigned int roomba1forfast = 51;
const unsigned int roomba1backfast = 52;

//laser
const unsigned int firelaser = 64;
const unsigned int offlaser = 65;

//status
const unsigned int left_on = 80;
const unsigned int right_on = 81;
const unsigned int forward_on = 82;
const unsigned int backward_on = 83;
const unsigned int stopped_on = 84;

const unsigned int left_off = 112;
const unsigned int right_off = 113;
const unsigned int forward_off = 114;
const unsigned int backward_off = 115;
const unsigned int stopped_off = 116;

// GENERAL DIRECTION CONSTANTS
const int backward = -1;
const int forward = 1;
const int stopped = 0;
const int right = 2;
const int left = 3;
const int backward_fast = 4;
const int forward_fast = 5;
const int right_fast = 6;
const int left_fast = 7;

// CONSTANT PIN ASSIGNATIONS
const int laser_activation_pin = 3; // the number of the laster activation pin
const int photoresitor_pin = A15;

// GLOBAL STATUS VARIABLES
bool firing_laser = false; // global variable for current state of laser
int servo_1_dir = 0; // -1 (backward), 0 (static), 1 (forward)
int servo_2_dir = 0; // -1 (backward), 0 (static), 1 (forward)
int roomba1_dir = 0;// -1 (backward), 0 (static), 1 (forward)
int roomba2_dir = 0;// -1 (backward), 0 (static), 1 (forward)

Servo myServo, myServo2; //The two servo motors. myServo attached to digital pin 9 and myServo2 attached to digital pin 8

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

void turn_roomba_right() {
  r.drive(100, -1);
}

void turn_roomba_left() {
  r.drive(100, 1);
}

void move_roomba_forward() {
  r.drive(-100, 32768);
}

void move_roomba_backward() {
  r.drive(100, 32768);
}

void stop_roomba() {
  r.drive(0, 0);
}

void move_roomba_forward_left() {
  r.drive(100, -100);
}

void move_roomba_forward_right() {
  r.drive(100, 100);
}

void move_roomba_backward_left() {
  r.drive(-100, 100);
}

void move_roomba_backward_right() {
  r.drive(-100, -100);
}

void turn_roomba_right_fast() {
  r.drive(200, -1);
}

void turn_roomba_left_fast() {
  r.drive(200, 1);
}

void move_roomba_forward_fast() {
  r.drive(200, 32768);
}

void move_roomba_backward_fast() {
  r.drive(-200, 32768);
}

void move_roomba_forward_left_fast() {
  r.drive(200, 100);
}

void move_roomba_forward_right_fast() {
  r.drive(200, -100);
}

void move_roomba_backward_left_fast() {
  r.drive(-200, -100);
}

void move_roomba_backward_right_fast() {
  r.drive(-200, 100);
}

void poll_incoming_commands() {
  digitalWrite(44, HIGH);
  // poll for incoming commands
  if(Serial1.available()) {
    int command = Serial1.read();
    //Serial.println(command);
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

      case roomba1for:
        roomba1_dir = right;
        break;
      case roomba1back:
        roomba1_dir = left;
        break;
      case roomba1stop:
        roomba1_dir = stopped;
        break;
      case roomba1forfast:
        roomba1_dir = right_fast;
        break;
      case roomba1backfast:
        roomba1_dir = left_fast;
        break;

      case roomba2for:
        roomba2_dir = backward;
        break;
      case roomba2back:
        roomba2_dir = forward;
        break;
      case roomba2stop:
        roomba2_dir = stopped;
        break;
      case roomba2forfast:
        roomba2_dir = forward_fast;
        break;
      case roomba2backfast:
        roomba2_dir = backward_fast;
        break;
    } 
    Serial.println(roomba1_dir);
    Serial.println(roomba2_dir);
  }
  digitalWrite(44, LOW);
}

void move_servos() {
  digitalWrite(45, HIGH);
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
  digitalWrite(45, LOW);
}

void move_roomba() {
  digitalWrite(46, HIGH);
  if (roomba1_dir == right_fast && roomba2_dir == forward_fast || roomba1_dir == right && roomba2_dir == forward_fast || roomba1_dir == right_fast && roomba2_dir == forward) { 
    move_roomba_forward_right_fast();
  } else if (roomba1_dir == left_fast && roomba2_dir == forward_fast || roomba1_dir == left && roomba2_dir == forward_fast || roomba1_dir == left_fast && roomba2_dir == forward) { 
    move_roomba_forward_left_fast();
  } else if (roomba1_dir == right_fast && roomba2_dir == backward_fast || roomba1_dir == right && roomba2_dir == backward_fast || roomba1_dir == right_fast && roomba2_dir == backward) { 
    move_roomba_backward_right_fast();
  } else if (roomba1_dir == left_fast && roomba2_dir == backward_fast || roomba1_dir == left && roomba2_dir == backward_fast || roomba1_dir == left_fast && roomba2_dir == backward) { 
    move_roomba_backward_left_fast();
  } else if (roomba2_dir == backward_fast) {
    move_roomba_backward_fast();
  } else if (roomba2_dir == forward_fast) {
    move_roomba_forward_fast();
  } else if (roomba1_dir == right_fast) { 
    turn_roomba_right_fast();
  } else if (roomba1_dir == left_fast) { 
    turn_roomba_left_fast();
  } else if (roomba2_dir == backward) {
    move_roomba_backward();
  } else if (roomba2_dir == forward) {
    move_roomba_forward();
  } else if (roomba1_dir == right) { 
    turn_roomba_right();
  } else if (roomba1_dir == left) { 
    turn_roomba_left();
  } 

  if (roomba1_dir == stopped && roomba2_dir == stopped) {
    stop_roomba();
  } 
  digitalWrite(46, HIGH);
}

void send_status() {
  int photoresistor_val = analogRead(photoresitor_pin);
  bool light_on_photoresistor = photoresistor_val > 400;
  if (light_on_photoresistor) {
    if (roomba1_dir == left_fast) Serial1.write(80);
    else if (roomba1_dir == right_fast) Serial1.write(81);
    else if (roomba2_dir == forward_fast) Serial1.write(82);
    else if (roomba2_dir == backward_fast) Serial1.write(83);
    else if (roomba1_dir == stopped && roomba2_dir == stopped) Serial1.write(84);
  } else {
    if (roomba1_dir == left_fast) Serial1.write(112);
    else if (roomba1_dir == right_fast) Serial1.write(113);
    else if (roomba2_dir == forward_fast) Serial1.write(114);
    else if (roomba2_dir == backward_fast) Serial1.write(115);
    else if (roomba1_dir == stopped && roomba2_dir == stopped) Serial1.write(116);
  }
}
 
// idle task
void idle(uint32_t idle_period)
{
  // this function can perform some low-priority task while the scheduler has nothing to run.
  // It should return before the idle period (measured in ms) has expired.  For example, it
  // could sleep or respond to I/O.
 
  // example idle function that just pulses a pin.
  //digitalWrite(idle_pin, HIGH);
  //delay(idle_period);
  //digitalWrite(idle_pin, LOW);
}
 
void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  
  // initialize laser related pins
  pinMode(laser_activation_pin, OUTPUT);

  // for logic analyzer test
  pinMode(44, OUTPUT);
  pinMode(45, OUTPUT);
  pinMode(46, OUTPUT);

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

  //initialize the Roomba
  r.init();
  delay(1000);
 
  // initialize scheduler
  Scheduler_Init();
 
  // Start task arguments are:
  //    start offset in ms, period in ms, function callback
 
  Scheduler_StartTask(0, 100, poll_incoming_commands); // 10 Hz
  Scheduler_StartTask(100, 15, move_servos); // 67 Hz
  Scheduler_StartTask(100, 150, move_roomba); // 6.7 Hz
  Scheduler_StartTask(200, 500, send_status); // 2 Hz
}
 
void loop()
{
  uint32_t idle_period = Scheduler_Dispatch();
  if (idle_period)
  {
    idle(idle_period);
  }
}

