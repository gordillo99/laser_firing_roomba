
#include <Servo.h>
// the setup routine runs once when you press reset:

const int laser_button_pin = 2;     // the number of the pushbutton pin (fire laser)
const int laser_activation_pin = 3;      // the number of the laster activation pin

Servo myServo, myServo2; //The two servo motors. myServo attached to digital pin 9 and myServo2 attached to digital pin 8

int pos;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial1.begin(9600);
  // initialize laser related pins
  pinMode(laser_button_pin, INPUT_PULLUP);
  pinMode(laser_activation_pin, OUTPUT);


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

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0); //input signal for controlling myServo
  int sensorValue2 = analogRead(A1); //input signal for controllers myServo2
  
  //The part below controlls the movement of myServo
  sensorValue = map(sensorValue, 100, 670, 0, 180);
  
  //Serial.println(sensorValue);//This prints the value to Serial1
  
  pos = myServo.read();
  if(pos == 0 || pos == 180){
      if(pos == 0){
        myServo.write(pos+1);
        delay(10);
      }
      if(pos == 180){
        myServo.write(pos-1);
        delay(10);
      }
  }
  if((pos <= 179 && pos >= 1) && (sensorValue < 56 || sensorValue > 120)){
    if(sensorValue > (pos)){
      myServo.write(pos+1);
      delay(10);
    }else if(sensorValue > (pos+20)){
      myServo.write(pos+2);
      delay(10);
    }
    if(sensorValue < (pos)){
      myServo.write(pos-1);
      delay(10);
    }else if(sensorValue < (pos-20)){
      myServo.write(pos-2);
      delay(10);
    }
  }

  //the part below controlls the movement of myServo2
  sensorValue2 = map(sensorValue2, 100, 670, 0, 180);

  Serial.println(sensorValue2);//This prints the value to Serial1
  
  pos = myServo2.read();
  if(pos == 0 || pos == 180){
      if(pos == 0){
        myServo2.write(pos+1);
        delay(10);
      }
      if(pos == 180){
        myServo2.write(pos-1);
        delay(10);
      }
  }
  if((pos <= 179 && pos >= 1) && (sensorValue2 < 56 || sensorValue2 > 120)){
    if(sensorValue2 > (pos)){
      myServo2.write(pos+1);
      delay(10);
    }else if(sensorValue2 > (pos+20)){
      myServo2.write(pos+2);
      delay(10);
    }
    if(sensorValue2 < (pos)){
      myServo2.write(pos-1);
      delay(10);
    } else if(sensorValue2 < (pos-20)){
      myServo2.write(pos-2);
      delay(10);
    }
  }

int laser_button_state = digitalRead(laser_button_pin);

  if (laser_button_state == HIGH) {
    digitalWrite(laser_activation_pin, LOW);
  } else {
    digitalWrite(laser_activation_pin, HIGH);
  }

  delay(1);        // delay in between reads for stability
}
