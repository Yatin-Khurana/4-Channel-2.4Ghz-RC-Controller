/*
 * *************************************************************************************************************
 * Author - Yatin Khurana
 * Description - Microwave RF Rx Side Transreciever For Veronica and other Applications.
 * Needs to be Updated with left analog X as yaw
 * *************************************************************************************************************
*/
// Output PWM Pins/Pinout
#include <Servo.h>
Servo throttle;
Servo roll;
Servo pitch;
Servo yaw;

// 5V = 5V
//GND
//---------*****---------
/*--------NRF24l01 Pinout---------
 * Nrf24l01 -  SPI Bus - 11,12,23 
 *             plus pins - 7,8(Atmega 16 - PB0,PB1)
 *--------------------------------
 * *----------------------- Integer values/Range for Buttons-----------------------
 * Start - 1
 * Select - 2
 * Pad Up - 500 - 755
 * Pad down - 2000 - 2255
 * Pad left - 1500 - 1755
 * Pad right -1000 - 1255
 * Triangle - 3000 - 3255
 * circle - 3500 - 3755
 * Square - 4000 - 4255
 * Cross - 4500 - 4755
 * Left analog X - 5000 - 5255
 * Left Analog Y - 5500 - 5755
 * Right analog X - 6000 - 6255
 * Right Analog Y - 6500 - 6755
 * L 1 - 7
 * L 2 - 5
 * L 3 - 3
 * R 1 - 8
 * R 2 - 6
 * R 3 - 4
 * ------------------------------------------------------------------------------
 * ----------Controls----------------
 * response senstivity = L1 = ++;L2 = --; this is the multiplication factor of Delay between commands
 * throttle up = Padup
 * Throttle Down = PadDown
 * Roll - Right Analog X
 * Pitch - Left Anlog X
 * Shoot Throttle to Full = R2
 * Lower Throttle to Zero = R1
*/
// NRF24L01 Library
bool radioNumber = 0;
#include <SPI.h>
#include "RF24.h"
RF24 radio(7,8);//CE,SCN
byte addresses[][6] = {"ADD5B","ABDRA"};//Add Any 5 Bit Address
// RGB Leds
int loop_delay = 0;
long int failsafe_counter_not_recieved = 0;
long int failsafe_counter_recieved = 0;
 float throttle_value = 111.0;
 float roll_value = 0.0;
 float pitch_value = 0.0;
 float yaw_value = 90.0;
 float aux1_value = 0.0;
 float aux2_value = 0.0; 
 float mid_value = 129.0;
// Signal's Frequencies
//int response_senstivity = 1000;// should be between 1 to 10 where 1 means faster commands and 10 means slower
//int yaw_freq = 1;
//int roll_freq = 1;
//int pitch_freq = 1;
//// New request times
//int new_throttletime = 0;
//int new_rolltime = 0;
//int new_pitchtime = 0;
//int new_yawtime = 0;
// last trigger times
//int last_throttletime = 0;
//int last_rolltime = 0;
//int last_pitchtime = 0;
//int last_yawtime = 0;
      // multipliers PWM
//int throttle_multiplier = 1;
//int roll_multiplier = 1;
//int pitch_multiplier = 1;
//int yaw_multiplier = 1;
      // Signal Frequencies/Delay
//int throttle_delay = 300;
//int roll_delay = 300;
//int pitch_delay = 300;
//int yaw_delay = 300;
byte serialdebug = 1;
void setup() {
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
   if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  radio.startListening();
  if(serialdebug)
  Serial.begin(250000);
  radio.setDataRate((RF24_2MBPS));
  //pinMode(3,OUTPUT);
  //pinMode(5,OUTPUT);
  //pinMode(6,OUTPUT);
  //pinMode(9,OUTPUT);


}

void loop() {
 long int loop_start = millis();
 float data_string = 0.0;
  
if( radio.available())
{    
      throttle.attach(3,900.0,2100.0);
      roll.attach(5,900.0,2100.0);
      pitch.attach(6,900.0,2100.0);
      yaw.attach(9,900.0,2100.0);
      while (radio.available()) 
      {  // While there is data ready
        radio.read( &data_string, sizeof(float) ); // Get the payload
      //Serial.println(data_string);
      }   
                        
 ///////////////////////////////Smoothing Throttle for DualShock's Signals//////////////////////
if((data_string>=500.0 )&&((data_string)<=(500.0+1024.0) )){
  throttle_value = (data_string - 500.0 );
}
else if((data_string>=4000.0 )&&((data_string)<=5024.0 )){
  roll_value = (data_string - 4000.0 );
}
else if((data_string>=7000.0 )&&((data_string)<=8024.0 )){
  pitch_value = (data_string - 7000.0 );
}
else if((data_string>=2000.0 )&&((data_string)<=3024.0 )){
  yaw_value = (data_string - 2000.0 );
}
 float roll_value1 = ((roll_value/1024.0)*180.0);
 float throttle_value1 = ((throttle_value/1024.0)*180.0);//map(throttle_value,0 ,1024 ,0.0,180.0);
 float pitch_value1 = ((pitch_value/1024.0)*180.0);//map(pitch_value,0 ,1024 ,0.0,180.0);
 float yaw_value1 = (((yaw_value+10.0)/940.0)*180.0);//map(yaw_value,50 ,980 ,0.0,180.0);
roll.write(roll_value1);
//delay(200);
throttle.write(throttle_value1);
//delay(200);
pitch.write(pitch_value1);
//delay(200);
yaw.write(yaw_value1);
//delay(200);
if(serialdebug)
Serial.println(String(throttle_value1) + "," + String(roll_value1) + "," + String(pitch_value1) + "," + String(yaw_value1));
//delay(20);
failsafe_counter_recieved = millis();
}
else{
  failsafe_counter_not_recieved = millis();
  if((failsafe_counter_not_recieved-failsafe_counter_recieved)>=15000){
    roll.detach();
    throttle.detach();
    yaw.detach();
    pitch.detach();
  }
}
}

