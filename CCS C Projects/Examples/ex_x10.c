/////////////////////////////////////////////////////////////////////////
////                             EX_X10.C                            ////
////                                                                 ////
////  This program interfaces a X10 TW523 unit to RS-232.  This      ////
////  program will accept and send three character codes of the      ////
////  form xyy where x is A-P and yy is 00-1F.                       ////
////  Key codes 00-0F are translated to the key number.              ////
////                                                                 ////
////  A * is sent to indicate transmition was aborted due to         ////
////  a collision.  A > is sent when reception begins to reduce      ////
////  the chance of attempting to transmit during reception.         ////
////                                                                 ////
////  Configure the CCS prototype card as follows:                   ////
////     Connect pin B0 to TW523 pin 1                               ////
////             pin B1              3                               ////
////             pin B2              4                               ////
////             GND                 2                               ////
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
#endif

#use delay(crystal=20mhz)
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART


#include <x10.c>
#include <input.c>


void main() {
   char house_code;
   BYTE key_code;

   printf("Online\n\r");

   while (TRUE) {

      if(kbhit()) {
        house_code = getc();
        if((house_code>='A') && (house_code<='P')) {
          putc(house_code);
          key_code=gethex();
          x10_write(house_code,key_code);
          x10_write(house_code,key_code);
        }
      }

      if(x10_data_ready()) {
        putc('>');
        x10_read(&house_code, &key_code);
        printf("%c%2X", house_code, key_code);
      }
   }
}
