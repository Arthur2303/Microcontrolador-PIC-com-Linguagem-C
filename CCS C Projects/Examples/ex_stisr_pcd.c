/////////////////////////////////////////////////////////////////////////
////                        EX_STISR_PCD.C                           ////
////                                                                 ////
////  This program shows how to implement an interrupt service       ////
////  routine to buffer up outgoing serial data.                     ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCD compilers. The following   ////
////  conditional compilation lines are used to include a valid      ////
////  device for each compiler.  Change the device, clock and RS232  ////
////  pins for your hardware if needed.                              ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 2018 Custom Computer Services              ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <24HJ128GP306.h>

#use delay(crystal=20MHz)
#use rs232(icd)               //Text through the ICD
//#use rs232(baud=9600,UART1) //Text through the UART



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
   disable_interrupts(INT_TBE);
}

void bputc(char c) {
   #bit U1TXIF = getenv("BIT:U1TXIF")
   short restart;
   int ni;

   restart=t_next_in==t_next_out;
   t_buffer[t_next_in]=c;
   ni=(t_next_in+1) % T_BUFFER_SIZE;
   while(ni==t_next_out);
   t_next_in=ni;
   if(restart)
      U1TXIF = 1;
}

void main() {
      enable_interrupts(INT_TBE);
      enable_interrupts(GLOBAL);

   printf(bputc,"\r\n\Running...\r\n");

   do {
      delay_ms(2000);
      printf(bputc,"This is buffered data\r\n");
   } while (TRUE);
}

