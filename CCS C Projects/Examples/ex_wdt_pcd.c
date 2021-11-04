/////////////////////////////////////////////////////////////////////////
////                      EX_WDT_PCD.C                               ////
////                                                                 ////
////  This program demonstrates the watchdog timer.  If the user     ////
////  does not hit a key in the set amount of time, the processor    ////
////  restarts, and tells the user why it restarted.                 ////
////                                                                 ////
////  This example will work with the PCD compiler.  The following   ////
////  conditional compilation lines are used to include a valid      ////
//    device.  Change the device,  clock and RS232 pins for your     ////
//    hardware if needed.                                            ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2017,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
////                      http://www.ccsinfo.com                     ////
/////////////////////////////////////////////////////////////////////////

#include <24EP256GP206.h>
#use delay(crystal=20MHz)

#pin_select U1TX=PIN_A4
#pin_select U1RX=PIN_A9
#use rs232(icd)                //Text through the ICD
//#use rs232(UART1, baud=9600) //Text through the UART

void main()
{
   unsigned int8 restart;
   
   restart = restart_cause();
   
   switch(restart)
   {
      case RESTART_WATCHDOG:
         printf("\r\nRestarted processor because of watchdog timeout!\r\n");
         break;
      case RESTART_POWER_UP:
         printf("\r\nNormal power up!\r\n");
         break;
      case RESTART_MCLR:
         printf("\r\nRestarted processor because of MCLR!\r\n");
         break;
      default:
         printf("\r\nRestart cause: %u\r\n", restart);
         break;
   }

   setup_wdt(WDT_2S);

   while(TRUE)
   {
      restart_wdt();
      printf("Hit any key to avoid a watchdog timeout.\r\n");
      getc();
   }
}
