///////////////////////////////////////////////////////////////////////////
////                      EX_PCD_BOOTLOADER.C                          ////
////                                                                   ////
////  This program is an example standalone bootloader.                ////
////                                                                   ////
////  This program must be loaded into a target chip using a device    ////
////  programmer.  Afterwards this program may be used to load new     ////
////  versions of the application program.                             ////
////                                                                   ////
////  This bootloader is designed to detect a pushbutton low on reset. ////
////  It will then use the RS232 link to download a new program.       ////
////  Otherwise the application program is started.                    ////
////                                                                   ////
////  Use an RS232 link and the SIOW.EXE program to load a new HEX     ////
////  file into the target chip. See example EX_PCD_BOOTLOAD.C.        ////
////                                                                   ////
////  Uncomment the define BOOTLOADER_MODE2X to use an alternate       ////
////  bootloader that receives each line in the hex file twice and     ////
////  compares them before acknowledging the line and writing it to    ////
////  the target chip's memory.  This mode only works with the         ////
////  CCS_BOOTLOADER.exe program, add MODE2X to the command line to    ////
////  use this mode.                                                   ////
////                                                                   ////
////  The config bits will be write protected, in software, by the     ////
////  rom_modify() function.  To allow config bits to change then      ////
////  you need to #define ROM_WRITE_CAN_MODIFY_CONFIG_BITS             ////
////                                                                   ////
////  Depending on the PIC the bootloader is being built for or if     ////
////  changes are made to the code the bootloader may not fit in the   ////
////  program memory set a side for it by default for this example.    ////
////  If that happens the size set aside for the bootloader can be     ////
////  changed by defining LOADER_PAGES to the number of erase pages    ////
////  to set aside for it before the pcd_bootloader.h is included,     ////
////  this define must be made in both the bootloader and application  ////
////  programs for the bootloader to work correctly.                   ////
////                                                                   ////
////  This example will work with the PCD compiler.  Change the        ////
////  device, clock, push button, and RS232 pins for your hardware if  ////
////  needed.                                                          ////
///////////////////////////////////////////////////////////////////////////
////         (C) Copyright 1996,2021 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

// see explanation above
//#define ROM_WRITE_CAN_MODIFY_CONFIG_BITS

#include <24EP256GP206.h>
#fuses NOIOL1WAY
#use delay(internal=32MHz)

#define UART_TX_PIN     PIN_A4
#define UART_RX_PIN     PIN_A9
#define UART_BAUD       115200

#define PUSH_BUTTON     PIN_C3

#pin_select U1TX=UART_TX_PIN
#pin_select U1RX=UART_RX_PIN

#use rs232(xmit=UART_TX_PIN, rcv=UART_RX_PIN, baud=UART_BAUD)

#define _bootloader
//#define BOOTLOADER_MODE2X

#include <pcd_bootloader.h>
#include <loader_pcd.c>

#org APPLICATION_START
void application(void)
{   
   while(TRUE);
}

void main(void)
{
   if(!input(PUSH_BUTTON))
   {
      delay_ms(140); // wait for PLL
      
      printf("\r\nBootloader Version 1.0\r\n");

      // Let the user know it is ready to accept a download
      printf("\r\nWaiting for download...");

      // Load the program
      load_program();
   }

   application();
}

#int_default
void isr(void)
{
   jump_to_isr(LOADER_END+5);
}

