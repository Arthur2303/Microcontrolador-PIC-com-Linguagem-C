/////////////////////////////////////////////////////////////////////////
////                      EX_ADMM10_PCD.C                            ////
////                                                                 ////
////  This program displays the min and max of 30 A/D samples over   ////
////  the RS-232 interface.  The process is repeated forever.        ////
////  This version uses a 10 bit ADC.                                ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCD compilers.  The following  ////
////  conditional compilation lines are used to include a valid      ////
////  device for each compiler.  Change the device, clock and RS232  ////
////  pins for your hardware if needed.                              ////
/////////////////////////////////////////////////////////////////////////
////          (C) Copyright 2018 Custom Computer Services            ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <30F2010.h>

#use delay( crystal=20mhz )
#use rs232(icd)                 //Text through the ICD
//#use rs232(baud=9600, UART1A) //Text through the UART 


void main() {

   unsigned int8 i;
   unsigned int16 value, min, max;

   printf("Sampling:");

   setup_port_a( ALL_ANALOG );
   setup_adc( ADC_CLOCK_INTERNAL );
   set_adc_channel( 0 );

   while (TRUE) {
      min=0xffff;
      max=0;
      for(i=0; i<=30; ++i) {
         delay_ms(100);
         value = read_adc();
         if(value<min)
            min=value;
         if(value>max)
            max=value;
      }
      printf("\n\rMin: %4LX  Max: %4LX\n\r",min,max);

   }
}
