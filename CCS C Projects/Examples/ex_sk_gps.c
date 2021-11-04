///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          ex_sk_gps.c                              ////
////                                                                   ////
//// Example showing how to get the GPS NMEA data from the GPS module. ////
//// The GPS on this board is a u-Blox Neo-6M.  This example developed ////
//// to run on the E3mini development kit.  Connect GND of module to   ////
//// GND of E3, connect VCC of module to +5 of E3, connect TXD of      ////
//// module to B5 of E3mini.                                           ////
////                                                                   ////
//// This example simply shows all incoming characters received.  For  ////
//// a more complex example that isolates specific variables from      ////
//// the GPGGA message, see ex_sk_gps_extracredit.c                    ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#include <e3.h>

// create a new serial port using the UART peripheral of the PIC18F14K50.
// on the PIC18F14K50, PIN_B5 is receive and PIN_B7 is transmit.
#use rs232(UART1, baud=9600, errors, receive_buffer=128, stream=STREAM_GPS)

void main(void)
{
   char c;
   
   fprintf(STREAM_USB, "\r\n\r\nex_sk_gps.c - GPS sensor board example\r\n\r\n");
   
   only_led_on(GREEN_LED);
   
   // interrupts need to be enabled in order for the #use rs232()'s 
   // receive buffering to work.
   enable_interrupts(GLOBAL);
 
   for(;;)
   {
      if(kbhit(STREAM_GPS))
      {
         c = fgetc(STREAM_GPS);
         fputc(c, STREAM_USB);
      }
   }
}

