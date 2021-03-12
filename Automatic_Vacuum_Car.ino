/*------ Install Library ------*/
#include <Servo.h>          
#include <NewPing.h>        
#include <SPI.h>
#include "nRF24L01.h"
#include <RF24.h>
/*----- L298N control pins ----*/
const int motor1 = 10;
const int motor2 = 9;
const int motor3 = 2;
const int motor4 = 4;
const int EnableA = 3;      //enable pin for motor 1
const int EnableB = 5;      //enable pin for motor 2
int motorspeed = 255;
/*----- Ultrasonic Sensor HC-SR04P ----*/
#define TRIG_PIN1 A1 
#define ECHO_PIN1 A2 
#define TRIG_PIN2 A4 //analog input 1
#define ECHO_PIN2 A3 //analog input 2
#define MAX_DISTANCE1 100
#define MAX_DISTANCE2 100
boolean goesForward = false;
int distance1 = 100;
int distance2 = 100;
/*----------- sensor function -----------*/
NewPing sonar1(TRIG_PIN1, ECHO_PIN1, MAX_DISTANCE1);
NewPing sonar2(TRIG_PIN2, ECHO_PIN2, MAX_DISTANCE2); 
Servo servo_motor; 
/*----- NRF24L01----*/
#define CE_PIN 6    
#define CSN_PIN 8    
const uint64_t pipe = 0xE8E8F0F0E1LL; 
RF24 radio(CE_PIN, CSN_PIN);       
int joystick[6];  
int buttonstatusA;
int buttonstatusB;
int buttonstatusC;
int buttonstatusD;

#define buzzer A0
#define RELAY 7

void setup(){

  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);
  pinMode(EnableA, OUTPUT);
  pinMode(EnableB, OUTPUT);
  
  servo_motor.attach(A5); //servo pin
  servo_motor.write(85);
  pinMode(buzzer, OUTPUT);
  pinMode(RELAY, OUTPUT);
  
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}

void loop(){
    int distanceRight = 0;
    int distanceLeft = 0;
        delay(100);
    radio.read( joystick, sizeof(joystick) ); 
    int X = joystick[0];
    int Y = joystick[1];  
    int A = joystick[2];
    int B = joystick[3];
    int C = joystick[4];
    int D = joystick[5];
             if (A ==LOW){digitalWrite(buzzer, LOW);}       
        else if (B ==LOW){digitalWrite(buzzer, LOW);}        
        else if (C ==LOW){digitalWrite(buzzer, LOW);}
        else if (D ==LOW){digitalWrite(buzzer, LOW);}
        else {digitalWrite(buzzer, HIGH);} 
        //JOY Start
     if (radio.available()){         
   /*   Serial.print("X =");
      Serial.print(X);
      Serial.print(" Y = ");  
      Serial.print(Y);
      Serial.print(" Up = ");
      Serial.print(A);
      Serial.print(" Right = ");  
      Serial.print(B);
      Serial.print(" Down = ");
      Serial.print(C);
      Serial.print(" Left = ");  
      Serial.println(D);*/
         if(buttonstatusA == 1){
         if( Y < 300 && X > 0 ){
          moveBackward();
    }
    else if( Y > 550 && X > 300 ){
          moveForward();
    }
    
    else if( X > 500 && Y > 0 ){
          turnleft();
    }
    else if( X == 0  && Y > 0 ){
          turnright();
    } 
      else{
          moveStop();
    } 
  }
      if(buttonstatusA == 2){
       //Serial.println("STOP");
        moveStop();
      }
      if(A == 0){
        buttonstatusA++;  
      }
      if(buttonstatusA > 2){
        buttonstatusA = 1;
      }
      //End ButtonA   
    
     }//--end JOY available
     else{
      //Serial.println("notwork");  
      distance1 = readPing1();
        delay(100);
      distance2 = readPing2();
        delay(100);
      distance2 = readPing2();
        delay(100);
      distance1 = readPing1();
        delay(100); 
        delay(50);
  
  if (distance2 <= 20 || distance1 <= 20){
    moveStop();
      delay(1000);
    moveBackward();
      delay(400);
    turnLeft_servo();
      delay(300);
    moveStop();
      delay(300);
    distanceRight = lookRight();
      delay(300);
    distanceLeft = lookLeft();
      delay(300);
      
    if (distance2 >= distanceLeft){
    turnRight_servo();
    moveStop();
    }
    else{
    turnLeft_servo();
    moveStop();
    }
  }
  else{  
  moveForward();
  }
 }
}//-end loop-//

/*-----------------------Function Sensor-----------------------*/
int lookRight(){  
  servo_motor.write(10);
  delay(500);
  int distance2 = readPing2();
  delay(100);
  servo_motor.write(90);
  return distance2;
}
int lookLeft(){
  servo_motor.write(170);
  delay(500);
  int distance2 = readPing2();
  delay(100);
  servo_motor.write(90);
  return distance2;
  delay(100);
}
int readPing1(){
  delay(70);
  int cm = sonar1.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}
int readPing2(){
  delay(70);
  int cm = sonar2.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}
/*-----------------------Function Motor-----------------------*/
void moveStop(){
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, LOW);
  analogWrite(EnableA,0);
  analogWrite(EnableB,0);
}
void moveForward(){
  if(!goesForward){
    goesForward=true;  
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, LOW); 
  analogWrite(EnableA,motorspeed);
  analogWrite(EnableB,motorspeed);
  }
}
void moveBackward(){
  goesForward=false;
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, HIGH);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, HIGH);
  analogWrite(EnableA,motorspeed);
  analogWrite(EnableB,motorspeed);
}
void turnright(){
  digitalWrite(motor1,LOW);                                                                                                                                                                                                                                 
  digitalWrite(motor2,HIGH);
  digitalWrite(motor3,HIGH);
  digitalWrite(motor4,LOW);
  analogWrite(EnableA,0);
  analogWrite(EnableB,motorspeed);
}
void turnleft(){
  digitalWrite(motor1,HIGH);                                                                                                                                                                                                                                 
  digitalWrite(motor2,LOW);
  digitalWrite(motor3,LOW);
  digitalWrite(motor4,HIGH);
  analogWrite(EnableA,motorspeed);
  analogWrite(EnableB,0);
}
void turnRight_servo(){
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, HIGH);
  analogWrite(EnableA,0);
  analogWrite(EnableB,motorspeed);
    delay(500);
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, LOW);
  analogWrite(EnableA,motorspeed);
  analogWrite(EnableB,motorspeed); 
}
void turnLeft_servo(){
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, HIGH);
  digitalWrite(motor3, HIGH); 
  digitalWrite(motor4, LOW);
  analogWrite(EnableA,motorspeed);
  analogWrite(EnableB,0);
    delay(500); 
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, LOW);
  analogWrite(EnableA,motorspeed);
  analogWrite(EnableB,motorspeed);
}
