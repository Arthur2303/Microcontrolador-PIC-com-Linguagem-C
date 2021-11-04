///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                        ex_sk_noise_detect.c                       ////
////                                                                   ////
//// Example code showing how to detect that there has been loud       ////
//// noise detected by the loud noise sensor.  Example code            ////
//// developed to run on the CCS E3 Mini development kit.              ////
//// Sensor module should be inserted into the top right of the of     ////
//// the E3 I/O header: the G pin of module inserts into the GND pin   ////
//// of the E3 and the D0 pin of the module inserts into the B5 pin of ////
//// the E3.                                                           ////
////                                                                   ////
//// This module detects when there is loud noise.  If noise           ////
//// has been detected, red LED will be lit.  If no noise, green       ////
//// LED will be lit.                                                  ////
////                                                                   ////
//// This sensor module is sometimes called ky037.                     ////
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

#define PIN_LOUD_NOISE_INPUT PIN_B5

void main(void)
{
   printf("\r\n\r\nex_sk_noise_detect.c - example starting\r\n\r\n");  

   while(TRUE)
   {
      if (input(PIN_LOUD_NOISE_INPUT))
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
