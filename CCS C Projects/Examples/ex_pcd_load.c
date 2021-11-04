///////////////////////////////////////////////////////////////////////////
////                        EX_PCD_LOAD.c                              ////
////                                                                   ////
////  This program uses the loader driver to show how you can          ////
////  read and write to an internal program memory.                    ////
////                                                                   ////
////  The PCD_LOADER.C will take an Intel 8-bit Hex file over RS232    ////
////  and modify the flash program memory with the new code.           ////
////                                                                   ////
////  After each good line, the loader sends an ACK character.  The    ////
////  driver uses XON/XOFF flow control.  Also, any buffer on the PC   ////
////  UART must be turned off, or to its lowest setting, otherwise it  ////
////  will miss data.                                                  ////
////                                                                   ////
////  Use an RS232 link and the SIOW.EXE program to load a new HEX     ////
////  file into the target chip.                                       ////
////                                                                   ////
////  Select either the ICD or your own RS232-to-PC connection for the ////
////  text I/O.                                                        ////
////                                                                   ////
////  This example will work with the PCD compiler.  The following     ////
////  conditional compilation lines are used to include a valid device ////
////  for each compiler.  Change the device, clock, and RS232 pins for ////
////  your hardware if needed.                                         ////
///////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2013,2018 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

//These defines control the chip family that this program will compile for.
//The setup for 4 different chips, one from each of these chip families,
//is listed below.
//#define DSPIC33FJ
//#define DSPIC30F
#define PIC24FJ
//#define PIC24HJ

//Here is the sample setup for 4 different chips.  The #if defined
//allows for rapid switching between chips prior to compiling.
#include <33FJ128GP706.h>
#use delay(crystal=12MHz)
#use rs232(icd)               //Text through the ICD
//#use rs232(BAUD=9600,UART2) //Text through the UART

#include <loader_pcd.c>

void main(void)
{
   // Clears out the rs232 buffer
   delay_ms (1000);
   while (kbhit())
   {
      getc();
   }

   // Loop until 'L' is pressed
   do
   {
      printf("\r\nSoftware Version A!\r\n");
      printf("Press L to download new software.\r\n");
   } while (getc() != 'L');

   // Let the user know it is ready to accept a download
   printf("\r\nWaiting for download...");

   // Load the program
   load_program();
}


