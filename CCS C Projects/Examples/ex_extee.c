/////////////////////////////////////////////////////////////////////////
////                          EX_EXTEE.C                             ////
////                                                                 ////
////  This program uses the 24xx or 93xx external EEPROM drivers to  ////
////  read and write to an external serial EEPROM.                   ////
////                                                                 ////
////  Change the #include <2416.C> to any of the other drivers to    ////
////  test other parts.  Note each driver defines EEPROM_ADDRESS     ////
////  indicate 8 or 16 bit addresses.                                ////
////                                                                 ////
////  Configure the CCS prototype card as follows:                   ////
////     EEPROM connections will vary depending on the device,       ////
////     for the 93C56 use the 9356.C driver and the default         ////
////     connections:                                                ////
////               9356 pin    Protoboard                            ////
////                  1            pin B7                            ////
////                  2            pin B6                            ////
////                  3            pin B5                            ////
////                  4            pin B4                            ////
////                  5            gnd                               ////
////                  6            gnd                               ////
////                  7            gnd                               ////
////                  8            +5V                               ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCB, PCM, PCH, and PCD         ////
////  compilers.                                                     ////
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
#elif defined(__PCD__)
#include <30F2010.h>
#endif

#use delay( crystal=20mhz )
#device cin_buffer = 10
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART

#include "ios.h"
#include "2416.c"


void main() {

   BYTE value;
   EEPROM_ADDRESS address;
   char cmd;
   init_ext_eeprom();

   do {
      do {
         cout << endl << "Read or Write: ";
         cin >> cmd;
         cmd=toupper(cmd);
      } while ( (cmd!='R') && (cmd!='W') );

      cout << endl << "Location (in hex): ";

      cin >> hex >> address;

      if(cmd=='R')
         cout << endl << "Value: " << READ_EXT_EEPROM( address ) << endl;

      if(cmd=='W') {
         cout << endl << "New value (in hex): ";
         cin >> hex >> value;
         cout << endl;
         WRITE_EXT_EEPROM( address, value );
      }
   } while (TRUE);
}
