///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          ex_sk_vibration.c                        ////
////                                                                   ////
//// This sensor board detects vibration.  This example was written    ////
//// for the E3 to demonstrate the sensor board's usage.               ////
//// Sensor module should be inserted into the top right of the of     ////
//// the E3 I/O header: the G pin of module inserts into the GND pin   ////
//// of the E3 and the D0 pin of the module inserts into the B5 pin of ////
//// the E3. Allow A0 to not be connected for this example             ////
////                                                                   ////
//// This module detects when there is vibration.  If vibration        ////
//// has been detected, red LED will be lit.  If no vibration, green   ////
//// LED will be lit.                                                  ////
////                                                                   ////
//// This sensor module is sometimes called ky002.                     ////
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

#define PIN_VIBRATION_INPUT PIN_B5

void main(void)
{
   printf("\r\n\r\nex_sk_vibration.c - example starting\r\n\r\n");

   while(TRUE)
   {
      if (input(PIN_VIBRATION_INPUT))
      {
         only_led_on(GREEN_LED);
      }
      else
      {
         only_led_on(RED_LED);
         delay_ms(250);
      }
   }
}
