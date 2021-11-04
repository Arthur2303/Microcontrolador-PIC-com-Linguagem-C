/////////////////////////////////////////////////////////////////////////
////                           ex_ext_pcd.c                          ////
////                                                                 ////
////  A simple example showing how to enable and use the EXTx        ////
////  interrupts for PCD devices.  This example will setup EXT0 to   ////
////  interrupt on the falling edge (H_TO_L) and increase a count    ////
////  variable each time the interrupt is serviced.  Additionally    ////
////  it will print using a RS232 connection that the interrupt      ////
////  occurred and the current count value.                          ////
////                                                                 ////
////  This example will work with the PCD compiler, change the       ////
////  device included, the #use delay() RS232 pins and the EXT       ////
////  peripheral to use as need for your hardware.  For some device  ////
////  some or all the EXTx pins are remappable, for those devices    ////
////  use #pin_select INTx=PIN_yy, x being the EXT peripheral using  ////
////  and yy being the pin to assign to the peripheral.  For example ////
////  the following will assign PIN_B0 as the input pin for the EXT1 ////
////  peripheral:                                                    ////
////     #pin_select INT1=PIN_B0                                     ////
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

#include <24FJ256GA702.h>
#fuses NOWDT
#use delay(internal=32MHz)

#define RS232_TX_PIN    PIN_B8
#define RS232_RX_PIN    PIN_B9

#pin_select U1TX=RS232_TX_PIN
#pin_select U1RX=RS232_RX_PIN

#use rs232(xmit=RS232_TX_PIN, rcv=RS232_RX_PIN, baud=115200)

int1 g_Ext0Occured = FALSE;
unsigned int16 g_Ext0Count = 0;

//create EXT0 interrupt service routine (ISR)
#INT_EXT0
void ext_isr(void)
{
   g_Ext0Count++;
   g_Ext0Occured = TRUE;
}

unsigned int16 GetExt0Count(void)
{
   unsigned int16 Result;
   
   disable_interrupts(INT_EXT0);
   Result = g_Ext0Count;
   enable_interrupts(INT_EXT0);
   
   return(Result);
}

void main(void)
{
   unsigned int16 Count;
   
   delay_ms(100);
   
   printf("ex_ext_pcd.c - %s\r\n", getenv("DEVICE"));
   
   clear_interrupt(INT_EXT0);
   
   ext_int_edge(0, H_TO_L);      //select edge EXT0 interrupt occurs on (H_TO_L or L_TO_H)
   enable_interrupts(INT_EXT0);  //enable EXT0 interrupt
   
   enable_interrupts(INTR_GLOBAL);  //enable global interrupt

   while(TRUE)
   {
      if(g_Ext0Occured)
      {
         Count = GetExt0Count();
         
         printf("EXT0 Interrupt Occured, Count: %lu\r\n", Count);
         
         g_Ext0Occured = FALSE;
      }
   }
}
