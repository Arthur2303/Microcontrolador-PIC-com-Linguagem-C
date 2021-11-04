/////////////////////////////////////////////////////////////////////////
////                    EX_RTOS_DEMO_7_AWAIT.C                       ////
////                                                                 ////
////  This file demonstrates how to use the real time operating      ////
////  systems rtos_await function                                    ////
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

#define RED PIN_B5
#define GREEN PIN_A5

int8 count;

#task(rate=1000ms,max=100ms,queue=2)
void the_first_rtos_task ( );

#task(rate=1000ms,max=100ms,queue=2)
void the_second_rtos_task ( );

void the_first_rtos_task ( ) {
   // rtos_await simply waits for the given expression to be true
   // if it is not true, it acts like an rtos_yield and passes the system
   // to the next task
   rtos_await(count==10);
   output_low(GREEN); delay_ms(20); output_high(GREEN);
   count=0;
}

void the_second_rtos_task ( ) {
   output_low(RED); delay_ms(20); output_high(RED);
   count++;
}

void main ( ) {
   count=0;
   rtos_run();
}
