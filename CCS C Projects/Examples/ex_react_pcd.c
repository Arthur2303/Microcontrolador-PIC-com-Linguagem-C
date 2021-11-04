/////////////////////////////////////////////////////////////////////////
////                           EX_REACT.C                            ////
////                                                                 ////
////  This program will show how to use the built in CCP to find the ////
////  reaction time of an external event.                            ////
////                                                                 ////
////  Configure the CCS prototype card as follows:                   ////
////     Make the following connections:                             ////
////        W117SIP-6 Relay    Protoboard                            ////
////              1              pin C2 (also use a pull-up resistor)////
////              2              +5V                                 ////
////              3              pin B1                              ////
////              4              Gnd                                 ////
////                                                                 ////
////  Configure the CCS PCD 30F2010 development board as follows:    ////
////     Make the following connections:                             ////
////        W117SIP-6 Relay    Protoboard                            ////
////              1              pin D0 (also use a pull-up resistor)////
////              2              +5V                                 ////
////              3              pin B3                              ////
////              4              Gnd                                 ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCM, PCD, and PCH compilers.   ////
////  The following conditional compilation lines are used to include////
////  a valid device for each compiler.  Change the device, clock and////
////  RS232 pins for your hardware if needed.                        ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <30F2010.h>
#use delay(crystal=20mhz)
#use rs232(icd)                 //Text through the ICD
//#use rs232(baud=9600, UART1A) //Text through the UART

#define RELAY_SET PIN_B3

void main() {

   unsigned int16 CCP_1;
   
   setup_timer2(TMR_INTERNAL);
   output_low(RELAY_SET);

   while(TRUE)
   {
      char c;
      printf("\r\nHit any key to turn on relay and begin timing\r\n");
      c = getc();
      setup_capture(1, CAPTURE_RE | CAPTURE_TIMER2);
      CCP_1=0;
      set_timer2(0);
      output_high(RELAY_SET);
      CCP_1=get_capture(1, TRUE);
      delay_ms(50);
      printf("Time for relay to turn on:  %lu us\r\n",CCP_1/5);

      delay_ms(100);

      setup_capture(1, CAPTURE_FE | CAPTURE_TIMER2);
      CCP_1=0;
      set_timer2(0);
      output_low(RELAY_SET);
      CCP_1=get_capture(1, TRUE);
      delay_ms(50);
      printf("Time for relay to turn off:  %lu us\r\n",CCP_1/5);

      delay_ms(100);
   }
}




