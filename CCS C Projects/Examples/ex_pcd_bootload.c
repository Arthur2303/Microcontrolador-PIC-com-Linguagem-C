///////////////////////////////////////////////////////////////////////////
////                       EX_PCD_BOOTLOAD.c                           ////
////                                                                   ////
////  This program is an example application program that can be       ////
////  be loaded, recompiled and reloaded by a standalone bootloader.   ////
////                                                                   ////
////  Before loading this program the bootloader (EX_PCD_BOOTLOADER.c) ////
////  must be compiled and loaded into the target chip.                ////
////                                                                   ////
////  Use an RS232 link and the SIOW.EXE program to load this HEX file ////
////  into the target chip.  See ex_pcd_bootloader.c for more          ////
////  information.                                                     ////
////                                                                   ////
////  Uncomment the define BOOTLOADER_MODE2X to use an alternate       ////
////  bootloader that receives each line in the hex file twice and     ////
////  compares them before acknowledging the line and writing it to    ////
////  the target chip's memory.  This mode only works with the         ////
////  CCS_BOOTLOADER.exe program, add MODE2X to the command line to    ////
////  use this mode.                                                   ////
////                                                                   ////
////  Select either the ICD or your own RS232-to-PC connection for the ////
////  text I/O.                                                        ////
////                                                                   ////
////  This example will work with the PCD compiler.  Change the        ////
////  device, clock and RS232 pins for your hardware if needed.        ////
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

#include <24EP256GP206.h>
#use delay(internal=32MHz)

#define UART_TX_PIN     PIN_A4
#define UART_RX_PIN     PIN_A9
#define UART_BAUD       115200

#define PUSH_BUTTON     PIN_C3

#pin_select U1TX=UART_TX_PIN
#pin_select U1RX=UART_RX_PIN

#use rs232(xmit=UART_TX_PIN, rcv=UART_RX_PIN, baud=UART_BAUD)

//#define BOOTLOADER_MODE2X

//This is a necessary include file.  It reserves space so that the
//bootloader is not overwritten.
#include <pcd_bootloader.h>

rom char version[] = "Application Version 1.0";

void main()
{
   delay_ms(100);

   printf("\r\n%s\r\n", version);
   
   printf("You can put whatever code you want here.\r\n");
   
   printf("So enjoy!\r\n");
   
   while(TRUE)
   {
   }
}
