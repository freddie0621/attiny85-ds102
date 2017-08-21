#include <avr/sleep.h>
#include <EEPROM.h>
// This library contains functions to set various low-power 
// states for the ATmega328

#include <DS1302.h>

#include <avr/wdt.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

int pinLed = 1;////PB1
volatile int f_wdt = 0;

// Init the DS1302
//VERT JAUNE NOIR
//DS1302 rtc(2, 3, 4);
// INVERSE  JAUNE VERT NOIR
DS1302 rtc(2, 4, 3);

int compteur=0;
//int count_remplissage=5;
int cycle_remplissage;
int cycletotal=0;
int dureeon=60;// EN MINUTES
int dureeoff=600; // EN SECONDES
int valueread;   //Location we want the data to be put.
int var;

//delay(1000);

void setup(void) {

pinMode(pinLed,OUTPUT);

/////////////////////////////////////
 EEPROM.get(0, cycle_remplissage);
  EEPROM.put(0, (cycle_remplissage+1)%10);
  EEPROM.get(0, cycle_remplissage);
  // put your setup code here, to run once:
  while(var < cycle_remplissage){
   digitalWrite(pinLed,HIGH);  
    delay(300);
    digitalWrite(pinLed, LOW);
    delay(300);
  var++;
}
delay(10000);
/////////////////////
  
   
   //pinMode(pinLed,OUTPUT);
   //7=2 sec, 8=4 sec, 9= 8sec
//setup_watchdog(0); // approximately 16ms sleep
//setup_watchdog(1); // approximately 32ms sleep
//setup_watchdog(2); // approximately 64ms sleep
//setup_watchdog(3); // approximately 128ms sleep
//setup_watchdog(4); // approximately 250ms sleep
//setup_watchdog(5); // approximately 500ms sleep
//setup_watchdog(6); // approximately 1s sleep
//setup_watchdog(7); // approximately 2s sleep
//setup_watchdog(8); // approximately 4s sleep
setup_watchdog(9); // approximately 8s sleep
  
  // memset( numbers, 0, sizeof(numbers) ); //Clear testing array.
 //EEPROM.get( address, numbers );  //Read back array data.

//Serial.begin(9600);
    //rtc.halt(false);
  rtc.writeProtect(false);
  //rtc.setDOW(MONDAY);        // Set Day-of-Week to FRIDAY
  rtc.setTime(0,0, 0);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(1, 7, 2017);   // Set the date to August 6th, 2010
   


}

void loop(void) {



    
// CODE TO BE EXECUTED PERIODICALLY

 Time t;

 t = rtc.getTime();

 wdt_reset(); 

//if (long (t.date*86400+ t.hour*3600 +(t.min%1)*60+t.sec -86400 )<=cycle_remplissage*dureeon &&  long(t.date*86400+t.hour*3600 +(t.min%1)*60+t.sec -86400)>=0) 
if ( ((t.hour)*60 +t.min  )<=cycle_remplissage*dureeon &&  ((t.hour)*60 +t.min+t.sec )>=0) 
   {
    
    // sleep_disable();    
    pinMode(pinLed,OUTPUT);
    //digitalWrite(pinLed,HIGH);  
    analogWrite(pinLed,60);//de 0 à 255
   // analogWrite(pinLed,120);//de 0 à 255
   }

//else if ((cycle_remplissage*dureeon <long (t.date*86400+t.hour*3600 +t.min*60+t.sec-86400))  && long (t.date*86400+t.hour*3600 +t.min*60+t.sec-86400)<(cycle_remplissage*dureeon+10))
else if ((cycle_remplissage*dureeon <((t.hour)*60 +t.min))  && ((t.hour)*60 +t.min)<(cycle_remplissage*dureeon+2))
   {
    EEPROM.put(0, 0);// remise à 0 du compteur de cycle
    //digitalWrite(pinLed, LOW); 
    pinMode(pinLed,INPUT); // set all used port to intput to save power
    system_sleep();
    //sleep_disable();  // deep sleep until WDT kicks
   }
 
  else{
     
     
    //digitalWrite(pinLed, LOW); 
    pinMode(pinLed,INPUT); // set all used port to intput to save power
    system_sleep();
   // sleep_disable();  // deep sleep until WDT kicks

  }



}





// set system into the sleep state 
// system wakes up when wtchdog is timed out
void system_sleep() {
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();

  sleep_mode();                        // System sleeps here

  sleep_disable();                     // System continues execution here when watchdog timed out 
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
}

// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {
 
  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;

  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
 // WDTCR |= _BV(WDTIE);// ATTINY13 seulement
  WDTCR |= _BV(WDIE);// ATTINY85 seulement
}
  
// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
   //f_wdt=1;  // set global flag
  //
 // f_wdt++;  // set global flag
}

float hourMinuteToHour(int hour, int minute){
  //return hour + minute/60.0f;
return hour%24 + minute/60.0f;
}







 
