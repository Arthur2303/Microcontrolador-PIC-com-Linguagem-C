/////////////////////////////////////////////////////////////////////////
////                             EX_CUST.C                           ////
////                                                                 ////
//// This program shows how to customize the compiler operation.     ////
//// The following are shown:                                        ////
////       - Case sensitivity is turned on (normally its off)        ////
////       - short, int and long are 8,16,32 bits (instead of 1,8,16)////
////       - RAM is zeroed out before main starts                    ////
////       - Each function is identified as inline or separate       ////
////       - In some areas the automatic TRIState manipulation       ////
////         is disabled to save time                                ////
////       - Registers from 20h to 2Fh are excluded from C use       ////
////       - #pragma is used on non-standard C pre-processor lines   ////
////         (not required)                                          ////
////                                                                 ////
////  This example will work with the PCB, PCM and PCH compilers.    ////
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

#pragma case
#pragma type short=8,int=16,long=32
#define version 0x0001

#if defined(__PCB__)
#include <16F57.h>
#elif defined(__PCM__)
#include <16F887.H>
#elif defined(__PCH__)
#include <18F4580.h>
#endif

#use delay( crystal=20mhz )
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART

#pragma id version
#reserve 0x20:0x2f

int CYCLES;    // Global cycles (note upper case)

#inline
void pulse_B0() {
#pragma use fast_io(B)
      output_low(PIN_B0);
      delay_ms(100);
      output_high(PIN_B0);
      delay_ms(100);
#pragma use standard_io(B)
}


#separate
void send_pulses() {
   int cycles;  // Local cycles (note lower case)

   for(cycles=CYCLES; cycles!=0; --cycles) {
      pulse_B0();
   }
}


#pragma zero_ram
void main() {

   output_low(PIN_B0);
   output_low(PIN_B1);

   printf("Firmware version is %4X\r\n",version);
   printf("Compiled on %s with compiler version %s\r\n",__DATE__,compiler);

   printf("Pulsing B0 and B1...");
   for(CYCLES=100;CYCLES<=300;CYCLES+=100) {
      printf("\r\nMajor cycle #%c...",(CYCLES/100)+'0');
      output_low(PIN_B1);
      output_high(PIN_B1);
      send_pulses();
   }
   printf("\r\nAll Done.");

   delay_ms(3);  // Allow UART to finish
}
