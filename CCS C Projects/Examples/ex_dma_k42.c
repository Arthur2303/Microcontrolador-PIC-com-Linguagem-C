///////////////////////////////////////////////////////////////////////////
////                            ex_dma_k42.c                           ////
////                                                                   ////
//// Example showing how to setup and use the DMA peripheral on        ////
//// PIC18FxxK42 devices.  This example sets up the PIC so that it     ////
//// receives data from the SPI and then transfers if via DMA to the   ////
//// UART peripheral.                                                  ////
////                                                                   ////
//// The SPI1 peripheral is setup as an SPI slave device and UART1 is  ////
//// setup for a baud of 115200, 8 data bits, 1 stop bit and no        ////
//// parity.                                                           ////
////                                                                   ////
//// This example will work with the PCH compiler.  The following      ////
//// conditional compilation lines are used to include a valid device. ////
//// Change the device, clock, UART pins and SPI pins for your         ////
//// hardware if needed.                                               ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2019 Custom Computer Services           ////
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

#include <18F46K42.h>
#fuses NOWDT
#use delay(internal=64MHz)

//UART pin assignments and setup
#pin_select U1TX=PIN_C6
#pin_select U1RX=PIN_C7

#use rs232(UART1, baud=115200)

//SPI pin assignments and setup
#pin_select SCK1IN=PIN_C3
#pin_select SDI1=PIN_C4
#pin_select SDO1=PIN_C5
#pin_select SS1IN=PIN_A5

#use spi(SLAVE, SPI1, ENABLE=PIN_A5,  MODE=0, bits=8)

void main(void)
{
   //Setup DMA1 start trigger to SPI1RX, and abort trigger to none
   setup_dma(1, DMA_TRIGGER_SPI1RX, DMA_TRIGGER_NONE);
   
   //Set DMA1 source address to be from SFR or GPR memory, source and destination
   //address to remain unchanged after transfer and for HW to trigger transfer.
   //Set DMA1 source address to be SPI1RXB register, and set DMA1 destination
   //address to be U1TXB register.  Set DMA1 source and destination counts to be 1.
   dma_start(1, DMA_SOURCE_ADDR_IS_SFR_GPR | DMA_SOURCE_ADDR_UNCHANGED | DMA_DEST_ADDR_UNCHANGED |
                DMA_HW_TRIGGER_STARTS_XFER, getenv("SFR:U1TXB"), getenv("SFR:SPI1RXB"), 1, 1);
   
   while(TRUE)
   {
   }
}
