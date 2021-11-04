///////////////////////////////////////////////////////////////////////////
////                      EX_PCD_BOOTLOADER_AIVT.C                     ////
////                                                                   ////
////  This program is an example standalone bootloader which uses the  ////
////  Alternate Interrupt Vector Table (AIVT) for the application's    ////
////  Interrupt Service Routines (ISR).                                ////
////                                                                   ////
////  The define LOADER_PAGES sets the number of erase pages the       ////
////  boot segment uses, this needs to be 1 more then is actually      ////
////  needed.  The extra page is were the AIVT is located and no       ////
////  executable code can be located in it.                            ////
////                                                                   ////
////  The configuration fuse BSS=x sets the size of the boot segment   ////
////  in pages.  X is the one's complement of LOADER_PAGES anded by    ////
////  0x1FFF.                                                          ////
////                                                                   ////
////  The configuration fuses BS and AIVT are required to enable the   ////
////  boot segment and Alternate Interrupt Vector table.               ////
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

#define LOADER_PAGES    4

#include <33EV32GM102.h>
#fuses NOWDT
#fuses BS            //enable the boot segment, required to use AIVT
#fuses NOBSS         //set boot segment security to none
#fuses BSS=0x1FFB    //set boot segment size, (~4) & 0x1FFF = 0x1FFB
#fuses AIVT          //enable the alternate interrupt vector table, required to use AIVT
#fuses WPCFG         //enable write protection on configuration segment, enabled to guarantee configuration fuses are not changed by bootloader
#fuses NOIOL1WAY     //allow multiple reconfigurations of peripheral pins, so application can assign it's peripheral pins
#use delay(internal=120MHz)

#pin_select U1TX=PIN_B6
#pin_select U1RX=PIN_B7
#use rs232(BAUD=115200,UART1,errors,stream=BOOTLOADER)

#define BOOTLOADER_STREAM  BOOTLOADER

#define PUSH_BUTTON PIN_B5

#define _bootloader
//#define BOOTLOADER_MODE2X

#include <pcd_bootloader_AIVT.h>
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
      
      fprintf(BOOTLOADER_STREAM, "\r\nBootloader Version 1.0");
      
      // Let the user know it is ready to accept a download
      fprintf(BOOTLOADER_STREAM, "\r\nWaiting for download...");

      // Load the program
      load_program();
   }

   application();
}
