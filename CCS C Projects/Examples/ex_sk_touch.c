///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          ex_sk_touch.c                            ////
////                                                                   ////
//// Example code showing how to detect that there has been a touch    ////
//// on the touch sensor.                                              ////
////                                                                   ////
//// Sensor module should be inserted into the top right of the of     ////
//// the E3 I/O header: the G pin of module inserts into the GND pin   ////
//// of the E3 and the D0 pin of the module inserts into the B5 pin of ////
//// the E3. Allow A0 to not be connected for this example             ////
////                                                                   ////
//// This module detects when an object touches the sensor.  If touch  ////
//// has been detected, red LED will be lit.  If no touch has been     ////
//// detected, green LED will be lit.                                  ////
////                                                                   ////
//// This sensor module is sometimes called ky036.                     ////
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

#define PIN_TOUCH_INPUT PIN_B5

void main(void)
{
   printf("\r\n\r\nex_sk_touch.c - example starting\r\n\r\n");

   while(TRUE)
   {
      if (input(PIN_TOUCH_INPUT))
      {
         only_led_on(RED_LED);
      }
      else
      {
         only_led_on(GREEN_LED);
      }
   }
}
