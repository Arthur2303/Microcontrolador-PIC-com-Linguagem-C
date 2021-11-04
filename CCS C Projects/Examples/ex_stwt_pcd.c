/////////////////////////////////////////////////////////////////////////
////                         EX_STWT_PCD.C                           ////
////                                                                 ////
////  This program uses the RTCC (timer0) and interrupts to keep a   ////
////  real time seconds counter.  A simple stop watch function is    ////
////  then implemented.                                              ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCD compiler. The following    ////
////  conditional compilation lines are used to include a valid      ////
////  device for each compiler.  Change the device, clock and RS232  ////
////  pins for your hardware if needed.                              ////
/////////////////////////////////////////////////////////////////////////
////    (C) Copyright 1996,2003,2018 Custom Computer Services        ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <30f2010.h>

#use delay(crystal=20mhz)
#use rs232(icd)                 //Text through the ICD
//#use rs232(baud=9600, UART1A) //Text through the UART

#define INTS_PER_SECOND 100

BYTE seconds;      // A running seconds counter
BYTE int_count;    // Number of interrupts left before a second has elapsed

#int_timer1                          
void clock_isr()
{
    if(--int_count==0) {           // per second.
      ++seconds;
      int_count=INTS_PER_SECOND;
    }
}

void main() {

   int_count=INTS_PER_SECOND;
   
   setup_timer1(TMR_INTERNAL,50000);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);

   do {

      printf("Press any key to begin.\n\r");
      getc();
      seconds=0;
      set_timer1(0);
      printf("Press any key to stop.\n\r");
      getc();
      printf("%u seconds.\n\r",seconds);

   } while (TRUE);

}
