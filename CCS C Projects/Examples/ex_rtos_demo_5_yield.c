/////////////////////////////////////////////////////////////////////////
////                     EX_RTOS_DEMO_5_YIELD.C                      ////
////                                                                 ////
////  This file demonstrates how to use the real time operating      ////
////  rtos_yield function.                                           ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2003,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <18F452.h>
#use delay(crystal=20mhz)
#use rs232(icd)                                //Text through the ICD
//#use rs232(baud=9600,xmit=PIN_C6,rcv=PIN_C7) //Text through the UART
#use rtos(timer=0,minor_cycle=100ms)

#task(rate=1000ms,max=100ms,queue=2)
void the_first_rtos_task ( );

#task(rate=500ms,max=100ms,queue=2)
void the_second_rtos_task ( );

void the_first_rtos_task ( ) {
   int count=0;
   // rtos_yield allows the user to break out of a task at a given point
   // and return to the same ponit when the task comes back into context
   while(TRUE){
      count++;
      rtos_msg_send(the_second_rtos_task,count);
      rtos_yield ( );
   }
}

void the_second_rtos_task ( ) {
   if(rtos_msg_poll( ))
   {
      printf("count is : %i\n\r",rtos_msg_read ( ));
   }
}

void main ( ) {
   rtos_run();
}
