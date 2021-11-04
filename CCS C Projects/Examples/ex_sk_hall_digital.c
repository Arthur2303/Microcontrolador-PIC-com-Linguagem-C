///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                     ex_sk_hall_digital.c                          ////
////                                                                   ////
//// Example code showing how to read the result of the                ////
//// magnetic field recorded by the magnetic hall effect sensor.       ////
//// Example code developed to run on the CCS E3 Mini development kit. ////
//// Sensor module should be inserted into the top right of the of     ////
//// the E3 I/O header: the G pin of module inserts into the GND pin   ////
//// of the E3 and the Y pin of the module inserts into the B5 pin of  ////
//// the E3.                                                           //// 
////                                                                   ////
//// This module detects magnetic fields and returns a digital result  ////
//// (LED) based on the output of the hall effect sensor.  The hall    ////
//// effect sensor only returns a YES (pin high) or NO (pin low) if    ////
//// it's detecting a megnetic field.                                  ////
////                                                                   ////
//// When the example code is running, a green LED will be lit. Use a  ////
//// magnet up close to the module. When the module senses the         ////
//// magnetic field, a red LED will light up and the GREEN LED will    ////
//// turn off.                                                         ////
////                                                                   ////
//// This sensor module is sometimes called ky003.                     ////
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

#define PIN_HALL_EFFECT_INPUT PIN_B5

void main(void)
{
   printf("\r\n\r\nex_sk_hall_digital.c - example starting\r\n\r\n");

   while(TRUE)
   {
      if (input(PIN_HALL_EFFECT_INPUT))
      {
         only_led_on(GREEN_LED);
      }
      else
      {
         only_led_on(RED_LED);
      }
   }
}

