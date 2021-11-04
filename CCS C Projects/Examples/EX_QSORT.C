/////////////////////////////////////////////////////////////////////////
////                           EX_QSORT.C                            ////
////                                                                 ////
////  This program shows how to use the stdlib function qsort.       ////
////  This is also a classic example of how to implement pointers    ////
////  to functions.                                                  ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCM and PCH compilers.  The    ////
////  following conditional compilation lines are used to include a  ////
////  valid device for each compiler.  Change the device, clock and  ////
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
#elif defined(__PCH__)
#include <18F4580.h>
#endif
#use delay(crystal=20mhz)
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART

#include <stdlib.h>

#define MAXCHARS 6
#define ITEMS 5

char data[ITEMS][MAXCHARS] = {"ONE","TWO","THREE","FOUR","FIVE"};


signed int assending(char * p1,char * p2) {
   int i;

   i=0;
   while((i<maxchars)&&(*p1==*p2)) {
     ++i;
     ++p1;
     ++p2;
   }
   if(i==maxchars)
     return 0;
   else if(*p1<*p2)
     return -1;
   else
     return 1;
}

signed int desending(char * p1,char * p2) {
   int i;

   i=0;
   while((i<maxchars)&&(*p1==*p2)) {
     ++i;
     ++p1;
     ++p2;
   }
   if(i==maxchars)
     return 0;
   else if(*p1>*p2)
     return -1;
   else
     return 1;
}



void main() {
   int i;
   char c;
   _Cmpfun compare;

   while( TRUE ) {
      do {
        printf("\r\n\r\nSort Assending or Desending (A,D): ");
        c=toupper(getc());
      } while ((c!='A')&&(c!='D'));

      if(c == 'A')
        compare=assending;
      else
        compare=desending;

      qsort(data, items, maxchars, compare);
      for(i=0; i<items; ++i)
        printf("\r\n%s",data[i]);
   }
}

