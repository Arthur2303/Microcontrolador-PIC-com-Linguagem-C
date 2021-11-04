/////////////////////////////////////////////////////////////////////////
////                           EX_MXRAM.C                            ////
////                                                                 ////
////  This program it not intended to be executed but does show how  ////
////  to directly use all the RAM on a 877 chip (or any chip with    ////
////  RAM over 255).  For another method that takes less ROM see     ////
////  EX_PSP.C                                                       ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCM and PCH compilers.         ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2003,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCH__)
#include <18F4520.h>
#elif defined(__PCM__)
#include <16F887.h>
#endif

#device *=16                    // This allows auto variables over location 0xFF
#use delay(crystal=20mhz)
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART
#define BUFFSIZE 64


byte buffer1[BUFFSIZE],buffer2[BUFFSIZE],buffer3[BUFFSIZE];
// Three buffers are allocated because C requires arrays elements to be in
// consecutive memory locations and the PIC memory has holes.


// The following allows reads and writes to the three arrays like they are
// one large array.


void write_buffer(char * index, char value) {
   if(index<BUFFSIZE)
    buffer1[index]=value;
 else if(index<(BUFFSIZE*2))
    buffer2[index%BUFFSIZE]=value;
 else
    buffer3[index%BUFFSIZE]=value;
}

char read_buffer(char * index) {
   if(index<BUFFSIZE)
    return(buffer1[index]);
 else if(index<(BUFFSIZE*2))
    return(buffer2[index%BUFFSIZE]);
 else
    return(buffer3[index%BUFFSIZE]);
}



void main() {
   char * i;
   char c;

   do {
       i=0;
       printf("\r\nEnter string: ");
       do{
         c=getc();
          write_buffer(i++,c);
       } while(c!='\r');

       i=0;
       printf("\r\nString from buffer: ");
       do{
         c=read_buffer(i++);
         putc(c);
       } while(c!='\r');
 } while(1);
}
