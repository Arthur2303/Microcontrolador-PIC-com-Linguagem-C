///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          ex_sk_keypad.c                           ////
////                                                                   ////
//// Example code shows how to read from the 3x4 keypad. Sample        ////
//// code developed to run on the CCS E3 Mini development kit.         ////
//// Keypad connects to the E3 as so:                                  ////
////   pin1 of keypad - PIN_C1                                         ////
////   pin2 of keypad - PIN_C2                                         ////
////   pin3 of keypad - PIN_B4                                         ////
////   pin4 of keypad - PIN_B5                                         ////
////   pin5 of keypad - PIN_B6                                         ////
////   pin6 of keypad - PIN_B7                                         ////
////   pin7 of keypad - PIN_C5                                         ////
////                                                                   ////
//// When the example is running, all keypad presses are displayed     ////
//// over USB and can be viewed using SIOW.                            ////
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

#include <tick.c>

#define KBD_USES_PULLUPS

#define KBD_COL1  PIN_B6
#define KBD_COL2  PIN_B7
#define KBD_COL3  PIN_C5

#define KBD_ROW1  PIN_C1
#define KBD_ROW2  PIN_C2
#define KBD_ROW3  PIN_B4
#define KBD_ROW4  PIN_B5

#include <kbd3.c>

void main(void)
{
   char c;
   
   port_b_pullups(0x30);

   printf("\r\n\r\nex_sk_keypad.c - example starting\r\n");
   
   // yellow LED and greed LED are overlapping KBD_ROW1 and KBD_ROW2, 
   // so we can't use them as LEDs; do not attempt to turn them on.
   only_led_on(RED_LED);
   
   kbd_init();
   
   while(1)
   {
      c = kbd_getc();
      
      if (c)
      {
         printf("got = '%c'\r\n", c);
      }
   }
}

