#include "Arduino.h"
#include "scheduler.h"
#include <LiquidCrystal.h>
 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

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
const unsigned int roomba1for = 48;
const unsigned int roomba1back = 49;
const unsigned int roomba1stop = 50;

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
const int laser_button_pin = A10;     // the number of the pushbutton pin (fire laser)
const int servo_1_pin = A8;     // pin # moves servo 1
const int servo_2_pin = A9;     // pin # moves servo 2
const int roomba_1_pin = A13; // pin # moves roomba
const int roomba_2_pin = A14; // pin # moves roomba
const int roomba_button_pin = A12;


// GLOBAL STATUS VARIABLES
bool firing_laser = false; // global variable for current state of laser
int servo_1_dir = 0; // -1 (backward), 0 (static), 1 (forward)
int servo_2_dir = 0; // -1 (backward), 0 (static), 1 (forward)
int roomba_dir1 = 0; // -1 (backward), 0 (static), 1 (forward)
int roomba_dir2 = 0; // -1 (backward), 0 (static), 1 (forward)


void signal_fire_laser() {
  Serial1.write(firelaser);
}

void signal_turnoff_laser() {
  Serial1.write(offlaser);
}
 
// task function for joystick1 task
void joystick1_task()
{
  int sensorValue1 = map((int) analogRead(servo_1_pin), 0, 1024, 0, 180); //input signal for controlling myServo
  int sensorValue2 = map((int) analogRead(servo_2_pin), 0, 1024, 0, 180); //input signal for controllers myServo2
  
  //lcd.setCursor(0,0);
  //lcd.print("X: );
  //lcd.print
  
  if (sensorValue1 < 70 && sensorValue1 >= 0 && servo_1_dir != backward) {
    servo_1_dir = backward;
    Serial1.write(servo1left);
  } else if (sensorValue1 < 180 && sensorValue1 > 110 && servo_1_dir != forward) {
    servo_1_dir = forward;
    Serial1.write(servo1right);
  } else if(sensorValue1 < 109 && sensorValue1 > 50 && servo_1_dir != stopped) {
    servo_1_dir = stopped;
    Serial1.write(servo1stopped);
  }

  if (sensorValue2 < 70 && sensorValue2 >= 0 && servo_2_dir != backward) {
    servo_2_dir = backward;
    Serial1.write(servo2left);
  } else if (sensorValue2 < 180 && sensorValue2 > 110 && servo_2_dir != forward) {
    servo_2_dir = forward;
    Serial1.write(servo2right);
  } else if(sensorValue2 < 109 && sensorValue2 > 50 && servo_2_dir != stopped) {
    servo_2_dir = stopped;
    Serial1.write(servo2stopped);
  }

}
 
// task function for joystick2 task
void joystick2_task()
{
  int sensorValue3 = map((int) analogRead(roomba_1_pin), 0, 1024, 0, 180); //input signal for controlling roomba1
  int sensorValue4 = map((int) analogRead(roomba_2_pin), 0, 1024, 0, 180); //input signal for controllers roomba2

  if (sensorValue3 < 70 && sensorValue3 >= 0 && roomba_dir1 != backward) {
    roomba_dir1 = backward;
    Serial1.write(roomba1back);
  } else if (sensorValue3 < 180 && sensorValue3 > 110 && roomba_dir1 != forward) {
    roomba_dir1 = forward;
    Serial1.write(roomba1for);
  } else if(sensorValue3 < 109 && sensorValue3 > 50 && roomba_dir1 != stopped) {
    roomba_dir1 = stopped;
    Serial1.write(roomba1stop);
  }

  if (sensorValue4 < 70 && sensorValue4 >= 0 && roomba_dir2 != backward) {
    roomba_dir2 = backward;
    Serial1.write(roomba2back);
  } else if (sensorValue4 < 180 && sensorValue4 > 110 && roomba_dir2 != forward) {
    roomba_dir2 = forward;
    Serial1.write(roomba2for);
  } else if(sensorValue4 < 109 && sensorValue4 > 50 && roomba_dir2 != stopped) {
    roomba_dir2 = stopped;
    Serial1.write(roomba2stop);
  }
}

// tast function for 
void bluetooth_task()
{
  
}

void lcd_task()
{
  lcd.setCursor(0,1);
  lcd.print("Laser: ");
  // transmit only when change state of laser
  int laser_value = analogRead(laser_button_pin);
  if (laser_value < 400 && !firing_laser) {
    lcd.setCursor(7,1);
    lcd.print(" ON ");
    firing_laser = true;
    signal_fire_laser();
  } else if(laser_value > 401 && firing_laser){
    lcd.setCursor(7,1);
    lcd.print(" OFF");
    firing_laser = false;
    signal_turnoff_laser();
  }
  else if(!firing_laser)
    lcd.print(" OFF");
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
  Serial.begin(9600);
  Serial1.begin(9600);
  // initialize pins
  pinMode(laser_button_pin, INPUT_PULLUP);
  pinMode(roomba_button_pin, INPUT_PULLUP);
  lcd.begin(16, 2);
 
  Scheduler_Init();
 
  // Start task arguments are:
  //    start offset in ms, period in ms, function callback
 
  Scheduler_StartTask(0, 100, joystick1_task);
  Scheduler_StartTask(200, 100, joystick2_task);
  Scheduler_StartTask(300, 100, bluetooth_task);
  Scheduler_StartTask(300, 100, lcd_task);
}
 
void loop()
{
  uint32_t idle_period = Scheduler_Dispatch();
  if (idle_period)
  {
    idle(idle_period);
  }
}

