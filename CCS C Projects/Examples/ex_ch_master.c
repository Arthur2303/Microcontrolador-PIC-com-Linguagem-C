////////////////////////////////////////////////////////////////////////////
////                           ex_ch_master.c                           ////
////                                                                    ////
//// The follow example shows how to program the Slave processor from   ////
//// the Master processor and how to communicate between the            ////
//// processors using the Master Slave Interface (MSI).                 ////
////                                                                    ////
//// This example works in conjunction with ex_ch_slave.c.  This        ////
//// program is for the Master processor of a dual core device,         ////
//// dsPIC33CHxxMPxxx.  This example flashes the specified LED at the   ////
//// specified rate and sends a message to the Slave processor, either  ////
//// by using one of the mailbox registers or the FIFO write registers, ////
//// to initiate an ADC reading by the Slave processor.  The Slave      ////
//// then communicates the ADC result back to the Master processor      ////
//// using one of the mailbox registers or it's FIFO write registers,   ////
//// Master's FIFO read registers.  Then the Master displays the ADC    ////
//// reading using the UART peripheral to a serial terminal program.    ////
////                                                                    ////
//// This example will work on the PCD compiler on one of the dual core ////
//// devices.  Change the device, clock, LED and UART pins has needed   ////
//// for your hardware.                                                 ////
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

// The following defines set were in the Master's flash the Slave program
// is stored.  The defines SLAVE_PROGRAM_START_ADDR needs to match the same
// defines in ex_ch_slave.c.  Because of how the memory is organized
// SLAVE_PROGRAM_START_ADDR needs to be on a boundary of two instructions,
// multiple of four, it's recommended putting it on a boundary of the flash
// erase size.  The define SLAVE_PROGRAM_END_ADDR sets the last address in the
// Master's flash for the Slave program, this just needs to be big enough to
// hold the Slave program.  It's recommended to set it so that the area is a
// multiple of the flash erase size and big enough to contain the maximum size
// the Slave program can be.  The define SLAVE_PROGRAM_INSTRUCTIONS sets the
// number of instructions that this example actual copies to the Slave 
// processor's PRAM.  This just needs to be a multiple of two bigger then the
// actual number of Slave program instructions, this can be adjusted based on
// the actual size of the Slave program.
#define SLAVE_PROGRAM_START_ADDR       0x10000  //this define should match what's in ex_ch_slave.c
#define SLAVE_PROGRAM_END_ADDR         0x18FFE
#define SLAVE_PROGRAM_INSTRUCTIONS     512

// This define specifies whether to use the mailbox register or the FIFO 
// register to communicate between the Master and Slave processor.  If the 
// following define is not commented out then this example uses the mailbox 
// registers, if commented out then this example use the FIFO registers.  The
// state of this define needs to be the same as in ex_ch_slave.c file.
#define MASTER_SLAVE_COMM_USES_MAILBOXES

// Specified LED pin must be assigned to Master processor's control.
#define MASTER_LED_PIN           PIN_E0
#define MASTER_LED_FLASH_RATE    250      //in ms
#define MASTER_ADC_UPDATE_RATE   100      //in ms

#include <33CH512MP508.h>
#fuses WDT_SW, S1_WDT_SW, S1_PLLWAIT

//The following configuration fuses assign which processor controls the I/O pins.
//When a bit is set the corresponding PORT pin is controlled by the Master, and 
//when a bit is cleared the corresponding PORT pin is controlled by the Slave.
//By default all pin are controlled by the Master.
#fuses CPRA=0xFFF7 //PIN_A3 controlled by Slave, all other PORTA pins controlled by Master
#fuses CPRE=0xFF7F //PIN_E7 controlled by Slave, all other PORTE pins controlled by Master

#ifdef MASTER_SLAVE_COMM_USES_MAILBOXES
 //The following configuration fuses sets the direction of the mailbox register
 //When a bit is set the corresponding mailbox is set for Master read, Slave write,
 //and when a bit is cleared the corresponding mailbox is set for Master write, 
 //Slave read.  By default all mailboxes are set for Master read, Slave write
 //direction.
 #fuses MBXDIR=0xFFFE //Mailbox 0 set for Master write, mailbox 1-15 set for Master read
 
 //The following configuration fuses assigns the mailbox registers to the 
 //the handshaking protocols.
 #fuses HSPA=0  //Mailbox 0 assigned to handshaking protocol A
 #fuses HSPB=1  //Mailbox 1 assigned to handshaking protocol B
 
 //The following configuration fuses are used to enable the handshaking
 //protocols.
 #fuses HSA_ENABLED
 #fuses HSB_ENABLED
#endif

#use delay(clock=100MHz, crystal=8Mhz, SLAVE: clock=200MHz, crystal=8MHz)

#pin_select U1TX=PIN_C11
#pin_select U1RX=PIN_C10

#use rs232(UART1,baud=115200)

//Sets aside Master's flash memory used to store the Slave's program.  This
//keeps the compiler from using this memory region.
#org SLAVE_PROGRAM_START_ADDR,SLAVE_PROGRAM_END_ADDR {}

//Import Slave's program into Master's flash, Slave's program must be built
//before Master's program.
#import(file="ex_ch_slave.hex", HEX, range=SLAVE_PROGRAM_START_ADDR:SLAVE_PROGRAM_END_ADDR)

#include <tick.c>
#define TickDifference(a,b)   (a-b)

#ifdef MASTER_SLAVE_COMM_USES_MAILBOXES
unsigned int16 MailboxData;
int1 HasMailboxData = FALSE;

#INT_MSIB
void msib_isr(void)
{
   unsigned int16 Data;
   
   //Get data from MSI Mailbox 1
   Data = msi_read_mailbox(1);
   
   if(!HasMailboxData)
   {
      MailboxData = Data;
   
      HasMailboxData = TRUE;
   }
}
#endif

void main(void)
{
   unsigned int16 SlaveVerify;
   TICK cTick, LedTick, ADCTick;
   int1 SlaveRunning = FALSE;
   unsigned int16 Reading;
   
   delay_ms(1000);

   printf("\r\nex_ch_master.c - %s\r\n", getenv("DEVICE"));
   
   load_slave_program(SLAVE_PROGRAM_START_ADDR, SLAVE_PROGRAM_INSTRUCTIONS);
   SlaveVerify = verify_slave_program(SLAVE_PROGRAM_START_ADDR, SLAVE_PROGRAM_INSTRUCTIONS);
   
   if(SlaveVerify == 0) //no verify errors
   {
      printf("\r\nSlave Program Verified");
      
     #ifdef MASTER_SLAVE_COMM_USES_MAILBOXES
      setup_msi(MSI_SLAVE_ENABLE);
      enable_interrupts(INT_MSIB);
     #else
      setup_msi(MSI_SLAVE_ENABLE | MSI_WRITE_FIFO_ENABLED | MSI_READ_FIFO_ENABLED);
     #endif
   }
   else
      printf("\r\nSlave Verify Errors - %lu", SlaveVerify);
   
   LedTick = ADCTick = TickGet();
   
   enable_interrupts(GLOBAL);
   
   while(TRUE)
   {
      cTick = TickGet();
      
      if(TickDifference(cTick, LedTick) >= ((MASTER_LED_FLASH_RATE * TICKS_PER_SECOND) / 1000))
      {
         output_toggle(MASTER_LED_PIN);
         
         LedTick = cTick;
      }
      
      if(SlaveRunning)
      {
         //At specified rate send data to Slave processor to initiate a reading
         //of the ADC
         if(TickDifference(cTick, ADCTick) >= ((MASTER_ADC_UPDATE_RATE * TICKS_PER_SECOND) / 1000))
         {
           #ifdef MASTER_SLAVE_COMM_USES_MAILBOXES
            msi_write_mailbox(0, 0x0001);
           #else
            msi_write_fifo(0x0001);
           #endif
            
            ADCTick = cTick;
         }
         
        #ifdef MASTER_SLAVE_COMM_USES_MAILBOXES
         if(HasMailboxData)
         {
            Reading = MailboxData;
            HasMailboxData = FALSE;
            
            printf("\r\nSlave ADC Reading: %04LX", Reading);
         }
        #else
         //Check is MSI Read FIFO as data to read
         if((msi_fifo_status() & MSI_READ_FIFO_EMPTY) != MSI_READ_FIFO_EMPTY)
         {
            //Get data from MSI Read FIFO
            msi_read_fifo(&Reading);
            
            printf("\r\nSlave ADC Reading: %04LX", Reading);
         }
        #endif
      }
      else
      {
         if((msi_status() & MSI_SLAVE_IN_RESET) == 0)
         {
            printf("\r\nSlave Running");
            
            SlaveRunning = TRUE;
         }
      }
   }
}
