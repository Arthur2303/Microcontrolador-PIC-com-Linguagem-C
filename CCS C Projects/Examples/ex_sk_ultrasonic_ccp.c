///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                       ex_sk_ultrasonic_ccp.c                      ////
////                                                                   ////
//// This is similar to ex_sk_ultrasonic.c, but CCP is used to capture ////
//// the timer value when an echo is received.                         ////
////                                                                   ////
//// Since the CCP1 input on the PIC18F14K50 of the E3 is tied to the  ////
//// RB5 pin, then the wiring to the ultrasonic sensor needs to be:    ////
//// VCC to +5, Gnd to G, Trig to B7, Echo to C5.                      ////
////                                                                   ////
//// When the example code is running, the sensor will output to the   ////
//// SIOW the distance of an object infront of it in inches and        ////
//// centimeters.                                                      ////
////                                                                   ////
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

#define PIN_ULTRASONIC_TRIGGER PIN_B7    //output
#define PIN_ULTRASONIC_ECHO    PIN_C5    //input

#if (PIN_ULTRASONIC_ECHO != PIN_C5)
   #error This example uses the CCP1 peripheral on PIN_C5 of the PIC18F14K50.  That means you cannot change this pin!
#endif

void main(void)
{
   unsigned int16 ticks;
   float duration;
   float cm; 
   float inches;
   
   printf("\r\n\r\nex_sk_ultrasonic.c - example starting\r\n\r\n");
   
   enable_interrupts(GLOBAL);
   
   // 16bit timer.
   // if div8, then will tick at 0.667us and overflow at 43.69ms.
   // math above assumes 48MHz (12MIPS) operation
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
   #define TICKS_PER_SECOND   (1500000)   // 1/0.667us
   
   // configure CCP1, connected to PIN_C5, to capture timer on falling edge
   setup_ccp1(CCP_CAPTURE_FE);
   
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
      
      set_timer1(0); // clear timer
      
      clear_interrupt(INT_CCP1); //clear CCP1 interrupt flag
      
      // wait for CCP1 interrupt.  CCP1 will interrupt when PIN_ULTRASONIC_ECHO
      // goes low due to CCP_CAPTURE_FE passed to setup_ccp1()
      while(!interrupt_active(INT_CCP1));
      
      // CCP_1 is a 16bit SFR of the PIC18F14K50 and holds the timer1 tick
      // count when the echo pin went low (because of CCP_CAPTURE_FE passed to 
      // setup_ccp1())
      ticks = CCP_1;    //get time
      
      //based on speed of sound in m/s converted to cm and inches per us
      duration = (float)ticks/(float)TICKS_PER_SECOND;
      duration /= (0.000001);
      cm = duration/58.0;
      inches = duration/148.0;

      printf("ticks=%lu, duration=%fus, centimeters=%g, inches=%g\r\n", ticks, duration, cm, inches);
   }
}

