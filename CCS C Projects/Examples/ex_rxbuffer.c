/////////////////////////////////////////////////////////////////////////
////                          EX_RXBUFFER.C                          ////
////                                                                 ////
////  This program shows how to implement an interrupt service       ////
////  routine to buffer up incoming serial data.                     ////
////                                                                 ////
////  If the PIC does not have an RDA interrupt pin, B0 may be used  ////
////  with the INT_EXT.                                              ////
////                                                                 ////
////  Also see EX_SISR.c for an example where this is done manually. ////
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

#define BUFFER_SIZE 32

#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, receive_buffer=BUFFER_SIZE) //Text through the UART


void main() {

   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);

   printf("\r\n\Running...\r\n");

               // The program will delay for 10 seconds and then display
               // any data that came in during the 10 second delay

   do {
      delay_ms(10000);
      printf("\r\nBuffered data => ");
      while(kbhit())
        putc( getc() );
   } while (TRUE);
}
