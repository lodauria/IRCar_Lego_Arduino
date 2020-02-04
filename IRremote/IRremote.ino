/* © Lorenzo D'Auria
 *   Program for controlling a car with the wii nunchuk by sending an IR signal
*/

#include <IRremote.h> 
IRsend irsend;                                //IR LED initialization (LED IN SERIES WITH 100 OHM RESISTOR CONNECTED TO PIN 3)

#include <Wire.h>
#include <ArduinoNunchuk.h>
ArduinoNunchuk nunchuk = ArduinoNunchuk();    //Wii nunchuk initialization

const float xmin=36;                          //Joystick calibration:
const float xmed=127;                         //min= minumum value; max= max value; med= medium value
const float xmax=216;                         //x= left-right movement; y= up down movement
const float ymin=45;                          //Use ArduinoNunchukDemo.ino in the ArduinoNunchuk library for more information
const float ymed=134;
const float ymax=223;

const float accxmin=310;                      //Accelerometer calibration
const float accxmed=500;                      //Same of joystick calibration
const float accxmax=690;
const float accymin=370;
const float accymed=500;
const float accymax=630;

long va,vb,ylet,xlet,pos;          //Value of A+ and A- of the DC motor, Y read, X read, servo motor position
int stpz,staz;                     //Previous Z botton value, actual Z botton value
boolean mod=true;                  //Actual read modality: TRUE=joysyick, FALSE=accelerometer
int led=13;                        //LED for knowing the actual modality
long IRcode;                       //Hexadecimal code to send


void setup() {
  Serial.begin(9600);              //Only for debug or calibration
  nunchuk.init();                  //Nunchuk initialization
  pinMode(led,OUTPUT);             //PIN mode for the actual modality LED
}


void loop() {
  nunchuk.update();                               //Read data from nunchuk
  staz=nunchuk.zButton;                           //Read actual Z botton value
  if(stpz-staz==1) mod=!mod;                      //If it has been pressed, invert the actual modality

  if (mod==true){                                 //Reading from joystick
    ylet=nunchuk.analogY;                         //Save X and Y actual value
    xlet=nunchuk.analogX;
    va=15/(ymin-ymed)*(ylet-ymed);                //Linear functions for calculating the value to send based on the calibration
    vb=15/(ymax-ymed)*(ylet-ymed);
    pos=16/(xmax-xmin)*(xlet-xmin);
    digitalWrite(led,LOW);                        //LED is OFF when reading from joystick
  }
  
  else{                                           //Read data from accelerometer
    xlet=nunchuk.accelX;                          //Same logic of the part before
    ylet=nunchuk.accelY;
    va=15/(accymin-accymed)*(ylet-accymed);
    vb=15/(accymax-accymed)*(ylet-accymed);
    pos=16/(accxmax-accxmin)*(xlet-accxmin);
    digitalWrite(led,HIGH);
  }
  
  if(va<0) va=0;                                  //Wrong data correction due to a wrong calibration and linear function property
  if(va>15)va=15;
  if(vb<0) vb=0;
  if(vb>15)vb=15;
  if(pos<0)pos=0;
  if(pos>15)pos=15;

  IRcode=pos*0x100+va*0x10+vb;                    //Generation of the hexadecimal string to send 
  irsend.sendSony(IRcode, 12);                    //IR data sending using the Sony standard
  delay(100);
  irsend.sendSony(IRcode, 12);
  delay(100);
  irsend.sendSony(IRcode, 12);
  delay(50);

  stpz=staz;                                      //Previous Z botton value update
  
  //Serial.println(IRcode,HEX);                   //Remove comment to see the Hexadecimal string that has been sent
  /*
  Serial.print(va);                               //Remove comment to see which values has been calculated
  Serial.print("   ");
  Serial.print(vb);
  Serial.print("   ");
  Serial.println(pos);
  delay(100);
  */
}
