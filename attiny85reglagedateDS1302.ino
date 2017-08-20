



#include <DS1302.h>
#include <EEPROM.h>

// Init the DS1302
//VERT JAUNE NOIR
//DS1302 rtc(2, 3, 4);
 //INVERSE  JAUNE VERT NOIR
DS1302 rtc(2, 4, 3);

 int pinLed = 1;////PB1 


void setup(void) {
   
    rtc.halt(false);
   rtc.writeProtect(false);
  rtc.setDOW(FRIDAY);        // Set Day-of-Week to FRIDAY
  delay(1000);
  rtc.setTime(21, 22, 0);     // Set the time to 12:00:00 (24hr format)
  delay(1000);
  rtc.setDate(19, 8, 2017);   // Set the date to August 6th, 2010
   delay(1000);
    Time t;
 t = rtc.getTime();
EEPROM.put( 1,t.hour );  //Write array data.
  EEPROM.put( 2,t.min );  //Write array data.
   EEPROM.put( 3,t.date );  //Write array data.
  EEPROM.put( 4,t.mon );  //Write array data.
   EEPROM.put( 5,t.year );  //Write array data.
 
 while(t.hour != 21 && t.min != 22){
  
 rtc.setTime(21, 22, 0);     // Set the time to 12:00:00 (24hr format)
  delay(1000);
  t = rtc.getTime();
}
  pinMode(pinLed,OUTPUT);
    digitalWrite(pinLed,HIGH);  
   
   
  
  
}
 


     
    
   
    
   
  
  
    
 


void loop(void) {
  
}







 
