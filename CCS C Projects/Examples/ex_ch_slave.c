////////////////////////////////////////////////////////////////////////////
////                           ex_ch_slave.c                            ////
////                                                                    ////
//// The following example shows how to build a Slave processor program ////
//// that can be loaded by the Master processor onto the Slave          ////
//// processor and how to communicate between the processors using the  ////
//// Master Slave Interface (MSI).                                      ////
////                                                                    ////
//// This example works in conjunction with ex_ch_master.c.  This       ////
//// program is for the Slave processor of a dual core device,          ////
//// dsPIC33CHxxMPxxx.  This example flashes the specified LED at the   ////
//// specified rate and receives a message from the Master processor,   ////
//// via one of the mailbox registers or it's FIFO read registers, to   ////
//// initiate an ADC reading.  The Slave then communicates the ADC      ////
//// reading back to the Master processor using one of the mailbox      ////
//// registers or it's FIFO write registers.                            ////
////                                                                    ////
//// This example will work on the PCD compiler on one of the dual core ////
//// devices.  Change the device, clock and LED pin as needed for your  ////
//// hardware.                                                          ////
////////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2019 Custom Computer Services            ////
////                                                                    ////
//// This source code may only be used by licensed users of the CCS C   ////
//// compiler or to users who purchased the rights to use this code.    ////
//// This source code may only be distributed to other licensed users   ////
//// of the CCS C compiler or other users who purchased the rights to   ////
//// this code.  The rights to use this code can only be granted        ////
//// by CCS.  No other use, reproduction or distribution is permitted   ////
//// without written permission.  Derivative programs created using     ////
//// this software in object code form are not restricted in any way.   ////
////                                                                    ////
////                     http://www.ccsinfo.com                         ////
////////////////////////////////////////////////////////////////////////////


// The following defines sets the offset in the Master's flash were the Slave
// program is stored.  The defines SLAVE_PROGRAM_START_ADDR needs to match the
// same define in ex_ch_master.c.  Because of how the memory is organized
// SLAVE_PROGRAM_START_ADDR needs to be on a boundary of two instructions,
// multiple of four, it's recommend putting it on a boundary of
// the flash erase size.
#define SLAVE_PROGRAM_START_ADDR       0x10000  //this define should match what's in ex_ch_slave.c

// This define specifies whether to use mailbox register or the FIFO register to
// communicate between the Master and Slave processor.  If the following define
// is not commented out then this example uses the mailbox registers, if 
// commented out then this example use the FIFO registers.  The state of this
// define needs to be the same as in ex_ch_master.c file.
#define MASTER_SLAVE_COMM_USES_MAILBOXES

// Specified pin must be assigned to Slave processor's control
#define SLAVE_LED_PIN            PIN_E7
#define SLAVE_LED_FLASH_RATE     100      //in ms

#include <33CH512MP508S1.h>
#device ADC=12

#use delay(clock=200MHz, crystal=8MHz)

//This causes the compiler to generate the hex file with the specified offset.
//This needs to be done so that the code in the hex file is located in the same
//location that it will be stored in the Master's flash, so it matches the
//range the Master is importing.
#export(file=ex_ch_slave.hex, HEX, offset=SLAVE_PROGRAM_START_ADDR)

#include <tick.c>
#define TickDifference(a,b)   (a-b)

#ifdef MASTER_SLAVE_COMM_USES_MAILBOXES
unsigned int16 MailboxData;
int1 HasMailboxData = FALSE;

#INT_MSIA
void msia_isr(void)
{
   unsigned int16 Data;
   
   //Get data from MSI Mailbox 0
   Data = msi_read_mailbox(0);
   
   if(!HasMailboxData)
   {
      MailboxData = Data;
   
      HasMailboxData = TRUE;
   }
}
#endif

void main(void)
{
   TICK cTick, LedTick;
   unsigned int16 Data;
   unsigned int16 Reading;
   
   delay_ms(100);
   
   setup_adc_ports(sAN0, VSS_VDD);
   setup_adc(ADC_CLOCK_SYSTEM | ADC_CLOCK_DIV_32 | ADC_SHARED_TAD_MUL_513);
   setup_dedicated_adc(0, ADC_DEDICATED_CLOCK_DIV_2 | ADC_DEDICATED_TAD_MUL_513);
   
   set_dedicated_adc_channel(0, ADC_CHANNEL_AN0);
   
   LedTick = TickGet();
   
  #ifndef MASTER_SLAVE_COMM_USES_MAILBOXES
   setup_msi(MSI_WRITE_FIFO_ENABLED | MSI_READ_FIFO_ENABLED);
  #else
   enable_interrupts(INT_MSIA);
  #endif
  
   enable_interrupts(GLOBAL);

   while(TRUE)
   {
       cTick = TickGet();
      
      if(TickDifference(cTick, LedTick) >= ((SLAVE_LED_FLASH_RATE * TICKS_PER_SECOND) / 1000))
      {
         output_toggle(SLAVE_LED_PIN);
         
         LedTick = cTick;
      }
      
     #ifdef MASTER_SLAVE_COMM_USES_MAILBOXES
      if(HasMailboxData)
      {
         Data = MailboxData;
         
         HasMailboxData = FALSE;
     #else
      if((msi_fifo_status() & MSI_READ_FIFO_EMPTY) != MSI_READ_FIFO_EMPTY)
      {
         msi_read_fifo(&Data);
     #endif
      
         if(Data == 0x0001)
         {
            Reading = read_adc();
            
           #ifdef MASTER_SLAVE_COMM_USES_MAILBOXES
            msi_write_mailbox(1, Reading);
           #else
            msi_write_fifo(Reading);
           #endif
         }
      }
   }
}
