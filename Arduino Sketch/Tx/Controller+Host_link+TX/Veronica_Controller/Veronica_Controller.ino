 /*
 * *************************************************************************************************************
 * Author - Yatin Khurana
 * Description - Microwave RF Transreciever Controller Using DualShock 2 For Veronica and other Applications.
 * *************************************************************************************************************
 *   ---------------------------------Complete Pinout---------------------------
 * LEDs- Red - A1 // For Radio
 *       Green - A2 // For Controller Configured
 *       Blue - A3 // For data
 * DualShock 2 - Clock - 3
 *               Command- 4
 *               Attention- 5
 *               Data- 2
 * Nrf24l01 -  SPI Bus - 11,12,13 
 *             plus pins - 9,10
 *-------------------------------------------------------------------------------
 *----------------------- Integer values/Range for Buttons-----------------------
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
*/
bool radioNumber = 1;
#include <SPI.h>
#include "RF24.h"
#include <PS2X_lib.h>
PS2X ps2x;

// For DualShock 2
int error = 0; 
byte type = 0;
byte vibrate = 0;
// Status LEDs for Transmission
int green = 15;
int red = 16;
int blue = 17;
int v1 =0;
int v2 =0;
int v3=0;
float centre_l_x = 0; // Centre values for sticks
float centre_l_y = 0;
float centre_r_x = 0; // Centre values for sticks
float centre_r_y = 0;
float error_lx;
float error_ly;
float error_rx;
float error_ry;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);
controller_type = 1; // 0 for dual shock, 1 for mannual buttons.
/****************************************************************************/

byte addresses[][6] = {"ADD5E","ABDRC"};//{"3Node","7Node"};//{"2NODE","5NODE"};//
void buzz
void setup()
{
  // Print a message to the Serial.
  Serial.print("Initializing.");
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  Serial.begin(250000);
  error = ps2x.config_gamepad(5,4,3,2, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate((RF24_2MBPS));
   
  //Serial.print("Connecting DS2");
 if(error == 0){
   Serial.println("CCNE");//Controller Configured and No Error
   blinke(green,100);
    
    
  //Serial.print("DualShock2");
   
  //Serial.print("Found");
 }
   
  else if(error == 1){
   Serial.println("E02");//No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips
   blinke(red,100);
    
    
  //Serial.print("No Controller");
   
  //Serial.print("Found");
  }
  else if(error == 2){
   Serial.println("E01"); //Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips
   blinke(red,100);
    
    
 // Serial.print("Controller Not");
   
  //Serial.print("Accepting Cmds");
  }
  else if(error == 3){
   Serial.println("E00"); // Error for not supporting Presure Modes
   blinke(red,100);
    
    
  //Serial.print("DualShock2");
   
  //Serial.print("Found W/O PMode");
  }
   type = ps2x.readType(); 
     switch(type) {
       case 0:
        Serial.println("Unknown Controller type");
        blinke(red,100);
         
         
         
       // Serial.print("unknwn Controller");
         
        //Serial.print("Found");
        digitalWrite(green,LOW);
       break;
       case 1:
        Serial.println("DualShock Controller Found");
        blinke(green,25);
         
         
        //Serial.print("DualShock 2");
         
        //Serial.print("Ready");
        digitalWrite(green,HIGH);
       break;
       case 2:
         Serial.println("UnSupported Controller");
         blinke(red,100);
         digitalWrite(green,LOW);
          
          
        //Serial.print("unknwn Controller");
         
        //Serial.print("Found");
       break;
     }
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  radio.startListening();
   
   
  Serial.print("Microwave Radio");
  Serial.print("Ready");
        digitalWrite(green,HIGH);
        digitalWrite(blue,HIGH);
        delay(1000);
        ps2x.read_gamepad(false, vibrate);
        /*
        centre_l_x = ps2x.Analog(PSS_LX);
        centre_r_x = ps2x.Analog(PSS_RX);
        centre_l_y = ps2x.Analog(PSS_LY);
        centre_r_x = ps2x.Analog(PSS_RX);
        error_lx = 127.25 - centre_l_x;
        error_rx = 127.25 - centre_r_x;
        error_ly = 127.25 - centre_l_y;
        error_ry = 127.25 - centre_r_y;
        */
        digitalWrite(blue,HIGH);
        }
void loop(){
  if((type == 1 || type == 2))// If DualShock2 is Found Then Continue
  {
     ps2x.read_gamepad(false, vibrate);
    
    // Read Each and Every Value of Buttons
    // Single value
    if(ps2x.Button(PSB_START)) {                  //will be TRUE as long as button is pressed
         Serial.println("1");
         rftransmit(1);
    }
    if(ps2x.Button(PSB_SELECT)){
         Serial.println("Select is being held");
         rftransmit(2);
  }
  if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
       int value = 500 + (ps2x.Analog(PSAB_PAD_UP)); // Up Values will be in range 500-
       Serial.println(value);
       rftransmit(value);
      }
  if(ps2x.Button(PSB_PAD_RIGHT)) {         //will be TRUE as long as button is pressed
       int value = 1000 + (ps2x.Analog(PSAB_PAD_RIGHT));
       Serial.println(value);
       rftransmit(value);
      }
  if(ps2x.Button(PSB_PAD_LEFT)) {         //will be TRUE as long as button is pressed
       int value = 1500 + (ps2x.Analog(PSAB_PAD_LEFT));
       Serial.println(value);
       rftransmit(value);
      }
 if(ps2x.Button(PSB_PAD_DOWN)) {         //will be TRUE as long as button is pressed
       int value = 2000 + (ps2x.Analog(PSAB_PAD_DOWN));
       Serial.println(value);
       rftransmit(value);
      }
 if(ps2x.Button(PSB_PAD_RIGHT)) {         //will be TRUE as long as button is pressed
       int value = (2500 + (ps2x.Analog(PSAB_PAD_RIGHT)));
       Serial.println(value);
       rftransmit(value);
      }
        if(ps2x.Button(PSB_L3)){
         Serial.println(3);
         error_lx = 0;
        error_rx = 0;
        error_ly = 0;
        error_ry = 0;
        blinke(green,50);
        digitalWrite(green,HIGH);
         rftransmit(3);
         }
        if(ps2x.Button(PSB_R3)){
         Serial.println(4);
         rftransmit(4);
         }
        if(ps2x.Button(PSB_L2)){
         Serial.println(5);
         rftransmit(5);
         }
        if(ps2x.Button(PSB_R2)){
         Serial.println(6);
        rftransmit(6);
        }
        if(ps2x.Button(PSB_L1)){
         Serial.println(7);
         rftransmit(7);
         }
        if(ps2x.Button(PSB_R1)){
         Serial.println(8);
        rftransmit(8);
        }
////////////////////////////////////// Circle Triangle Square X////////////////////////////////////
if(ps2x.Button(PSB_GREEN)) {         //will be TRUE as long as button is pressed
       int value = 3000 + (ps2x.Analog(PSAB_GREEN));
       float L_X = 5000 + 127.5;
       float L_Y = 5500 + 127.5;
       float R_X = 6000 + 127.5;
       float R_Y = 6500 + 127.5;
       Serial.println(value);//Trinagle
       rftransmit(value);
      }
      else{
        // Axis LX
       float v_lx = 127.5; 
       float V_lx = (ps2x.Analog(PSS_LX));
       if(V_lx>=140){
        v_lx = V_lx - 10;
       }
       else if(V_lx<=115){
        v_lx = V_lx + 10;
       }
       else{
        v_lx = 127.5;
       }
       // AXIS LY
       float v_ly = 127.5; 
       float V_ly = (ps2x.Analog(PSS_LY));
       if(V_ly>=140){
        v_ly = V_ly - 10;
       }
       else if(V_ly<=115){
        v_ly = V_ly + 10;
       }
       else{
        v_ly = 127.5;
       }
       //Axis RX
       float v_rx = 127.5; 
       float V_rx = (ps2x.Analog(PSS_RX));
       if(V_rx>=140){
        v_rx = V_rx - 10;
       }
       else if(V_rx<=115){
        v_rx = V_rx + 10;
       }
       else{
        v_rx = 127.5;
       }
       //AXIS Ry
       float v_ry = 127.5; 
       float V_ry = (ps2x.Analog(PSS_RY));
       if(V_ry>=140){
        v_ry = V_ry - 10;
       }
       else if(V_lx<=115){
        v_ry = V_ry + 10;
       }
       else{
        v_ry = 127.5;
       }
       // Ends here
    float L_X = 5000 + error_lx + v_lx;
    float L_Y = 5500 + error_ly + v_ly;
    float R_X = 6000 + error_rx + v_rx;
    float R_Y = 6500 + error_ry + v_ry;
    rftransmit(L_X);
    rftransmit(L_Y);
    rftransmit(R_X);
    rftransmit(R_Y);
    v1 = (255-((R_Y-6500))/0.7);
      }
  if(ps2x.Button(PSB_RED)) {         //will be TRUE as long as button is pressed
       int value = 3500 + (ps2x.Analog(PSAB_RED));
       Serial.println(value);//Circle
       rftransmit(value);
      }
  if(ps2x.Button(PSB_PINK)) {         //will be TRUE as long as button is pressed
       int value = 4000 + (ps2x.Analog(PSAB_PINK));
       Serial.println(value);//Square
       // If square is pressed centre analog sticks
       centre_l_x = ps2x.Analog(PSS_LX);
        centre_r_x = ps2x.Analog(PSS_RX);
        centre_l_y = ps2x.Analog(PSS_LY);
        centre_r_x = ps2x.Analog(PSS_RX);
        error_lx = 127.5 - centre_l_x;
        error_rx = 127.5 - centre_r_x;
        error_ly = 127.5 - centre_l_y;
        error_ry = 127.5 + centre_r_y;
        blinke(red,50);
        //----------------------------------------
       rftransmit(value);
      }  
  if(ps2x.Button(PSB_BLUE)) {         //will be TRUE as long as button is pressed
       int value = (4500 + (ps2x.Analog(PSAB_BLUE)));//X
       rftransmit(value);
       Serial.println(value);
      }
      ps2x.read_gamepad(false, vibrate);
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////Analog Sticks//////////////////////////////////////////


}

int v = 0;//v1+v2+v3;
if(v>255){
  vibrate =255;
  }
  else{
    vibrate = v;
  }
/*if( radio.available()){    
  // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read(&vibrate, sizeof(unsigned long) ); 
        Serial.println("Vibrate ="+ String(vibrate));// Get the payload
      }
}*/
delay(1);
}

void blinke(int pin,int del){
  digitalWrite(pin,HIGH);
  delay(del);
  digitalWrite(pin,LOW);
  delay(del);
  digitalWrite(pin,HIGH);
  delay(del);
  digitalWrite(pin,LOW);
  delay(del);
}
void rftransmit(unsigned long data)
{
  Serial.println(data);
  radio.stopListening();                             // Take the time, and send it.  This will block until complete
  radio.write( &data, sizeof(unsigned long));
  blinke(blue,1);
  radio.startListening();
  //Serial.println(data);// Now, continue listening
}

