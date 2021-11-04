///////////////////////////////////////////////////////////////////////////
////                         ex_bootloader_dp.c                        ////
////                                                                   ////
////  This program uses the loader driver to show how you can read and ////
////  write to the inactive partition when in dual partition mode.     ////
////                                                                   ////
////  This program must be loaded into a target chip using a device    ////
////  programmer the first time.  Afterwards modified versions of this ////
////  program may be loaded into the inactive partition of the program ////
////  memory, then assuming the PARTITION_SEQUENCE is less then        ////
////  current active partition the new program will execute after it   ////
////  resets.                                                          ////
////                                                                   ////
////  This bootloader is designed to detect a pushbutton low on reset. ////
////  It will then use the RS232 link to download a new program.       ////
////  Otherwise the application program is executed.                   ////
////                                                                   ////
////  After each good line, the loader sends an ACK character.  The    ////
////  driver uses XON/XOFF flow control.  Also, any buffer on the PC   ////
////  UART must be turned off, or to its lowest setting, otherwise it  ////
////  will miss data.                                                  ////
////                                                                   ////
////  Use an RS232 link and the SIOW.EXE program to load a new HEX     ////
////  file into the target chip.                                       ////
////                                                                   ////
////  This example will work with the PCD compiler on devices that     ////
////  have a dual partition mode.  The following conditional           ////
////  compilation lines are used to include a valid device.  Change    ////
////  the device, clock, and RS232 pins for your hardware if needed.   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2020 Custom Computer Services           ////
////                                                                   ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#include <24FJ512GA610.h>
#fuses NOWDT
#device DUAL_PARTITION              //Setup for dual partition mode
#device PARTITION_SEQUENCE = 4095   //This value controls which partition is run
                                    //after a reset, lower value is run so when
                                    //a new partition is built it should be 
                                    //decreased by at least one.
#device ICSP=2
#use delay(internal=8MHz)

#pin_select U1TX=PIN_F5
#pin_select U1RX=PIN_F4

#use rs232(UART1, baud=38400, stream=U1_STREAM, errors)

#define PUSH_BUTTON  PIN_D13

/////////////////// Bootloader related defines and includes ///////////////////

#define BOOTLOADER_STREAM     U1_STREAM

#include <loader_pcd_dp.c>

///////////////////////////////////////////////////////////////////////////////

void main(void)
{
   // Bootloader code
   if(input(PUSH_BUTTON) == 0)
   {
      delay_us(10);
      
      if(input(PUSH_BUTTON) == 0)
      {
         fprintf(U1_STREAM, "\r\nBootloader Version 1.00\r\n");

         // Let the user know it is ready to accept a download
         fprintf(U1_STREAM, "\r\nWaiting for download...");
   
         // Load the program
         load_program();
      }
   }
   
   //Application code   
   fprintf(U1_STREAM, "\r\nApplication Version 1.00\r\n");

   while(TRUE)
   {
      output_toggle(PIN_A7);
      delay_ms(250);
   }
}
