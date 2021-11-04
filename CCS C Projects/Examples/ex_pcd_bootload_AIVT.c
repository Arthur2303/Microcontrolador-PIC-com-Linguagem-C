///////////////////////////////////////////////////////////////////////////
////                       EX_PCD_BOOTLOAD_AIVT.c                      ////
////                                                                   ////
////  This program is an example application program that can be       ////
////  be loaded, recompiled and reloaded by a standalone bootloader.   ////
////                                                                   ////
////  Before loading this program the bootloader                       ////
////  (EX_PCD_BOOTLOADER_AIVT.c) must be compiled and loaded into the  ////
////  target chip.                                                     ////
////                                                                   ////
////  Use an RS232 link and the SIOW.EXE program to load this HEX file ////
////  into the target chip.  See ex_pcd_bootloader_AIVT.c for more     ////
////  information.                                                     ////
////                                                                   ////
////  Uncomment the define BOOTLOADER_MODE2X to use an alternate       ////
////  bootloader that receives each line in the hex file twice and     ////
////  compares them before acknowledging the line and writing it to    ////
////  the target chip's memory.  This mode only works with the         ////
////  CCS_BOOTLOADER.exe program, add MODE2X to the command line to    ////
////  use this mode.                                                   ////
////                                                                   ////
////  This example will work with the PCD compiler on devices where    ////
////  the AIVT location is determined by the BSS configuration fuse.   ////
////  The following conditional compilation lines are used to include  ////
////  a valid device for the compiler.  Change the device, clock, push ////
////  button, and RS232 pins for your hardware if needed.              ////
///////////////////////////////////////////////////////////////////////////
////          (C) Copyright 1996,2020 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#define LOADER_PAGES    4  //must match what's in ex_pcd_bootloader_AIVT.c

#include <33EV32GM102.h>
#use delay(internal=120MHz)

#pin_select U1TX=PIN_B6
#pin_select U1RX=PIN_B7
#use rs232(BAUD=115200,UART1,errors) //Text through the UART

#define PUSH_BUTTON  PIN_B5
#define LED          PIN_B4

//This is a necessary include file.  It reserves space so that the
//bootloader is not overwritten.
#include <pcd_bootloader_AIVT.h>

#INT_TIMER1
void time1_isr(void)
{
   output_toggle(LED);
}

rom char version[] = "Application Version 1.0";

void main()
{
   unsigned int16 Count = 0;

   delay_ms(100);

   printf("\r\n%s\r\n", version);
   
   printf("You can put whatever code you want here.\r\n");
   
   printf("So enjoy!\r\n");
   
   setup_timer1(TMR_INTERNAL | TMR_DIV_BY_256, 35155); //~150ms interrupt period
   
   enable_interrupts(INTR_ALTERNATE);  //set PIC to use alternate IVT
   enable_interrupts(INTR_GLOBAL);     //enable global interrupts
   
   enable_interrupts(INT_TIMER1);      //enable Timer 1 interrupt
   
   while(TRUE)
   {
      printf("\r\nCount: %lu", Count++);
      
      delay_ms(500);
   }
}
