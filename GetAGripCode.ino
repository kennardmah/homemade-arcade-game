// include libraries
#include <Servo.h>

// define constants
#define stepsPerRevolution 200

// define servos

Servo Servo1;
Servo Servo2;

// define pin numbers

const int limitSw_L = 2;     // limitswitches
const int limitSw_R = 3;     
const int servoPin_R = 4;     // claw servos
const int servoPin_L = 5;
const int redPin = 6;
const int greenPin = 7;
const int stepPin = 8;       // step
const int dirPin = 9;        // direction of horizontal motor
const int startButton = 10;  // start button
const int trigPin = 11;      // ultra sonic sound sensor
const int echoPin = 12;      // 


// variables:
long duration;
int distance;          // distance from ultra sonic sensor in cms
int motorSpeed;        // speed the horizontal motor moves
int start_time;
int current_time;

void setup() {
  Serial.begin(9600);
  
  // horizontal movements
  pinMode(startButton, INPUT);
  pinMode(limitSw_L, INPUT);
  pinMode(limitSw_R, INPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // lights
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  RG_color(50, 50);
  
  digitalWrite(dirPin, LOW);

  // user interface
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // claw servos
  Servo1.attach(servoPin_R);
  Servo2.attach(servoPin_L);
  delay(1000);
  Servo1.write(90);
  Servo2.write(0);
}

void loop() {
    
  Serial.println("Press When Ready");

  while (digitalRead(startButton) == LOW){
  RG_color(10, 10);
  }

  // timer
  start_time = millis();
  current_time = millis() - start_time;
  
  // starting lights
  RG_color(0, 255);
  Serial.println("3");
  delay(500);
  RG_color(0, 0);
  delay(500);
  Serial.println("2");
  RG_color(0, 255);
  delay(500);
  RG_color(0, 0);
  delay(500);
  Serial.println("1");
  RG_color(0, 255);
  delay(500);
  RG_color(0, 0);
  delay(500);
  RG_color(0, 255);
  Serial.println("GO!");

  horizontalMovement(20000);
  
  // LED turns red
  RG_color(255, 0);
  
  Serial.println("claw grabbing initiate");
  
  // claw grabbing movement
  start_time = millis();
  current_time = millis() - start_time;
  
  while (current_time < 3000){
    Servo1.write(0);
    Servo2.write(90);
    current_time = millis() - start_time;
  }

  // returning to starting position
  while(digitalRead(limitSw_L) == HIGH){ // move left until touches limit sensor
    digitalWrite(dirPin, HIGH);
    motorSpeed = 4000;
    motorStep(100);
  }
  
  // claw retracting movement
  Serial.println("claw retracting initiate");
  
  // timer
  start_time = millis();
  current_time = millis() - start_time;
  
  while (current_time < 5000){
    Servo1.write(90);
    Servo2.write(0);
    current_time = millis() - start_time;
  }
  start_time = millis();
  current_time = millis() - start_time;
}

void horizontalMovement(int  timeCount){
  while (current_time < timeCount){
    digitalWrite(trigPin, LOW); // cleans trigpin condition
    delay(2);
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    if( digitalRead(limitSw_L) == HIGH && distance > 30){ // if user is over 30 cms away, and left switch is not pressed move left
      digitalWrite(dirPin, HIGH);
      motorSpeed = 4000;
      motorStep(100);
    }
    else if( digitalRead(limitSw_R) == LOW){ // if right limit switch is pressed, change directions
      digitalWrite(dirPin, HIGH);
      motorSpeed = 4000;
      motorStep(100);
    }
    else if( digitalRead(limitSw_R) == HIGH && distance < 31){ // if user is close move right
      digitalWrite(dirPin, LOW);
      motorSpeed = 2000 + distance * 50; // faster if the distance is shorter
      motorStep(100);
    }
 
    current_time = millis() - start_time;
      
    if (current_time > 10000){ // turn yellow with 10 seconds left
      RG_color(100, 40);
    }
    if (current_time > 19000){ // turn yellow with 10 seconds left
      RG_color(0, 0);
    }
    else if (current_time > 18000){
      RG_color(100, 40);
    }
    else if (current_time > 17000){ // turn yellow with 10 seconds left
      RG_color(0, 0);
    }
    else if (current_time > 16000){
      RG_color(100, 40);
    }
  }
}

void motorStep(int MAX){
  for(int x = 0; x < MAX; x++){
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(motorSpeed);
  }
}

void RG_color(int redValue, int greenValue){
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
}
