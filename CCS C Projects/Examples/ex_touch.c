/////////////////////////////////////////////////////////////////////////
////                             EX_TOUCH.C                          ////
////                                                                 ////
////  This program interfaces a Dallas DS1991 touch memory device.   ////
////  It will display the family code and ID of any device that      ////
////  "touch"es pin B0.                                              ////
////                                                                 ////
////  Configure the CCS prototype card as follows:                   ////
////     Connect pin B0 through a 4.7K resistor to +5V               ////
////     Connect the touch device to GND and then "Touch" to pin B0  ////
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
#include <18F4520.h>
#endif

#use delay(crystal=20mhz)
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART


#include <touch.c>


void main() {
   BYTE buffer[8];
   BYTE i;

   printf("\r\nWaiting for a touch device...\r\n");
   while (TRUE) {
     while(!touch_present()) ;
     delay_ms(200);
     if(touch_present()) {
        touch_write_byte(0x33);
        for(i=0;i<8;++i)
           buffer[i]=touch_read_byte();
        printf("Family: %2X  ID: ",buffer[0]);
        for(i=6;i!=0;--i)
          printf("%2X",buffer[i]);
        printf("\r\n");
        delay_ms(1000);
     }
  }
}
