///////////////////////////////////////////////////////////////////////////
////                    EX_USB_BOOTLOADER.C                            ////
////                                                                   ////
////  This program is an example stand alone USB bootloader.  It uses  ////
////  the communication device class (CDC) that creates a virtual COM  ////
////  port on your PC.  The bootloader contains its own set of USB     ////
////  and bootloader code in low memory, the high memory will contain  ////
////  the application (and if you are developing a USB application,    ////
////  this will hold its own USB code).                                ////
////                                                                   ////
////  This program must be loaded into a target chip using a device    ////
////  programmer.  Afterwards this program may be used to load new     ////
////  versions of the application program.  There is a specific        ////
////  protocol used by the bootloader, SIOW.EXE uses this protocol     ////
////  if you use the 'Download Software' option.                       ////
////                                                                   ////
////  Compiling this program gives you a HEX file of just the loader.  ////
////  To load the application you have two choices: a.) use the loader ////
////  to load the application, or b.) merge the application HEX and    ////
////  loader HEX into one production HEX.  The #import() CCS command   ////
////  will help you perform the latter.  Be aware that if you do the   ////
////  former the PIC will get it's configuration bit settings (fuses)  ////
////  from the loader and not the application (this bootloader does    ////
////  not change the configuration bits)                               ////
////                                                                   ////
////  To create an application that is compatible with this loader     ////
////  simply #include usb_bootloader.h into your application.  This    ////
////  loader does support interrupts in your application.              ////
////                                                                   ////
////  This bootloader is designed to detect pin A4 low on reset (this  ////
////  is the push-button on the CCS USB Development kit).  If A4 is    ////
////  low it starts the USB CDC stack and checks for bootload          ////
////  information over USB.  Otherwise the application program is      ////
////  started.                                                         ////
////                                                                   ////
////  If SEND_BOOTLOADER_IDLE_MESSAGE is #defined, then every 3s       ////
////  it will send a "Bootloader Ready" message over USB.  It will     ////
////  stop sending this once it receives a character over USB.         ////
////                                                                   ////
////  This is a port of ex_bootloader.c, which is a serial bootloader. ////
////  Since CDC makes it easy to port legacy applications, the         ////
////  previous example makes the bulk of this code.                    ////
////                                                                   ////
//// This file is part of CCS's PIC USB driver code.  See USB.H        ////
//// for more documentation and a list of examples.                    ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
//// VERSION HISTORY                                                   ////
////                                                                   ////
//// January 26th, 2017:                                               ////
////  Added support for PCM.                                           ////
////  Added SEND_BOOTLOADER_IDLE_MESSAGE option.                       ////
////                                                                   ////
//// July 9th, 2012:                                                   ////
////  Added support for PCD.                                           ////
////  No longer uses interrupts (polling).  This simplifies the        ////
////     interrupt support in your application, because they don't     ////
////     need to be shared.                                            ////
////                                                                   ////
//// Oct 15th, 2010:                                                   ////
////  Potential bug in CDC that causes lost PIC->PC chars resolved?    ////
////  Added initial 18F47J53 family support.                           ////
////  Added USB_ISR_POLLING support.  Define this and interrupts       ////
////     will not be used.  usb_task() must be called periodically     ////
////     in your main loop.  If it is not called faster than once      ////
////     per millisecond, USB may not work (PIC18 and PIC24 only).     ////
////                                                                   ////
//// April 28th, 2010:                                                 ////
////  Added ex_usb_common.h.                                           ////
////  Initial support for CCS PIC24USB board.                          ////
////                                                                   ////
//// March 5th, 2009:                                                  ////
////   Cleanup for Wizard.                                             ////
////   PIC24 Initial release.                                          ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2009 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS    ////
//// C compiler.  This source code may only be distributed to other    ////
//// licensed users of the CCS C compiler.  No other use,              ////
//// reproduction or distribution is permitted without written         ////
//// permission.  Derivative programs created using this software      ////
//// in object code form are not restricted in any way.                ////
///////////////////////////////////////////////////////////////////////////

// option - usually enabled.
// see documentation above.
#define SEND_BOOTLOADER_IDLE_MESSAGE

// option - usually not enabled.
// see documentation in usb_bootloader.h
//#define USB_BOOTLOADER_CONFIG_BITS_WRITABLE

///////////////////////

#include <ex_usb_common.h>

/*
 Configure, then load the bootloader definitions
*/

#define _bootloader
#include "usb_bootloader.h"

// Includes all USB code and interrupts, as well as the CDC API
#include <usb_cdc.h>

#if defined(USB_BOOTLOADER_CONFIG_BITS_WRITABLE)
#define ROM_WRITE_CAN_MODIFY_CONFIG_BITS
#endif

#include <rom_write.c>

#define BUFFER_LEN_LOD 64

#define ACKLOD 0x06
#define XON    0x11
#define XOFF   0x13

// Convert two hex characters to a int8
unsigned int8 atoi_b16(char *s) 
{  
   char c;
   unsigned int8 result = 0;
   int i;

   for (i=0; i<2; i++,s++)  
   {
      c = *s;
      if (c >= 'A')
         result = 16*result + c - 'A' + 10;
      else
         result = 16*result + c - '0';         
   }

   return(result);
}

#if defined(SEND_BOOTLOADER_IDLE_MESSAGE)
#include <tick.c>
#endif

void load_program(void)
{
   int1  do_ACKLOD, done=FALSE;
   unsigned int8  checksum, line_type;
   unsigned int16 l_addr,h_addr=0;
   unsigned int8 to;
   unsigned int32 addr;
   unsigned int8  dataidx, i, count;
   unsigned int8  data[32];
   unsigned int8  buffidx;
   char buffer[BUFFER_LEN_LOD];
  #if defined(SEND_BOOTLOADER_IDLE_MESSAGE)
   TICK t;
   int1 doSend;
   
   t = TickGet();
   doSend = 1;
  #endif
   
   while (!done)  // Loop until the entire program is downloaded
   {
      usb_task(); 
      
     #if defined(SEND_BOOTLOADER_IDLE_MESSAGE)
      if 
      (
         doSend && 
         TickIsExpired(t, TICKS_PER_SECOND * 3)
      )
      {
         t = TickGet();
         printf(usb_cdc_putc_fast, "BOOTLOADER READY\r\n");
      }
     #endif
      
      if (!usb_cdc_kbhit())
         continue;

     #if defined(SEND_BOOTLOADER_IDLE_MESSAGE)
      doSend = 0;
     #endif
     
      buffidx = 0;  // Read into the buffer until 0x0D ('\r') is received or the buffer is full
      to = 250;   //250 milliseconds

      do
      {
         if (!usb_cdc_kbhit())
         {
            delay_ms(1);
            usb_task();
            to--;
            if (!to)
               break;
         }
         else
         {
            to = 250;
            i = usb_cdc_getc();
            buffer[buffidx++] = i;
            if ((i == 0x0D) || (i == 0x0A))
               break;
         }
      } while (buffidx <= BUFFER_LEN_LOD);
     
      if (!to)
         continue;
      
      usb_cdc_putc(XOFF);  // Suspend sender
      usb_task();
      
      do_ACKLOD = TRUE;
      
      // Only process data blocks that start with ':'
      if (buffer[0] == ':') 
      {
         checksum = 0;  // Sum the bytes to find the check sum value
         
         for (i=1; i<(buffidx-3); i+=2)
            checksum += atoi_b16 (&buffer[i]);
         checksum = 0xFF - checksum + 1;
         
         if (checksum != atoi_b16 (&buffer[buffidx-3]))
            do_ACKLOD = FALSE;
         else
         {
            count = atoi_b16(&buffer[1]);  // Get the number of bytes from the buffer
         
            // Get the lower 16 bits of address
            l_addr = make16(atoi_b16(&buffer[3]),atoi_b16(&buffer[5]));
            
            line_type = atoi_b16 (&buffer[7]);
            
            addr = make32(h_addr,l_addr) / (__ADDRESS__)_LOADER_BPA;
            
            // If the line type is 1, then data is done being sent
            if (line_type == 1)
               done = TRUE;
            else if (line_type == 4)
               h_addr = make16(atoi_b16(&buffer[9]), atoi_b16(&buffer[11]));
            else if ((line_type == 0) && (addr >= (int32)APPLICATION_START))
            {
               // Loops through all of the data and stores it in data
               // The last 2 bytes are the check sum, hence buffidx-3
               for (i = 9,dataidx=0; i < buffidx-3; i += 2)
                  data[dataidx++]=atoi_b16(&buffer[i]);

               rom_modify(addr, data, count);
            }
         }
      }
      if(do_ACKLOD)
         usb_cdc_putc(ACKLOD);
      
      usb_cdc_putc(XON);
   }
   
   rom_flush();
   
   usb_cdc_putc(ACKLOD);
   usb_cdc_putc(XON);
   
   while(!usb_cdc_put_buffer_free())
   {
      usb_task();
   }
   
   to = 250;
   while(to)   //give time for packet to flush
   {
      delay_ms(1);
      usb_task();
      to--;
   }
   
   reset_cpu();
}

#if !defined(LED3) && defined(LED1)
#define LED3   LED1
#endif

void main(void)
{  
   HW_INIT();
   
   LEDS_OFF();

   //We use a button as an event to determine if we should start the USB CDC
   //bootloader.  If button is not pressed then go to the application, else if 
   //button is pressed then do the bootloader.
   if(BUTTON_PRESSED())
   {
      usb_init_cs();
      
      while(!usb_enumerated())
      {
         // necessary for polling function to work
        #ifdef USB_ISR_POLLING
         usb_task();
        #endif
      }
      
      #if defined(LED3)
      LED_ON(LED3);
      #endif
      
      load_program();
   }
   
   goto_address(APPLICATION_START);
}

#if defined(__PCD__)
#int_default
#else
#int_global
#endif
void isr(void)
{
   jump_to_isr(APPLICATION_ISR);
}

