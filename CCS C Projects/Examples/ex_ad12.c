/////////////////////////////////////////////////////////////////////////
////                          EX_AD12.C                              ////
////                                                                 ////
////  This program will read two A/D channels and display the        ////
////  results as both a voltage and raw hex number over the RS-232.  ////
////  A reading is taken every second. This can be done using either ////
////  the LTC1298 chip or the MCP3208 chip.                          ////
////                                                                 ////
////  Configure the CCS prototype card as follows:                   ////
////                                                                 ////
////     Using the LTC1298:                                          ////
////     LTC1298    Protoboard                                       ////
////         1             pin B3                                    ////
////         2             POT 1 output                              ////
////         3             POT 2 output                              ////
////         4             gnd                                       ////
////         5             pin B2                                    ////
////         6             pin B1                                    ////
////         7             pin B0                                    ////
////         8             +5V                                       ////
////                                                                 ////
////     Using the MCP3208:                                          ////
////     MCP3208    Protoboard                                       ////
////         1            POT 1 output                               ////
////         2            POT 2 output                               ////
////         3..8         Not Connected                              ////
////         9            gnd                                        ////
////        10            pin B3                                     ////
////        11            pin B2                                     ////
////        12            pin B1                                     ////
////        13            pin B0                                     ////
////        14            gnd                                        ////
////        15            +5V                                        ////
////        16            +5V                                        ////
////                                                                 ////
////  Using the MCP3208 requires changing the #include <LTC1298.C>   ////
////  to #include <MCP3208.C>                                        ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
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

#if defined(__PCM__)
#include <16F887.h>
#elif defined(__PCB__)
#include <16F57.h>
#elif defined(__PCH__)
#include <18F4520.h>
#endif

#use delay( crystal=20mhz )
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART

#include <ltc1298.c>


void display_data( long int data ) {
     char volt_string[6];

     convert_to_volts( data, volt_string );
     printf(volt_string);
     printf(" (%4lX)",data);
}


void main() {
   long int value;

   adc_init();

   printf("Sampling:\r\n");

   while (TRUE) {
      delay_ms(1000);

      value = read_analog(0);
      printf("\n\rCh0: ");
      display_data( value );

      value = read_analog(1);
      printf("   Ch1: ");
      display_data( value );

   } 

}
