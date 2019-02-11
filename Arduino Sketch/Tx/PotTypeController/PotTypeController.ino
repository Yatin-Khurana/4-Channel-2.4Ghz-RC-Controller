int pth = A0;
int pph = A2;
int prl = A3;
int pyw = A1;
int tr_th_1 = 2;
int tr_th_2 = 3;
int tr_yw_1 = 4;
int tr_yw_2 = 5;
int tr_ph_1 = 7;
int tr_ph_2 = 6;
int tr_rl_1 = 9;
int tr_rl_2 = A6;
int buzzer = A4;
int led = 0;
int l1 = A5;
int r1 = 10;
bool radioNumber = 1;
#include <SPI.h>
#include "RF24.h"
RF24 radio(7,8);
byte addresses[][6] = {"ADD5B","ABDRA"};//Add Any 5 Bit Address


void setup() {
pinMode(pth,INPUT);
pinMode(prl,INPUT);
pinMode(pyw,INPUT);
pinMode(pph,INPUT);
pinMode(tr_th_1,INPUT_PULLUP);
pinMode(tr_th_2,INPUT_PULLUP);
pinMode(tr_yw_1,INPUT_PULLUP);
pinMode(tr_yw_2,INPUT_PULLUP);
pinMode(tr_ph_1,INPUT_PULLUP);
pinMode(tr_ph_2,INPUT_PULLUP);
pinMode(tr_rl_1,INPUT_PULLUP);
pinMode(tr_rl_2,INPUT_PULLUP);
pinMode(buzzer,OUTPUT);
pinMode(led,OUTPUT);
pinMode(l1,INPUT_PULLUP);
pinMode(r1,INPUT_PULLUP);
radio.begin();
radio.setPALevel(RF24_PA_MAX);
radio.openWritingPipe(addresses[1]);
radio.openReadingPipe(1,addresses[0]);
radio.setDataRate((RF24_2MBPS));
blinke(led,200);
for(int x=0;x<100;x++){
  blinke(buzzer,700);
}
//Serial.begin(9600);
delay(20);
for(int x=0;x<110;x++){
  blinke(buzzer,500);
}
delay(20);
for(int x=0;x<150;x++){
  blinke(buzzer,400);
}
delay(20);
for(int x=0;x<400;x++){
  blinke(buzzer,300);
}
delay(1000);
}


int error_lx=0,error_ly=0,error_rx=0,error_ry=0;
long old_millis = 0;



void loop() {
//buzz();
int lx = analogRead(A0);
int ly = analogRead(A1);
int ry = analogRead(A2);
int rx = (analogRead(A3));
digitalWrite(led,HIGH);
    int L_X = 500 + int(lx)+ int(error_lx);
    int L_Y = 2000 + int(error_ly) + int(ly);
    int R_X = 4000  + int(rx)+ int(error_rx);
    int R_Y = 7000 + int(error_ry) + int(ry);
    //buzz();
    rftransmit(L_X);
    
    rftransmit(L_Y);
    rftransmit(R_X);
    rftransmit(R_Y);
    //buzz();
    
    
    if((millis()-old_millis)>10){
    {
      
        // --------------------------------------------------------
        if(!(digitalRead(2))){
        error_ly = error_ly + 1;
        buzz();
        }
        else if(!(digitalRead(4))){
        error_lx = error_lx + 1;
        buzz();
        }
        else if(!(analogRead(A7)>10)){
        error_ry = error_ry + 1;
        buzz();
        }
        else if(!(digitalRead(9))){
        error_rx = error_rx + 1;
        buzz();
        }


        else if(!(digitalRead(3))){
        error_ly = error_ly - 1;
        buzz();
        }
        else if(!(digitalRead(5))){
        error_lx = error_lx - 1;
        buzz();
        }
        else if(!(digitalRead(6))){
        error_ry = error_ry - 1;
        buzz();
        }
        else if(!(analogRead(A6)>10)){
        error_rx = error_rx - 1;
        buzz();
        }
        // --------------------------------------------------------
      }
      old_millis = millis();
    }
    //digitalWrite(led,LOW);
}
void rftransmit(unsigned long int data)
{
  //Serial.println(data);
  radio.stopListening();                             // Take the time, and send it.  This will block until complete
  radio.write( &data, sizeof(unsigned long int));
  //radio.startListening();
  //Serial.println(data);// Now, continue listening
}
void blinke(int pin,int del){
  digitalWrite(pin,HIGH);
  delayMicroseconds(del);
  digitalWrite(pin,LOW);
  delayMicroseconds(del);
  digitalWrite(pin,HIGH);
  delayMicroseconds(del);
  digitalWrite(pin,LOW);
  delayMicroseconds(del);
}
void buzz(){
  for(int x=0;x<5;x++){
  blinke(buzzer,200);
}
}

