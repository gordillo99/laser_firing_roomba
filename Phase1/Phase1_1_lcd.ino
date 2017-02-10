#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
  lcd.begin(16, 2);
}

int x_position = 0, y_position = 0; //x position and y position of joystick
int counter = 0;
const int photoresistor_pin = A8;  // define a pin for Photo resistor

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 0);
  if(Serial1.available()){
    x_position = Serial1.read();
    x_position = map(x_position, 0, 180, 0, 1023);
    
    if(counter > 2500){
      lcd.clear();
      lcd.print("x: ");
      lcd.print(x_position);
      //Serial.println(x_position);    
    }
  }
  if(Serial1.available()){
    lcd.setCursor(7, 0);
    y_position = Serial1.read();
    y_position = map(y_position, 0, 180, 0, 1023);
    
    if(counter > 2500){
      lcd.print(" y: ");
      lcd.print(y_position);
      
      //Serial.println(y_position);
      lcd.setCursor(0, 1);
      lcd.print("Light: ");
      if (analogRead(photoresistor_pin) > 400) {
        lcd.print("Yes");
      } else {
        lcd.print("No");
      }
      
      counter = 0;
    }
 
/*
  if(Serial1.available()){
    
    if(counter > 2500){
      lcd.clear();
      String str = Serial1.readString();
      Serial.print(str);
      lcd.print(str);
      lcd.setCursor(0, 1);
      lcd.print("Light: ");
      if (analogRead(photoresistor_pin) > 400) {
        lcd.print("Yes");
      } else {
        lcd.print("No");
      }
      
      counter = 0;
    }
  }
  */
  counter++;
  //delay(2);
  //delay(150);
}
