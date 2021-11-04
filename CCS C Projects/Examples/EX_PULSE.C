///////////////////////////////////////////////////////////////////////////
////                           EX_PULSE.C                              ////
////                                                                   ////
////  This program uses the RTCC (timer0) to time a single pulse       ////
////  input to the PIC.                                                ////
////                                                                   ////
////  Configure the CCS prototype card as follows:                     ////
////     Connect pulse generator to pin B1                             ////
////                                                                   ////
////  This example will work with the PCB, PCM, PCH and PCD compilers. ////
////  The following conditional compilation lines are used to          ////
////  include a valid device for each compiler.  Change the device,    ////
////  clock and RS232 pins for your hardware if needed.                ////
///////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2003,2018 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F887.h>
#elif defined(__PCB__)
#include <16F57.h>
#elif defined(__PCH__)
#include <18F4580.h>
#elif defined(__PCD__)
#include <30F2010.h>
#endif
#use delay(crystal=20mhz)
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART


#include <ctype.h>

char get_scale() {
   char scale;

   do {
     printf("\n\rPress S for short or L for long: ");
     scale = getc();
     scale = toupper(scale);
   } while ( (scale!='S') && (scale!='L') );

   return(scale);
}

void wait_for_low_to_high() {

   while(input(PIN_B1)) ;       /* if it's high, wait for a low */

   delay_us(3);                 /* account for fall time */

   while(!input(PIN_B1));       /* wait for signal to go high */
}

void wait_for_low() {

   delay_us(3);                 /* account for rise time */

   while(input(PIN_B1));        /* wait for signal to go low */
}


void main() {
   
   char scale;
   int16 time;

   while (TRUE) {
   
      scale = get_scale();
      if(scale == 'S')
      {
         /* The user specified the smaller timer division */
         setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
      }
      else
      {
         /* The user specified the larger timer division */
         setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
      }
      
      wait_for_low_to_high();
      set_timer1(0);
      wait_for_low();
      time = get_timer1();

      printf("Counter value: %2X\n\n\r", time);

   }           
}
