/////////////////////////////////////////////////////////////////////////
////                             EX_CNI.C                            ////
////                                                                 ////
////  This program is an example of how to setup and use the Change  ////
////  Notification Interrupt (CNI) on PIC24, dsPIC30 and dsPIC33     ////
////  devices.                                                       ////
////                                                                 ////
////  The code will enable the CNI on up to three pins on the        ////
////  PIC24EP256GP206 chip, see CNI_PIN1, CNI_PIN2 and CNI_PIN3      ////
////  defines for setting which pins the CNI will occur on.          ////
////                                                                 ////
////  The code will toggle and LED and service the CNI interrupt     ////
////  service routine when a signal changes on one of the enabled    ////
////  CNI pins.  The interrupt service routine has a printf          ////
////  statement that will indicate that the code has entered the     ////
////  interrupt and which pin caused the interrupt to occur.         ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work only for the PCD Compiler.  Change the  ////
////  device, clock, LED, CNI and RS232 pins for your hardware as    ////
////  needed.                                                        ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2017,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
////                     http://www.ccsinfo.com                      ////
/////////////////////////////////////////////////////////////////////////

#if !defined(__PCD__)
#error This example will only compile for the PCD 24 bit compiler
#endif
#include <24EP256GP206.h>
#use delay(crystal=20MHz)

#define PIN_LED   PIN_A1
#define CNI_PIN1  PIN_C3
#define CNI_PIN2  PIN_E14
#define CNI_PIN3  PIN_F0

#pin_select U1TX=PIN_A4
#pin_select U1RX=PIN_A9
#use rs232(icd)                //Text through the ICD
//#use rs232(UART1, baud=9600) //Text through the UART

int1 cState[3], pState[3];

//CNI interrupt service routine, all enabled CNI pins are serviced by this ISR.
//Added NOCLEAR option, because the CNI Flag is cleared after all enabled CNI
//pins are read.  This way if a change occurs on another pin while servicing
//the interrupt for another it won't be missed.  
#INT_CNI NOCLEAR
void CCNI_Interrupt()
{
   //Read the current state of each of the enabled CNI pins and compare to 
   //previous state to determine which pin/pins caused interrupt to occur.
  #ifdef CNI_PIN1
   cState[0] =  input(CNI_PIN1);
   
   if(cState[0] != pState[0])
   {
      printf("\r\nCNI_PIN1 Changed");
      
      pState[0] = cState[0];
   }
  #endif
  
  #ifdef CNI_PIN2
   cState[1] = input(CNI_PIN2);
   
   if(cState[1] != pState[1])
   {
      printf("\r\nCNI_PIN2 Changed");
      
      pState[1] = cState[1];
   }
  #endif
  
  #ifdef CNI_PIN3
   cState[2] = input(CNI_PIN3);
   
   if(cState[2] != pState[2])
   {
      printf("\r\nCNI_PIN3 Changed");
      
      pState[2] = cState[2];
   }
  #endif
}

void main() 
{
   delay_ms(1000);
   
   printf("\n\rEX_CNI.c - %s", getenv("DEVICE"));

   //Read current state of the CNI pins and enable the interrupt for those pins.
  #ifdef CNI_PIN1
   pState[0] = input(CNI_PIN1);   
   enable_interrupts(INTR_CN_PIN | CNI_PIN1);
  #endif
  
  #ifdef CNI_PIN2
   pState[1] = input(CNI_PIN2);
   enable_interrupts(INTR_CN_PIN | CNI_PIN2);
  #endif
  
  #ifdef CNI_PIN3
   pState[2] = input(CNI_PIN3);
   enable_interrupts(INTR_CN_PIN | CNI_PIN3);
  #endif
  
   enable_interrupts(GLOBAL);
   
   while (TRUE) 
   {
     output_toggle(PIN_LED);     
     delay_ms(500);
   }
}
