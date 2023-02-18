//Programma per la rilevazione e la trasmissione dei dati del Joystick (Shield v1.a Funduino) da usare come telecomando

#include <IRremote.h>             //Libreria per la trasmissione infrarossi
IRsend irsend;                    //Utilizzo della libreria in modalità di invio dei segnali, LED infrarossi collegato necessariamente al PIN=3

const byte PIN_X = 0;             //PIN per la coordinata orizzontale del joystick
const byte PIN_Y = 1;             //Coordinata verticale

//Calibrazione joystick: i valori corrispondono a quelli che il Joystick trasmette quando si trova in posizione
const float x_max = 983;          //orizzontale - tutto a destra
const float x_med = 492;          //orizzontale - centro
const float x_min = 0;            //orizzontale - tutto a sinistra
const float y_max = 1023;         //verticale - tutto giù
const float y_med = 499;          //verticale - centro
const float y_min = 0;            //verticale - tutto giù

long va,vb,pos;                   //Dati per il voltaggio dei morsetti del motore, posizione del servo motore (long per evitare errori di overflow durante i calcoli successivi)
long xread,yread;                 //Attuale posizione verticale (y) e orizzontale (x) del joystick
long hexIR;                       //Stringa esadecimale da inviare tramite infrarossi
 
void setup() {
}

void loop() {
  
  xread = analogRead(PIN_X);                      //Lettura dati attuali del joystick
  yread = analogRead(PIN_Y);

  va=15/(y_min-y_med)*(yread-y_med);              //Calcolo delle variabili per il controllo dei motori mediante funzioni lineari con parametri i dati di calibrazione
  vb=15/(y_max-y_med)*(yread-y_med);              //va=0 per yread=y_med, va=15 per yread=y_min; vb=0 per yread=y_med, vb=15 per yread=y_max 
  pos=16/(x_max-x_min)*(xread-x_min);             //pos=8 per xread=x_med, pos=0 per xread=x_min, pos=16 per xread=x_max (16 per avere il valore 8 come centrale)

  if(va<0) va=0;                                  //Correzzione valori non accettati dovuti alla struttura delle funzioni ed eventuali calibrazioni sbagliate
  if(va>15) va=15;                                //Necessaria per esser sicuri di avere ogni dato di massimo 4 bit
  if(vb<0) vb=0;
  if(vb>15) vb=15;
  if(pos<0) pos=0;
  if(pos>15) pos=15;                              //Inevitabilmente pos diventa asimmetrico (7 gradazioni per girare a destra, 8 a sinistra)

  /*A questo punto le varibili di controllo dei motori rispondono alla posizione del joystick come sotto 
   * (possono presentarsi anche valori intermedi tra quelli riportati)
   * 
   *       pos   va   vb     xread  yread     Posizione joystick                Movimento auto:
   *         8    0    0     x_med  y_med     Joystick rilasciato               Auto ferma, ruote dritte
   *         0    0    0     x_min  y_med     Joystick a tutto a sinistra       Auto ferma, ruote tutto a sinistra
   *        15    0    0     x_max  y_med     Joystick a tutto a destra         Auto ferma, ruote tutto a destra
   *         8   15    0     x_med  y_min     Joystick tutto giù                Auto all'indietro a massima velocità, ruote dritte
   *         8    0   15     x_med  y_max     Joystick tutto su                 Auto in avanti a massima velocità, ruote dritte
   */

  hexIR=pos*0x100+va*0x10+vb;                     //Creazione stringa esadecimale del tipo |pos|va|vb| (errori di overflow in caso di varibili di tipo int)

  for(int i=0; i<3; i++){ 
    irsend.sendSony(hexIR, 12);                   //Invio stringa esadecimale tramite infrarossi secondo lo standard Sony
    delay(100);
  }
}
