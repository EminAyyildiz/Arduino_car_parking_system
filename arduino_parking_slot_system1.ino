#include<Servo.h>    
#include <Keypad.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(32,16,2);
int t=3000;
byte capacity = 10;
const int trigPin1 = 9;
const int echoPin1 = 8;
const int trigPin2 = 11;
const int echoPin2 = 10;

int red_led1 = 2;
int green_led1 = 3;
int red_led2 = 4;
int green_led2 = 7;

long duration1, distance1;
long duration2, distance2;
Servo servogiris; 
Servo servocikis;
int entryCounter = 0;
int exitCounter = 0;
void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(red_led1, OUTPUT);
  pinMode(green_led1, OUTPUT);
  pinMode(green_led2, OUTPUT);
  pinMode(red_led2, OUTPUT);
  servogiris.attach(5);
  servogiris.write(0);
  servocikis.attach(6);
  servocikis.write(0);
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("***WELCOME***");
  lcd.setCursor(0, 1);
  lcd.print("Capacity: " + String(capacity));

}
void loop() {

  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1 / 2) / 29.1;

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2 / 2) / 29.1;

  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.print(" cm, Distance 2: ");
  Serial.print(distance2);
  Serial.println(" cm");
  digitalWrite(red_led1, HIGH);
  digitalWrite(green_led1, LOW);
  digitalWrite(red_led2, HIGH);
  digitalWrite(green_led2, LOW);
  delay(100);

  if (distance1 < 50) {
    entryCounter++;
    if (entryCounter == 1) {
      if (capacity > 0) {
        digitalWrite(red_led1, LOW);
        digitalWrite(green_led1, HIGH);
        servogiris.write(90);
        capacity--;
        delay(1000);
        digitalWrite(red_led1, HIGH);
        digitalWrite(green_led1, LOW);
        servogiris.write(0);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("****WELCOME****");
        lcd.setCursor(0, 1);
        lcd.print("Capacity: 0" + String(capacity));
      } else {
        servogiris.write(0);
        lcd.print("Capacity: " + String(capacity));
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("*****SORRY*****");
        lcd.setCursor(0, 1);
        lcd.print("Parking Lot Full.");
        digitalWrite(red_led1, HIGH);
      }
    }
  } else {
    entryCounter = 0;
  }

  if (distance2 < 50) {
    exitCounter++;
    if (exitCounter == 1) {
      if (capacity < 10) {
        digitalWrite(red_led2, LOW);
        digitalWrite(green_led2, HIGH);
        capacity++;
        servocikis.write(90);
        delay(1000);
        digitalWrite(red_led2, HIGH);
        digitalWrite(green_led2, LOW);
        servocikis.write(0);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("***WELCOMW***");
        lcd.setCursor(0, 1);
        lcd.print("Capacity: " + String(capacity));
      } 
      
      else {
        servocikis.write(0);
        lcd.print("Capacity: 0" + String(capacity));
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("*****WELCOME*****");
        lcd.setCursor(0, 1);
        lcd.print("There is no car.");
        digitalWrite(red_led2, HIGH);
      }
    }
  } else {
    exitCounter = 0;
  }
}