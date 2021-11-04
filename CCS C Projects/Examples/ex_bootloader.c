///////////////////////////////////////////////////////////////////////////
////                      EX_BOOTLOADER.C                              ////
////                                                                   ////
////  This program is an example stand alone bootloader.               ////
////                                                                   ////
////  This program must be loaded into a target chip using a device    ////
////  programmer.  Afterwards this program may be used to load new     ////
////  versions of the application program.                             ////
////                                                                   ////
////  This bootloader is designed to detect pin B5 low on reset.  It   ////
////  will then use the RS232 link to download a new program.          ////
////  Otherwise the application program is started.                    ////
////                                                                   ////
////  Use an RS232 link and the SIOW.EXE or CCS_BOOTLOADER.exe program ////
////  to load a new HEX file into the target chip.                     ////
////                                                                   ////
////  Uncomment the define BOOTLOADER_MODE2X to use an alternate       ////
////  bootloader that receives each line in the hex file twice and     ////
////  compares them before acknowledging the line and writing it to    ////
////  the target chip's memory.  This mode only works with the         ////
////  CCS_BOOTLOADER.exe program, add MODE2X to the command line to    ////
////  use this mode.                                                   ////
////                                                                   ////
////  Depending on the PIC the bootloader is being built for or if     ////
////  changes are made to the code the bootloader may not fit in the   ////
////  program memory set a side for it by default for this example.    ////
////  If that happens the size set aside for the bootloader can be     ////
////  changed by defining LOADER_END to the new size before the        ////
////  bootloader.h file is included, this define must be made in both  ////
////  the bootloader and application programs for the bootloader to    ////
////  work correctly.  When using the LOADER_END define to change the  ////
////  size of the bootloader memory it must be defined such that the   ////
////  value is one less then a multiple of the erase size for the PIC. ////
////  getenv("FLASH_ERASE_SIZE") can be used to determine the erase    ////
////  size of the PIC, for example the PIC18F4520 erase size is 64.    ////
////  So LOADER_END can be define in multiples of 0x40 minus one,      ////
////  0x3F, 0x7F, 0xBF, 0xFF, 0x13F, etc.                              ////
////                                                                   ////
////  This example will work with the PCM and PCH compilers.  The      ////
////  following conditional compilation lines are used to include a    ////
////  valid device for each compiler.  Change the device, clock and    ////
////  RS232 pins for your hardware if needed.                          ////
///////////////////////////////////////////////////////////////////////////
////          (C) Copyright 1996,2021 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
 #include <16F887.h>
#elif defined(__PCH__)
 #include <18F4520.h>
#endif

#use delay( crystal=20MHz )
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)
#define PUSH_BUTTON PIN_A4

#define _BOOTLOADER
//#define BOOTLOADER_MODE2X

#include <bootloader.h>
#include <loader.c>

#org LOADER_END+1,LOADER_END+3
void application(void) {
  while(TRUE);
}

void main(void) {

   if(!input(PUSH_BUTTON))
   {
      printf("\r\nBootloader Version 1.0\r\n");
   
      // Let the user know it is ready to accept a download
      printf("\r\nWaiting for download...");
      load_program();
   }

   application();
}

#int_global
void isr(void) {
   jump_to_isr(LOADER_END+5*(getenv("BITS_PER_INSTRUCTION")/8));
}


