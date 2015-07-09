/*
 * SDuino_sensor, primitive weather forcasting program
 * July 9, 2015 by kodera2t
 * This program requires "Adafruit_MPL115A2 libraly"
 * Forthcoming climate is predicted by the delivative of pressure value
 * of last 2 hours. Every two hours, SDuino_sensor will sense pressure and taking
 * delivative of them. First forcast will be shown 2 hours after power supplied.
 * Result will be shown as:
 *    worse (meaning fine to cloudy, cloudy to rain) by RED LED
 *    moderate (meaning no change) by GREEN LED
 *    better (rain to cloudy, cloudy to fine) by BLUE LED
 *    no data (first 2 hours since power-on, or some parts broken) by WHITE LED
 */

#include <Wire.h>
#include <Adafruit_MPL115A2.h>

Adafruit_MPL115A2 mpl115a2;
#define worse_led 6
#define moderate_led 7
#define better_led 8
#define no_data_led 9


unsigned long time_1;
unsigned long interval=7200000; //two hour interval
int disp;
float pressureKPA=0, old_pressure=0, temperatureC = 0;

void setup(void) 
{
//  Serial.begin(9600);
  pinMode(worse_led, OUTPUT);
  pinMode(moderate_led, OUTPUT);
  pinMode(better_led, OUTPUT);
  pinMode(no_data_led, OUTPUT);

  blink_allLED();
  mpl115a2.begin();
}

void loop(void) 
{
  time_1=millis();
  if((time_1%interval<1200)||(time_1==1000)){
  old_pressure=pressureKPA;
  pressureKPA = mpl115a2.getPressure();  
//  Serial.print("Pressure (kPa): "); Serial.print(pressureKPA); Serial.println(" kPa");
  if(pressureKPA>old_pressure+0.15){
    disp=1;
  }else if(pressureKPA<old_pressure-0.15){
    disp=-1;
  }else{
  disp=0;
  }
  }
//
  if(millis()<7200001){
  disp=2;
  }

  switch(disp){
    case -1:
    led_blink(worse_led, 750);
    break;
    case 1:
    led_blink(better_led, 750);
    break;
    case 0:
    led_blink(moderate_led, 750);
    break;
    case 2:
    led_blink(no_data_led, 750);
    break;
    default:
    led_blink(moderate_led, 750);
    break;
  }
    }



  


void blink_allLED(void){
  int delay_time=20;
 for(int i=1; i<5; i++){
 led_blink(worse_led,delay_time);
 led_blink(moderate_led,delay_time);
  led_blink(better_led,delay_time);
  led_blink(no_data_led,delay_time);
  }
}


void led_blink(int led_num, int delay_time2){
   digitalWrite(led_num, HIGH);   
  delay(delay_time2);              
  digitalWrite(led_num, LOW);    
  delay(delay_time2);              
}
