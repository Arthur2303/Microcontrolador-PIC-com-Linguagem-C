/////////////////////////////////////////////////////////////////////////
////                       EX_WDT_PCM.C                              ////
////                                                                 ////
////  This program demonstrates the watchdog timer.  If the user     ////
////  does not hit a key in the set amount of time, the processor    ////
////  restarts, and tells the user why it restarted.                 ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCB and PCM compilers.         ////
////  The following conditional compilation lines are used to        ////
////  include a valid device for each compiler.  Change the device,  ////
////  clock and RS232 pins for your hardware if needed.              ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2003,2018 Custom Computer Services       ////
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
#endif

#use delay(crystal=20mhz)
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9200, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART

void main()   {

   switch ( restart_cause() )
   {
      case WDT_TIMEOUT:
      {
         printf("\r\nRestarted processor because of watchdog timeout!\r\n");
         break;
      }
      case NORMAL_POWER_UP:
      {
         printf("\r\nNormal power up!\r\n");
         break;
      }
   }

   setup_wdt(WDT_2304MS);

   while(TRUE)
   {
      restart_wdt();
      printf("Hit any key to avoid a watchdog timeout.\r\n");
      getc();
   }
}
