/* © Lorenzo D'Auria
 *   Generic code for controlling a brushed DC motor
 *   Inverting PIN+ and PIN- of the motor will only revers the turning direction
 *   You can connect direcly the motor's PINs to Arduino (not reccomended)
 *   For better result use an external power source with an H bridge using his inputs in the same way of the DC motor PINs
*/

int spd;                                //Speed of the motor (from -255 to 255)

void setup() {
  pinMode(5, OUTPUT);                   //PIN + DC motor (In this case PIN+ =5 and PIN- =6 of the Arduino)
  pinMode(6, OUTPUT);                   //PIN - DC motor (Remeber to choose only PINs with PWM regulation, they have a ~ before the number)
  Serial.begin(9600);                   //Only for debug
  spd=200;                              //Change this value for testing different speed or remove it and
                                        //implement the code in the loop for reading the speed from a sensor
}

void loop() {
  
  /*You can put here the code for reading a value of the speed from a sensor (for example a potentiometer)
   * .
   * .
   * .
   * spd=something from -255 to +255
   */
   
  Serial.println(spd);                //Only for debug
  bmotor(spd);                        //The bmotor function can be called in evry part of the loop for setting the new speed of the motor
                                      //It's usefull for converting the single variable of the speed in the correct PWM regulations of the two motor's PINs
  delay(1000);
  spd=-spd;                           //For example here it reverse the speed evry second
}

//Indipendent function that can be used in the same way in other programs
void bmotor(int s){
  
  if (abs(spd)>255)
    s=abs(s)/s;                             //Saturates wrong values to the maximum (less than -255 -> -255 ; more than 255 -> +255)
  int a1,a2;                                //Variable for the PWM of the two motor PINs
  
  //If the speed is negative, PIN+ (controlled by a1) to GND, PIN- (controlled by a2) set as higher has s (only positive value are accepted)
  if (s<0) {
    a1=0;
    a2=abs(s);
    }   
    
  //If positive, PIN- to GND and PIN+ at the voltage set by s (in this case is positive for sure) 
  else {
    a2=0;
    a1=s;
    }      
                               
  analogWrite(5,a1);                        //Set the PWMs calculated to the motor's PINs
  analogWrite(6,a2);
}
