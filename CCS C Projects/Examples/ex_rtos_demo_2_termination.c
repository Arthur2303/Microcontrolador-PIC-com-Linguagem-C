/////////////////////////////////////////////////////////////////////////
////                   EX_RTOS_DEMO_2_TERMINATION.C                  ////
////                                                                 ////
////  This file demonstrates how to use the real time operating      ////
////  system rtos_terminate funciton.                                ////
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

// a counter will be kept
int8 counter;

#task(rate=1000ms,max=100ms)
void the_first_rtos_task ( )
{
   printf("1\n\r");
   // if the counter has reached the desired value, the rtos will terminate
   if(++counter==5)
      rtos_terminate ( );
}

#task(rate=500ms,max=100ms)
void the_second_rtos_task ( )
{
   printf("\t2!\n\r");
}

#task(rate=100ms,max=100ms)
void the_third_rtos_task ( )
{
   printf("\t\t3\n\r");
}

void main ( )
{
   // main is the best place to initialize resources the the rtos is dependent
   // upon
   counter = 0;
   rtos_run ( );
   // once the rtos_terminate function has been called, rtos_run will return
   // program control back to main
   printf("RTOS has been terminated\n\r");
}
