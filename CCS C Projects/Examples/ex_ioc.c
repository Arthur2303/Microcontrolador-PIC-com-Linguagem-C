///////////////////////////////////////////////////////////////////////////
////                             ex_ioc.c                              ////
////                                                                   ////
//// An example showing how to use the Interrupt On Change (IOC)       ////
//// feature of the PIC, including how to use the IOC Interrupt to     ////
//// determine which IOC enabled pin caused the IOC interrupt to       ////
//// occur.                                                            ////
////                                                                   ////
//// This example sets up PIN_B0 and PIN_B1 as IOC pins to occur when  ////
//// a change from high to low is detected on the pins.  Change the    ////
//// pins to use as the IOC pins as desired for your hardware.         ////
////                                                                   ////
//// This example will work on the PCM and PCH compilers for devices   ////
//// that have the INT_IOC, INT_IOC_XX, INT_IOC_XX_H2L and             ////
//// INT_IOC_XX_L2H defines in the INT section of their header files.  ////
//// XX being the IOC pin, INT_IOC_A0 for example.  Change the device, ////
//// clock and RS232 pins for your hardware if needed.                 ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////         (C) Copyright 1996,2019 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                      http://www.ccsinfo.com                       ////
///////////////////////////////////////////////////////////////////////////
#if defined(__PCM__)
#include <16F1779.h>
#elif defined(__PCH__)
#include <18F4520.h>
#endif

#fuses NOWDT
#use delay(internal=8MHz)

#define PIN_RS232_TX    PIN_C6
#define PIN_RS232_RX    PIN_C7

#pin_select U1TX=PIN_RS232_TX
#pin_select U1RX=PIN_RS232_RX

#use rs232(xmit=PIN_RS232_TX, rcv=PIN_RS232_RX, baud=115200, TRANSMIT_BUFFER=256, TXISR)

//Common IOC Interrupt Service Routine (ISR), all IOC pins are serviced by this ISR
#INT_IOC
void ioc_isr(void
{
   //Check if IOC flag for PIN_B0 is set
   if(interrupt_active(INT_IOC_B0))
   {
      printf("\r\nPush Button B0 Pressed");
   }
   
   //Check if IOC flag for PIN_B1 is set
   if(interrupt_active(INT_IOC_B1))
   {
      printf("\r\nPush Button B1 Pressed");
   }
}

void main(void)
{
   printf("\r\nex_ioc.c - %s\r\n", getenv("DEVICE"));
   
   //Enable IOC interrupt for PIN_B0 when changing from high to low
   enable_interrupts(INT_IOC_B0_H2L);
   //Enable IOC interrupt for PIN_B1 when changing from high to low
   enable_interrupts(INT_IOC_B1_H2L);
   
   enable_interrupts(GLOBAL);

   while(TRUE)
   {
   }
}
