/////////////////////////////////////////////////////////////////////////
////                          ex_cni_ch_ck.c                         ////
////                                                                 ////
////  This program is an example of how to setup and use the Change  ////
////  Notification Interrupt (CNI) on dsPIC33CH and dsPIC33CK        ////
////  devices.                                                       ////
////                                                                 ////
////  The code will enable the CNI on four pins, three ports, on the ////
////  dsPIC33CK256MP508.  This code will service the CNI interrupt   ////
////  service routines when a signal changes on one of the enabled   ////
////  CNI pins.  The interrupt service routines set a software flag, ////
////  reads the CNI pin's current level, and clears the CNI pin's    ////
////  interrupt flag.  When one of the software flags are set the    ////
////  main loop will print out which pin caused the flag to be set,  ////
////  the level of the pin when the flag was set, and clear the      ////
////  software flag.                                                 ////
////                                                                 ////
////  This example will work only for the PCD Compiler on a          ////
////  dsPIC33CH or dsPIC33CK device.  Change the device, clock, CNI  ////
////  pins, CNIx interrupts and RS232 pins as needed for your        ////
////  hardware.                                                      ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2020 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
////                                                                 ////
////                     http://www.ccsinfo.com                      ////
/////////////////////////////////////////////////////////////////////////

#include <33CK256MP508.h>
#fuses NOWDT
#use delay(clock=100MHz, crystal=8MHz)

#pin_select U1TX=PIN_C11
#pin_select U1RX=PIN_C10
#use rs232(UART1, baud=115200)

// Software flags used to indicate a change occurred.
int1 Changed[4] = {false, false, false, false};
int1 Level[4];

// Interrupt Service routine for the CNI pins for PORTD.
#INT_CNID
void cnid_isr(void)
{
   //Check if PIN_D9 caused CNID interrupt to occur.
   if(interrupt_active(INTR_CN_PIN | PIN_D9))
   {
      //Read current level of pin.
      Level[0] = input(PIN_D9);
      
      //Set software flag indicating PIN_D9 cause CNID interrupt to occur.
      Changed[0] = true;
      
      //Clear CNI flag for PIN_D9.
      clear_interrupt(INTR_CN_PIN | PIN_D9);
   }
   
   //Check if PIN_D15 caused CNID interrupt to occur.
   if(interrupt_active(INTR_CN_PIN | PIN_D15))
   {
      //Read current level of pin.
      Level[2] = input(PIN_D15);
      
      //Set software flag indicating PIN_D15 cause CNID interrupt to occur.
      Changed[2] = true;
      
      //Clear CNI flag for PIN_D15.
      clear_interrupt(INTR_CN_PIN | PIN_D15);
   }
}

// Interrupt Service routine for the CNI pins for PORTE.
#INT_CNIE
void cnie_isr(void)
{
   if(interrupt_active(INTR_CN_PIN | PIN_E7))
   {
      //Read current level of pin.
      Level[1] = input(PIN_E7);
      
      //Set software flag indicating PIN_E7 cause CNID interrupt to occur.
      Changed[1] = true;
      
      //Clear CNI flag for PIN_E7.
      clear_interrupt(INTR_CN_PIN | PIN_E7);
   }
}

// Interrupt Service routine for the CNI pins for PORTB.
#INT_CNIB
void cnib_isr(void)
{
   if(interrupt_active(INTR_CN_PIN | PIN_B14))
   {
      //Read current level of pin.
      Level[3] = input(PIN_B14);
      
      //Set software flag indicating PIN_B14 cause CNID interrupt to occur.
      Changed[3] = true;
      
      //Clear CNI flag for PIN_B14.
      clear_interrupt(INTR_CN_PIN | PIN_B14);
   }
}

void main(void)
{
   delay_ms(100);
   
   printf("\r\nEX_CNI_CH_CK.c - %s\r\n", getenv("DEVICE"));
   
   //Read current levels of all CNI pins.
   Level[0] = input(PIN_D8);
   Level[1] = input(PIN_E7);
   Level[2] = input(PIN_D15);
   Level[3] = input(PIN_B14);
   
   //Enable the CNI pin interrupts for the following pins.
   enable_interrupts(INTR_CN_PIN | PIN_D9);
   enable_interrupts(INTR_CN_PIN | PIN_E7);
   enable_interrupts(INTR_CN_PIN | PIN_D15);
   enable_interrupts(INTR_CN_PIN | PIN_B14);
   
   //Enable the CNI port interrupt for ports B, D and E.
   enable_interrupts(INT_CNID);
   enable_interrupts(INT_CNIE);
   enable_interrupts(INT_CNIB);
   
   //Enable global interrupt.
   enable_interrupts(GLOBAL);
   
   while(TRUE)
   {
      if(Changed[0])
      {
         printf("\r\nCNI Interrupt PIN_D9 - Level: %u", Level[0]);
         
         Changed[0] = false;
      }
      
      if(Changed[1])
      {
         printf("\r\nCNI Interrupt PIN_E7 - Level: %u", Level[1]);
         
         Changed[1] = false;
      }
      
      if(Changed[2])
      {
         printf("\r\nCNI Interrupt PIN_D15 - Level: %u", Level[2]);
         
         Changed[2] = false;
      }
      
      if(Changed[3])
      {
         printf("\r\nCNI Interrupt PIN_B14 - Level: %u", Level[3]);
         
         Changed[3] = false;
      }
   }
}
