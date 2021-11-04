///////////////////////////////////////////////////////////////////////////
////                       ex_sent_transmitter.c                       ////
////                                                                   ////
//// Example showing how to setup and use the Single-Edge Nibble       ////
//// Transmission (SENT) peripheral.  A SENT Data frame consist of a   ////
//// Sync period, a status  nibble, 1-6 data nibbles, a CRC nibble and ////
//// an optional pause pulse, which can be used to provide a fixed     ////
//// message frame time period.                                        ////
////                                                                   ////
//// This example sets up the SENT1 peripheral to be an asynchronous   ////
//// transmitter, to send 6 data nibbles with a tick time of 90us and  ////
//// a frame time, message period, of 50 ms.  Additionally it enables  ////
//// the HW CRC and pause pulse.  This example can easily be modified  ////
//// into an synchronous transmitter by changing the option            ////
//// SENT_MODE_TRANSMITTER_ASYNCHRONOUS in setup_sent() to             ////
//// SENT_MODE_TRANSMITTER_SYNCHRONOUS and moving the call to          ////
//// sent_putd() before setup_sent() to after it.                      ////
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

#pin_select SENT1=PIN_B3

SENT_DATA_TYPE Count = {0,0,0,0,0,0,0,0};

void IncSentCount(SENT_DATA_TYPE *Ptr)
{
   if(++Ptr->DATA1 == 0)
   {
      if(++Ptr->DATA2 == 0)
      {
         if(++Ptr->DATA3 == 0)
         {
            if(++Ptr->DATA4 == 0)
            {
               if(++Ptr->DATA5 == 0)
               {
                  Ptr->DATA6++;
               }
            }
         }
      }
   }
}

//SENT1 ISR used to send data, when setup as an asynchronous transmitter the 
//next message to send needs to be loaded before the next frame's status nibble
//is sent.  Using the SENT1 ISR ensures that it's loaded before then.
#INT_SENT1
void sent1_isr(void)
{
   IncSentCount(&Count);
   
   sent_putd(1, Count);   
}

void main(void)
{
   delay_ms(1000);
   
   printf("\r\nex_sent_transmitter.c - %s\r\n", getenv("DEVICE"));
   
   //Clear and enable SENT1 interrupt, interrupt is used to load next data to 
   //send into SENT1 data registers.
   clear_interrupt(INT_SENT1);
   enable_interrupts(INT_SENT1);
   
   enable_interrupts(GLOBAL);
   
   //Since this example sets up SENT1 peripheral as an asynchronous transmitter
   //data needs to be pre-loaded before the peripheral is enabled, because it
   //starts sending once the peripheral is enabled.
   sent_putd(1, Count);
   
   //Enable SENT1 peripheral as an asynchronous transmitter, with HW CRC enabled, 
   //pause pulse enabled and 6 data nibbles a tick time of 90us and a frame time
   //of 50ms.
   setup_sent(1, SENT_MODE_TRANSMITTER_ASYNCHRONOUS | SENT_ENABLE_HW_CRC | SENT_USES_PAUSE_PULSE | SENT_DATA_NIBBLES_6, 90, 50000);
   
   while(TRUE)
   {
   }
}
