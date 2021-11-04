/////////////////////////////////////////////////////////////////////////
////                      EX_GRAPH_SIOW.C                            ////
////                                                                 ////
////  This program displays uses a featutre in the CCS IDE Serial    ////
////  input/output monitor (SIOW) to display a X/Y type graph.       ////
////                                                                 ////
////  The pot on most CCS prototyping boards is used as in input     ////
////  for the Y data on the graph.  Run the program and turn the     ////
////  pot to see the graph.                                          ////
////                                                                 ////
////  This program also shows using the ICD unit for data streaming  ////
////  to the PC.                                                     ////
////                                                                 ////
////  This example will work with the PCB, PCM, PCD, and PCH         ////
////  compilers. The following conditional compilation lines are     ////
////  used to include a valid device for each compiler.  Change the  ////
////  device, clock and RS232 pins for your hardware if needed.      ////
/////////////////////////////////////////////////////////////////////////
////      (C) Copyright 2013,2018 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCH__)
#include <18f6722.h>           // Change for your target chip
#elif defined(__PCB__)
#include <16F57.h>
#elif defined(__PCM__)
#include <16F887.h>
#elif defined(__PCD__)
#include <30F2010.h>
#endif

#use delay( crystal=20mhz )   // Change for your clock source
#use rs232(icd)              //Text through the ICD


#define GRAPH_TITLE "Pot Tracking"
#define GRAPH_X_MAX  "600"
#include <graph_siow.h>


void main(void) {

   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(AN0);             // May need to change for your chip/board
                                     // See the device .h file for help

   init_graph();
   while(TRUE) {
       graph_clear();
       for(long i=0;i<599;i++) {
          delay_ms(100);
          graph_point(read_adc());
       }
   }

}

