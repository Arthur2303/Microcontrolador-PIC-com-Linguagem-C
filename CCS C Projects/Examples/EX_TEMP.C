/////////////////////////////////////////////////////////////////////////
////                         EX_TEMP.C                               ////
////                                                                 ////
////  Reads temperature using the DS1621 and sends it over the RS232 ////
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

#if defined(__PCB__)
#include <16F57.h>
#elif defined(__PCM__)
#include <16F887.h>
#elif defined(__PCH__)
#include <18F4580.h>
#endif

#use delay(crystal=20mhz)
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART

#define DAL_SCL   PIN_B0
#define DAL_SDA   PIN_B1

#include <ds1621.c>


void main() {
   BYTE value;

   init_temp();

   do {
      value = read_temp();
      printf("%u\r\n",value);
      delay_ms(1000);
   } while (TRUE);
}
