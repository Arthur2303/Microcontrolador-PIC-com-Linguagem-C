/////////////////////////////////////////////////////////////////////////
////                          EX_INTEE.C                             ////
////                                                                 ////
////  This program will read and write to the internal               ////
////  EEPROM.                                                        ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCM, PCD, and PCH compilers.   ////
////  The following conditional compilation lines are used to include////
////  a valid device for each compiler.  Change the device, clock and////
////  RS232 pins for your hardware if needed.                        ////
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
#elif defined(__PCD__)
#include <30F2010.h>
#elif defined(__PCH__)
#include <18F4520.h>
#endif

#use delay(crystal=20mhz)
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART

#include "stdlib.h"
#include <input.c>
#include "limits.h"
// The following initializes the first 4 locations of the data EERPOM
//  using the #ROM directive
#rom getenv("EEPROM_ADDRESS")={1,2,3,4}

#if defined(__PCD__)
 // dsPIC30F/dsPIC33F/PIC24 internal eeprom is 16bit wide
 typedef int16 INTEE;
#else
 typedef int8 INTEE;
#endif

void main() {

   unsigned int8 i, j, address;
   INTEE value;

   do {
      printf("\r\n\nEEPROM:\r\n");              // Display contents of the first 64
      for(i=0; i<=3; ++i) {                     // bytes of the data EEPROM in hex
         for(j=0; j<=15; ++j) {
            printf( "%2x ", read_eeprom( i*16+j ) );
         }
         printf("\n\r");
      }
      printf("\r\nLocation to change: ");
      address = gethex();
      printf("\r\nNew value: ");
      value = gethex();

      write_eeprom( address, value );

   } while (TRUE);
}
