///////////////////////////////////////////////////////////////////////////
////                         ex_sent_receiver.c                        ////
////                                                                   ////
//// Example showing how to setup and use the Single-Edge Nibble       ////
//// Transmission (SENT) peripheral.  A SENT Data frame consist of a   ////
//// Sync period, a status  nibble, 1-6 data nibbles, a CRC nibble and ////
//// an optional pause pulse, which can be used to provide a fixed     ////
//// message frame time period.                                        ////
////                                                                   ////
//// This example sets up the SENT1 peripheral to be a receiver, which ////
//// receives 6 data nibbles with a tick time of 90us.  Additionally   ////
//// it enables the HW CRC and pause pulse.                            ////
////                                                                   ////
//// This example works with the PCD compiler.  Change the device, pin ////
//// assignments, #use delay() and #use rs232() settings as need for   ////
//// your hardware.                                                    ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2018 Custom Computer Services           ////
////                                                                   ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler or to users who purchased the rights to use this code.   ////
//// This source code may only be distributed to other licensed users  ////
//// of the CCS C compiler or other users who purchased the rights to  ////
//// this code.  The rights to use this code can only be granted       ////
//// by CCS.  No other use, reproduction or distribution is permitted  ////
//// without written permission.  Derivative programs created using    ////
//// this software in object code form are not restricted in any way.  ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#include <33EV32GM102.h>
#fuses NOWDT
#use delay(internal=64MHz)

#pin_select U1TX=PIN_B7
#pin_select U1RX=PIN_B1

#use rs232(UART1, baud=115200)

#pin_select SENT1R=PIN_B3

SENT_DATA_TYPE RxData;
int1 NewRxData = FALSE;

unsigned int32 GetSentCount(SENT_DATA_TYPE Data)
{
   union
   {
      unsigned int32 w;
      unsigned int8 b[4];
   } result;
   
   result.b[3] = 0;
   result.b[2] = (((unsigned int8)Data.DATA6 << 4) & 0xF0) | ((unsigned int8)Data.DATA5 & 0x0F);
   result.b[1] = (((unsigned int8)Data.DATA4 << 4) & 0xF0) | ((unsigned int8)Data.DATA3 & 0x0F);
   result.b[0] = (((unsigned int8)Data.DATA2 << 4) & 0xF0) | ((unsigned int8)Data.DATA1 & 0x0F);
   
   return(result.w);
}

//SENT1 ISR used to receive data, when setup as a receiver the message needs to
//be read before the next frame's status nibble is received.  Using the SENT1
//ISR ensures that it's read before then.
#INT_SENT1
void sent1_isr(void)
{
   RxData = sent_getd(1);
   
   NewRxData = TRUE;   
}

void main(void)
{
   unsigned int32 rCount;
   
   delay_ms(1000);
   
   printf("\r\nex_sent_receiver.c - %s\r\n", getenv("DEVICE"));
   
   //Clear and enable SENT1 interrupt, interrupt is used to read the message
   //received by the SENT1 data registers.
   clear_interrupt(INT_SENT1);
   enable_interrupts(INT_SENT1);
   
   enable_interrupts(GLOBAL);
   
   //Enable SENT1 peripheral as a receiver, with HW CRC enabled, pause pulse
   //enabled and 6 data nibbles with a tick time of 90us.
   setup_sent(1, SENT_MODE_RECEIVER | SENT_ENABLE_HW_CRC | SENT_USES_PAUSE_PULSE | SENT_DATA_NIBBLES_6, 90);
   
   while(TRUE)
   {
      if(NewRxData)
      {
         rCount = GetSentCount(RxData);
         NewRxData = FALSE;
         
         printf("\r\nrCount: %lu", rCount);
      }
   }
}
