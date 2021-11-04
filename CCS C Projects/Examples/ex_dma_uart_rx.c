///////////////////////////////////////////////////////////////////////////////
//                              ex_dma_uart_rx.c                             //
//                                                                           //
// Example showing how to setup DMA to receive UART data.  This example sets //
// up the DMA0 peripheral in repeated on-shot or continuous ping-pong, mode  //
// depending on the type of DMA peripheral the device has.                   //
//                                                                           //
// This example will work with the PCD compiler on devices with a DMA        //
// peripheral.  Change the device, clock and UART pins for your hardware as  //
// needed.                                                                   //
///////////////////////////////////////////////////////////////////////////////
//           (C) Copyright 1996,2017,2018 Custom Computer Services           //
//   This source code may only be used by licensed users of the CCS C        //
//   compiler.  This source code may only be distributed to other licensed   //
//   users of the CCS C compiler.  No other use, reproduction or             //
//   distribution is permitted without written permission.  Derivative       //
//   programs created using this software in object code form are not        //
//   restricted in any way.                                                  //
//                           http://www.ccsinfo.com                          //
///////////////////////////////////////////////////////////////////////////////

#include <24EP256GP206.h>
#use delay(crystal=20MHz)

#pin_select U1TX=PIN_A4
#pin_select U1RX=PIN_A9

#use rs232(UART1,baud=115200,stream=PC) //Text through the UART

#define DMA_BUFFER_SIZE    16 //This define sets the size of the buffer to use
                              //with DMA peripheral and the number of DMA
                              //transfers to do before the DMA interrupt
                              //occurs.

//Create DMA Receive Buffers in DMA memory region.
#bank_dma
#if getenv("DMA") == 2
char RxBuffer[DMA_BUFFER_SIZE];
#else
char RxBuffer[2][DMA_BUFFER_SIZE];
unsigned int8 RxIndex = 0;
#endif

char Data[DMA_BUFFER_SIZE];
int1 NewRxData = FALSE;

//DMA0 ISR
#INT_DMA0
void dma0_isr(void)
{
  #if getenv("DMA") == 2
   memcpy(Data, RxBuffer, 16);
  #else
   memcpy(Data, RxBuffer[RxIndex], 16);
   
   if(++RxIndex >= 2)
      RxIndex = 0;
  #endif
   
   NewRxData = TRUE;
}

void main(void)
{
   unsigned int8 i;
   
   delay_ms(1000);
   
   printf("\r\nex_dma_uart_rx.c - %s\r\n", getenv("DEVICE"));
   
  #if getenv("DMA") == 2
   //Setup DMA0 peripheral to be triggered by UART RX, sets transfer
   //size to byte and reloads source and destination addresses after
   //transfer is complete.
   setup_dma(0, DMA_TRIGGER_RDA, DMA_BYTE | DMA_RELOAD_ADDRESS);
  #else
   //Setup DMA0 peripheral to receive byte data from UART1 RX.
   setup_dma(0, DMA_IN_UART1, DMA_BYTE);
  #endif
   
   //Enable DMA0 ISR.
   enable_interrupts(INT_DMA0);
   enable_interrupts(GLOBAL);
   
  #if getenv("DMA") == 2
   //Starts DMA0 peripheral in repeated one-shot mode, assigns destination address to buffer and increments
   //destination address after each transfer.  Assigns source address to UART1 RXREG and doesn't change 
   //source address after each transfer.  Sets transfer size to DMA_BUFFER_SIZE, number of transfer to do
   //before DMA0 interrupt occurs.
   dma_start(0, DMA_SOURCE_ADDR_UNCHANGED | DMA_INC_DEST_ADDR | DMA_REPEATED | DMA_ONE_SHOT, RxBuffer, getenv("SFR:U1RXREG"), DMA_BUFFER_SIZE);
  #else
   //Start DMA0 peripheral in continuous Ping-Pong mode, assign buffers to use and
   //set transfer size to DMA_BUFFER_SIZE, number of transfer to do be DMA0 interrupt
   //occurs.
   dma_start(0, DMA_PING_PONG | DMA_CONTINOUS, RxBuffer[0], RxBuffer[1], (DMA_BUFFER_SIZE - 1));
  #endif

   while(TRUE)
   {
      if(NewRxData)
      {
         fprintf(PC,"\r\nUART RX Data: ");
         for(i=0;i<DMA_BUFFER_SIZE;i++)
            fputc(Data[i], PC);
         
         NewRxData = FALSE;
      }
   }
}
