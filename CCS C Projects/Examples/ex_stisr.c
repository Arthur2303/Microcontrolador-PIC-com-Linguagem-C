/////////////////////////////////////////////////////////////////////////
////                          EX_STISR.C                             ////
////                                                                 ////
////  This program shows how to implement an interrupt service       ////
////  routine to buffer up outgoing serial data.                     ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCM, PCH and PCD compilers.The ////
////  following conditional compilation lines are used to include a  ////
////  valid device for each compiler.  Change the device, clock and  ////
////  RS232 pins for your hardware if needed.                        ////
/////////////////////////////////////////////////////////////////////////
////         (C) Copyright 2018 Custom Computer Services             ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F887.h>
#elif defined(__PCH__)
#include <18F4520.h>
#elif defined(__PCD__)
#include <30F2010.h>
#endif

#use delay(crystal=20mhz)
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART


#define T_BUFFER_SIZE 64
byte t_buffer[T_BUFFER_SIZE];
byte t_next_in = 0;
byte t_next_out = 0;


#int_tbe
void serial_isr() {

   if(t_next_in!=t_next_out)
   {
      putc(t_buffer[t_next_out]);
      t_next_out=(t_next_out+1) % T_BUFFER_SIZE;
   }
}

void bputc(char c) {
   short restart;
   int ni;

   restart=t_next_in==t_next_out;
   t_buffer[t_next_in]=c;
   ni=(t_next_in+1) % T_BUFFER_SIZE;
   while(ni==t_next_out);
   t_next_in=ni;
   if(restart)
      enable_interrupts(INT_TBE); 
}

void main() {

   enable_interrupts(GLOBAL);

   printf(bputc,"\r\n\Running...\r\n");

   do {
      delay_ms(2000);
      printf(bputc,"This is buffered data\r\n");
   } while (TRUE);
}

