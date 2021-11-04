/////////////////////////////////////////////////////////////////////////
////                   EX_RTOS_DEMO_6_SEMAPHORES.C                   ////
////                                                                 ////
////  This file demonstrates how to use the real time operating      ////
////  rtos_signal and rtos_wait function to handle resources         ////
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

// a semaphore is simply a shared system resource
// in the case of this example, the semaphore will be the red LED
int8 sem;
#define RED PIN_B5

#task(rate=1000ms,max=100ms,queue=2)
void the_first_rtos_task ( );

#task(rate=1000ms,max=100ms,queue=2)
void the_second_rtos_task ( );

void the_first_rtos_task ( ) {
   int i;
   // this will decrement the semaphore variable to zero which signals
   // that no more user may use the resource
   rtos_wait(sem);
   for(i=0;i<5;i++){
      output_low(RED); delay_ms(20); output_high(RED);
      rtos_yield ( );
   }
   // this will inrement the semaphore variable to zero which then signals
   // that the resource is available for use
   rtos_signal(sem);
}

void the_second_rtos_task ( ) {
   int i;
   rtos_wait(sem);
   for(i=0;i<5;i++){
      output_high(RED); delay_ms(20); output_low(RED);
      rtos_yield ( );
   }
   rtos_signal(sem);
}

void main ( ) {
   // sem is initialized to the number of users allowed by the resource
   // in the case of the LED and most other resources that limit is one
   sem=1;
   rtos_run();
}
