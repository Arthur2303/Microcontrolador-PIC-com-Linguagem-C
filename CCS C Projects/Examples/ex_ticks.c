/////////////////////////////////////////////////////////////////////////
////                             EX_TICKS.C                          ////
////                                                                 ////
////  This program displays a message over the RS-232 and waits for  ////
////  any keypress to continue.  The program then blinks two         ////
////  different LEDs at different rates.  This is similar to         ////
////  ex_sqw.c, but uses the #use timer() library to create a        ////
////  tick timing system.                                            ////
////                                                                 ////
////  Comment out the printf's and getc to eliminate the RS232 and   ////
////  just output a square wave.                                     ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCB, PCM, PCD, and PCH         ////
////  compilers. The following conditional compilation lines are     ////
////  used to include a valid device for each compiler.  Change the  ////
////  device, clock and RS232 pins for your hardware if needed.      ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2010,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCB__)
#include <16F57.h>
#elif defined(__PCM__)
#include <16F887.h>
#elif defined(__PCD__)
#include <30F2010.h>
#elif defined(__PCH__)
#include <18F4520.h>
#endif

#use delay(crystal=20mhz)
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART
#define LED_1  PIN_B5
#define LED_2  PIN_B4


#use timer(tick=50ms, bits=32, NOISR)
typedef unsigned int32 tick_t;

void main() {
   tick_t current, led1, led2;
   
   printf("Press any key to begin\n\r");
   getc();
   printf("Running, %f ticks a second\n\r", 19.073049/*Ticks per second*/);
   
   set_ticks(0);  //not required
   led1 = 0;
   led2 = 0;

   while (TRUE) 
   {
      current = get_ticks();
      
      if ((current - led1) > (TICKS_PER_SECOND/4))
      {
         led1 = current;
         output_toggle(LED_1);
      }

      if ((current - led2) > (TICKS_PER_SECOND/6))
      {
         led2 = current;
         output_toggle(LED_2);
      }
   }
}
