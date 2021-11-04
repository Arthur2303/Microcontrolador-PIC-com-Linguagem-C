/////////////////////////////////////////////////////////////////////////
////                            EX_ICD.C                             ////
////                                                                 ////
////  This is a simple program used to demonstrate the use of the    ////
////  ICD as a device.  The example does some simple math and then   ////
////  flashes a LED at a frequency of 1Hz.                           ////
////                                                                 ////
////  This example will work with the PCM, and PCD compilers.        ////
////  The following conditional compilation lines are used to        ////
////  include a valid device for each compiler.  Change the device   ////
////   and clock for your hardware if needed.                        ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2008,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F887.h>
#elif defined(__PCD__)
#include <30F2010.h>
#endif

#device ICD=TRUE
#use delay(crystal=20mhz)

#define  TOGGLE_PIN  PIN_B5


// This function turns the LEDs on and off
void flash_led() {

   output_low(TOGGLE_PIN);
   delay_ms(500);
   output_high(TOGGLE_PIN);
   delay_ms(500);
}


// This function does simple math and also turns the LEDs on and
// of at a frequency of 1Hz
void main()   {
   unsigned int8 a,b,c;
   unsigned int16 al,bl,cl;

   a=10;             // initialize variables
   b=25;

   al=300;
   bl=525;

   c=a+b;            // simple math operations

   c=a*b;

   cl=al+bl;

   cl=al*bl;

   while(TRUE)       // flash LEDs at 1Hz
      flash_led();
}
