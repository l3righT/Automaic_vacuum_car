/*------ Install Library ------*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*----- NRF24L01----*/
#define CE_PIN   9  
#define CSN_PIN 10  
const uint64_t pipe = 0xE8E8F0F0E1LL; 
RF24 radio(CE_PIN, CSN_PIN); 
int joystick[6];  
#define JOYSTICK_X A0
#define JOYSTICK_Y A1
#define buttonUp    2
#define buttonRight 3
#define buttonDown  4
#define buttonLeft  5
void setup()   
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
  radio.stopListening();
  
  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonRight,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);
  pinMode(buttonLeft,INPUT_PULLUP);
  
  digitalWrite(buttonUp,LOW);
  digitalWrite(buttonRight,LOW);
  digitalWrite(buttonDown,LOW);
  digitalWrite(buttonLeft,LOW);
}
 
void loop()   /* Runs Continuously */
{
  joystick[0] = analogRead(JOYSTICK_X);
  joystick[1] = analogRead(JOYSTICK_Y);
  joystick[2] = digitalRead(buttonUp);
  joystick[3] = digitalRead(buttonRight);
  joystick[4] = digitalRead(buttonDown);
  joystick[5] = digitalRead(buttonLeft);
  radio.write(joystick, sizeof(joystick)); 
      //(DEBUG)
      /*Serial.print("X =");
      Serial.print(analogRead(JOYSTICK_X));
      Serial.print(" Y = ");  
      Serial.print(analogRead(JOYSTICK_Y));
      Serial.print(" Up = "); 
      Serial.print(digitalRead(buttonUp));
      Serial.print(" Right = "); 
      Serial.print(digitalRead(buttonRight));
      Serial.print(" Down = "); 
      Serial.print(digitalRead(buttonDown));
      Serial.print(" Left = "); 
      Serial.println(digitalRead(buttonLeft));*/
      delay(20);
}
