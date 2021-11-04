///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          ex_sk_relay.c                            //// 
////                                                                   ////
//// Example code showing how to turn on and off the relay module.     ////
//// Example code developed to run on the CCS E3 Mini development kit. ////
//// The - pin on the relay connects into the GND pin of the E3 and    ////
//// the s pin of the module connects to the B5.                       ////
////                                                                   ////
//// Pushing the C1 button will cause the relay to activate, releasing ////
//// the button causes the relay to deactivate.  When the relay is     ////
//// is not active, the NC screw terminal and the middle terminal      ////
//// will be connected and the NO screw terminal and the middle        ////
//// terminal will not be connected.  When the relay is active,        ////
//// the NC screw terminal and middle terminal will not be connected   ////
//// and the NO screw terminal and middle terminal will be connected.  ////
////                                                                   ////
//// This sensor module is sometimes called ky019.                     ////
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

#define PIN_RELAY_ACTIVATE PIN_B5

#define RELAY_ACTIVATE()  output_high(PIN_RELAY_ACTIVATE)
#define RELAY_DEACTIVATE() output_low(PIN_RELAY_ACTIVATE)

#define BUTTON_PRESSED()   (!input(BUTTON_1))

void main(void)
{
   printf("\r\n\r\nex_sk_relay.c - example starting\r\n\r\n");

   output_float(RED_LED);
   output_float(YELLOW_LED);
   output_float(GREEN_LED);

   for(;;)
   {
      if (BUTTON_PRESSED())
      {
         RELAY_ACTIVATE();
      }
      else
      {
         RELAY_DEACTIVATE();
      }
      
      delay_ms(72);
   }
}

