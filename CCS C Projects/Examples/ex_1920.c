/////////////////////////////////////////////////////////////////////////
////                             EX_1920.C                           ////
////                                                                 ////
////  This program interfaces a Dallas DS1920 touch memory device.   ////
////  It will display the temperature reading of the device it       ////
////  "touches" with pin B0.                                         ////
////                                                                 ////
////  Configure the CCS prototype card as follows:                   ////
////     Connect pin B0 through a 4.7K resistor to 5V                ////
////     Connect the touch device to ground and then "Touch" to B0   ////
////     See additional connections below.                           ////
////                                                                 ////
////  The device requires 2 seconds of contact to charge enough power////
////  for the temperature conversion.                                ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCB, PCM and PCH compilers.    ////
////  The following conditional compilation lines are used to        ////
////  include a valid device for each compiler.  Change the device,  ////
////  clock and RS232 pins for your hardware if needed.              ////
/////////////////////////////////////////////////////////////////////////
////      (C) Copyright 1996,2003,2018 Custom Computer Services      ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCH__)
#include <18F452.h>
#elif defined(__PCM__)
#include <16F887.h>
#elif defined(__PCB__)
#include <16F57.h>
#endif

#use delay( crystal=20mhz )
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_A3, rcv=PIN_A2) //Text through the UART

#include <touch.c>
#include <string.h>
#include <stdlib.h>

void main() {
   byte buffer[2];

   printf("\r\nTemperature requires at least 2 seconds of contact.");
   printf("\r\nWaiting for a touch device...\r\n");

   while (TRUE) {
     if(touch_present()) {
        touch_write_byte(0xCC);
        touch_write_byte (0x44);
        output_high(TOUCH_PIN);
        delay_ms(2000);

        touch_present();
        touch_write_byte(0xCC);
        touch_write_byte (0xBE);
        buffer[0] = touch_read_byte();
        buffer[1] = touch_read_byte();

        printf ("\r\nTemperature: %c%3.1f C", (buffer[1])?'-':' ', (float)buffer[0]/2);
        delay_ms (1000);
     }
  }
}
