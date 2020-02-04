/* © Lorenzo D'Auria
 *   Program for controlling a car by receiving an IR signal
*/

#include <IRremote.h>           //IR library
const int IR_PIN = 11;          //IR receiver PIN (11 in this case)
IRrecv irrecv(IR_PIN);          //Initialazing the IR sensor
decode_results results;

#include <Servo.h>              //Initializing servo motor
Servo ser;

#define a 6                     //DC motor PIN connect IN1 to PIN 6 and IN2 to PIN 5 of the H bridge
#define b 5
const int med=88;               //Servo motor calibration; Neutral postion
const int lf=35;                //Full left position
const int rg=135;               //Full right postion

long va=0;                      //DC motor alimentation
long vb=0;
long pos=med;                   //Servo motor position
unsigned long tric;             //Last reception time


void setup() {
  Serial.begin(9600);           //Only for debug or calibration
  pinMode(a,OUTPUT);            //DC motor PIN set up
  pinMode(b,OUTPUT);
  irrecv.enableIRIn();          //Start the IR reception
  irrecv.blink13(true);         //LED 13 blinks when receives data
  ser.attach(9);                //Servo motor PIN (9 in this case)
}


void loop() { 
  if (irrecv.decode(&results)) {                                     //IR sensor receives something
    if(results.decode_type==SONY && results.bits==12){               //Check of the received data
        tric=millis();                                               //Update last reception time
        pos=results.value/0x100;                                     //Calculation of the position of the servo motor
        va=(results.value-pos*0x100)/0x10;                           //Calculation of the speed of the DC motor
        vb=results.value-pos*0x100-va*0x10;
        if (pos>8) pos=(pos-8)*((rg-med)/7)+med;                     //Values adjustment due to servo calibration
        else pos=(pos-8)*((med-lf)/8)+med;
        va=va*17;                                                    //Values adjustment due to the transmission method
        vb=vb*17;
    }
    irrecv.resume();                                                 //IR sensor is ready to receive data again
  }
  
  if(millis()-tric>1000) {va=0; vb=0; pos=med;}                      //Out of signal check (signal is absent for more than 1 second)

  if(va*vb==0){                               //Security check of the calculated data, if correct
    analogWrite(a,va);                        //Control motor as received from the remote
    analogWrite(b,vb);
    ser.write(pos);
  }

  //Serial.println(results.value,HEX);        //Remove "//" to see what the IR sensor has received
  /*
  Serial.print(va);                           //Remove comment to see which values has been calculated
  Serial.print("   ");
  Serial.print(vb);
  Serial.print("   ");
  Serial.println(pos);
  */
}
