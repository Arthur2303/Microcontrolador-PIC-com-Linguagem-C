///////////////////////////////////////////////////////////////////////////////
////                             EX_SMT_TIMER.C                            ////
////                                                                       ////
//// This program shows how to setup and use the Signal Measurement Timer  ////
//// (SMT) peripheral as a 24 bit timer, and using it's interrupt to       ////
//// toggle a LED.                                                         ////
////                                                                       ////
//// This example will work for the PCM and PCH compilers.  The following  ////
//// conditional compilation lines are used to include a valid device for  ////
//// each compiler, change the device, clock, and defines for your         ////
///  hardware as needed.                                                   ////
///////////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016,2018 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS C      ////
//// compiler.  This source code may only be distributed to other licensed ////
//// users of the CCS C compiler.  No other use, reproduction or           ////
//// reproduction or distribution is permitted without written permission. ////
//// Derivative programs created using this software in object code form   ////
//// are not restricted in any way.                                        ////
///////////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F1618.h>
#elif defined(__PCH__)
#include <18F4520.h>
#endif

#use delay(internal=8MHz)

#define LED_PIN      PIN_B4
#define SMT_PERIOD   (getenv("CLOCK") - 1)  //1 second from an 8MHz clock

#INT_SMT1
void smt1_isr(void)
{
   output_toggle(LED_PIN);
}

void main(void)
{
   output_low(LED_PIN);

   setup_smt1(SMT_ENABLED | SMT_MODE_TIMER | SMT_CLK_FOSC, SMT_PERIOD);
   smt1_start();  //start SMT1
   
   enable_interrupts(INT_SMT1);  //enabled SMT1 interrupt
   enable_interrupts(GLOBAL);

   while(TRUE)
   {
   }
}
