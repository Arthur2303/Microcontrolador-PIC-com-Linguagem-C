///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          ex_sk_ultrasonic.c                       ////
////                                                                   ////
//// Example code showing how to use the HC-SR04 ultrasonic sensor     ////
//// to determine distance of objects. Example code                    ////
//// developed to run on the CCS E3 Mini development kit.              ////
//// Sensor module should be connected to a breadboard.                ////
//// VCC to +5, Gnd to G, Trig to B7, Echo to B6.                      ////
////                                                                   ////
//// This module uses sonar to detect distance by sending a pulse and  ////
//// measuring the time it takes to get a response (echo).             ////
////                                                                   ////
//// When the example code is running, the sensor will output to the   ////
//// SIOW the distance of an object infront of it in inches and        ////
//// centimeters.                                                      ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                                                                   ////
//// http://www.ccsinfo.com                                            ////
///////////////////////////////////////////////////////////////////////////

#include <e3.h>

#USE TIMER(TIMER=1, TICK=1us, BITS=16, ISR)

#define PIN_ULTRASONIC_TRIGGER PIN_B7    //output
#define PIN_ULTRASONIC_ECHO    PIN_B6    //input

void main(void)
{
   unsigned int16 ticks;
   float duration;
   float cm; 
   float inches;
   
   printf("\r\n\r\nex_sk_ultrasonic.c - example starting\r\n\r\n");
   
   enable_interrupts(GLOBAL);
   
   while(1)
   {  
      delay_ms(1000);
      
      //The sensor is triggered by a HIGH pulse of 10 or more microseconds.
      //Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
      output_low(PIN_ULTRASONIC_TRIGGER);
      delay_us(6);
      output_high(PIN_ULTRASONIC_TRIGGER);
      delay_us(12);
      output_low(PIN_ULTRASONIC_TRIGGER);      
      
      while(!input(PIN_ULTRASONIC_ECHO)); //wait for pin to go high
      
      set_ticks(0);  //start timing
      
      while(input(PIN_ULTRASONIC_ECHO));  //wait for pin to go low
      
      ticks = get_ticks(); //get time
      
      //based on speed of sound in m/s converted to cm and inches per us
      duration = (float)ticks/(float)TICKS_PER_SECOND;
      duration /= (0.000001);
      cm = duration/58.0;
      inches = duration/148.0;

      printf("ticks=%lu, duration=%fus, centimeters=%g, inches=%g\r\n", ticks, duration, cm, inches);
   }
}

