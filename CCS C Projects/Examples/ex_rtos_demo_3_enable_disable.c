/////////////////////////////////////////////////////////////////////////
////                EX_RTOS_DEMO_3_ENABLE_DISABLE.C                  ////
////                                                                 ////
////  This file demonstrates how to use the real time operating      ////
////  system rtos_enable and rtos_disable functions                  ////
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

int8 counter;

// now that task names will be passed as parameters, it is best
// to declare function prototypes so that there are no undefined
// identifier errors from the compiler
#task(rate=1000ms,max=100ms)
void the_first_rtos_task ( );

#task(rate=500ms,max=100ms)
void the_second_rtos_task ( );

#task(rate=100ms,max=100ms)
void the_third_rtos_task ( );

void the_first_rtos_task ( ) {
   printf("1\n\r");
   if(counter==3)
   {
      // to disable a task, simply pass the task name
      // into the rtos_disable function
      rtos_disable(the_third_rtos_task);
   }
}

void the_second_rtos_task ( ) {
   printf("\t2!\n\r");
   if(++counter==10) {
      counter=0;
      // enabling tasks is similar to disabling them
      rtos_enable(the_third_rtos_task);
   }
}

void the_third_rtos_task ( ) {
   printf("\t\t3\n\r");
}

void main ( ) {
   counter = 0;
   rtos_run ( );
}
