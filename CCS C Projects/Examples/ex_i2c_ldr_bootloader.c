///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                    ex_i2c_ldr_bootloader.c                        ////
////                                                                   ////
//// I2C Loader, the bootloader.  This program sits at low memory and  ////
//// receives I2C commands for writing to the flash program memory     ////
//// of the PICmicro.                                                  ////
////                                                                   ////
//// For full documentation, see i2c_bootloader.h.                     ////
////                                                                   ////
//// Generally you do not need to modify this program, except to       ////
//// change hardware specific things above the 'end configuration'     ////
//// line.                                                             ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////          (C) Copyright 1996,2021 Custom Computer Services         ////
////                                                                   ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler or to users who purchased the rights to use this code.   ////
//// This source code may only be distributed to other licensed users  ////
//// of the CCS C compiler or other users who purchased the rights to  ////
//// this code.  The rights to use this code can only be granted       ////
//// by CCS.  No other use, reproduction or distribution is permitted  ////
//// without written permission.  Derivative programs created using    ////
//// this software in object code form are not restricted in any way.  ////
////                                                                   ////
////                       http://www.ccsinfo.com                      ////
///////////////////////////////////////////////////////////////////////////

// Uncomment the following define to send debug messages using either the ICD
// or a RS232 connection.  If this is uncommented then the same define in the
// ex_i2c_ldr_application.c example should also have the same define
// uncommented.
//#define DEBUG_BOOTLOADER

#ifdef DEBUG_BOOTLOADER
 // The following selects if the bootloader debug message are sent through the
 // ICD or with an RS232 connection.  If defined sent through the ICD, if not 
 // defined sent with the RS232 connection.
 #define USE_ICD_FOR_RS232

 #ifndef USE_ICD_FOR_RS232
  #define RS232_TX_PIN  PIN_C6   //set the RS232 TX pin
  #define RS232_RX_PIN  PIN_C7   //set the RS232 RX pin
  
  #define RS232_SERIAL_BAUD      38400
 #endif
#endif

#if defined(__PCM__)
 #include <16F1937.h>
 #use delay(crystal=20MHz)
 
 #define INT_I2C_BOOTLOADER INT_SSP
 #use i2c(SLAVE, I2C1, address=0xA0, STREAM=STREAM_I2C_BOOTLOADER) //SDA=PIN_C4, SCL=PIN_C3 
 
 #define BUTTON_PRESSED()        (!input(PIN_A4))
 #define IS_BOOTLOADER_FORCED()  BUTTON_PRESSED()
#elif defined(__PCH__)
 #include <18f4520.h>
 #use delay(crystal=20MHz)
 
 #define INT_I2C_BOOTLOADER   INT_SSP
 #use i2c(SLAVE, I2C1, address=0xA0, STREAM=STREAM_I2C_BOOTLOADER) //SDA=PIN_C4, SCL=PIN_C3 
 
 #define BUTTON_PRESSED()        (!input(PIN_A4))
 #define IS_BOOTLOADER_FORCED()  BUTTON_PRESSED()
#else
 #error include a valid configuration for your device
#endif

#ifdef DEBUG_BOOTLOADER
 #ifdef USE_ICD_FOR_RS232
  #use rs232(ICD, stream=DEBUG_STREAM)
 #else
  #use rs232(xmit=RS232_TX_PIN, rcv=RS232_RX_PIN, baud=RS232_SERIAL_BAUD)
 #endif
 
 #define debug_printf      printf
 
 void debug_putc(char c)
 {
   fputc(c, DEBUG_STREAM);
 }
 
 #if defined(__PCM__)
  #define LOADER_END    0x53F
 #elif defined(__PCH__)
  #define LOADER_END    0x7FF
 #endif
#endif

//////////// end configuration /////////////

#define __bootloader    //tell i2c_bootloader.h that this is the bootloader program

#include "i2c_bootloader.h"

#include "rom_write.c"

#ifndef debug_printf
   #define debug_printf(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
#endif

void goto_application(void)
{
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_I2C_BOOTLOADER);

  #asm
   GOTO PROGRAM_APP_START
  #endasm
}

#ifndef IS_BOOTLOADER_FORCED
#define IS_BOOTLOADER_FORCED()   FALSE
#endif

#int_global
void isr(void)
{
   jump_to_isr(PROGRAM_ISR_START);
}

int1 g_PointerIsRam;
char *g_RamPointer;
unsigned int32 g_RomPointer;
unsigned int16 g_Magic;
unsigned int8 g_LastCmd;

inline void i2c_slave_interrupt(void)
{
   unsigned int8 incoming, state;
   static union
   {
      unsigned int8 b[4];
      unsigned int16 w[2];
      unsigned int32 dw;
   } val;
   static struct
   {
      unsigned int8 b[2]; //max data bytes in hex file is 32
      unsigned int8 index;
   } data;      
   
   state = i2c_isr_state(STREAM_I2C_BOOTLOADER);
   
   if(state <= 0x80)                      //Master is sending data
   {
      incoming = i2c_read(STREAM_I2C_BOOTLOADER, 2);          //Passing 2 as parameter, causes the function to read the SSPBUF without releasing the clock
   
      if (state == 1)
      {  
         //First received byte is the command
         g_LastCmd = incoming;
      }
      else if
      (
         (g_LastCmd == I2C_LOADER_COMMAND_GOTO_APPLICATION) && 
         ((state==2) || (state==3))
      )
      {
         val.b[state-2] = incoming;
         if ((state == 3) && (val.w[0] == I2C_LOADER_MAGIC))
         {
            debug_printf(debug_putc, "W=GOTO_APPLICATION\r\n");
            rom_flush();
            
            i2c_write(STREAM_I2C_BOOTLOADER, 0);  //dummy write to release the clock lines.  this does not actually TX
            clear_interrupt(INT_I2C_BOOTLOADER);
            
            goto_application();
         }
      }
      else if
      (
         (g_LastCmd == I2C_LOADER_COMMAND_ROM_ADDRESS) && 
         ((state>=2) || (state<=5))
      )
      {
         val.b[state-2] = incoming;
         if (state == 5)
         {
            g_RomPointer = val.dw;
            data.index = 0;
            debug_printf(debug_putc, "W=ADDRESS %LX\r\n", g_RomPointer);
         }
      }
      else if (g_LastCmd == I2C_LOADER_COMMAND_ROM_ACCESS)
      {
         data.b[data.index] = incoming;
         
         if(++data.index >= 2)
            data.index = 0;
      
         if ((g_RomPointer >= PROGRAM_APP_START) && (g_RomPointer < getenv("PROGRAM_MEMORY")))
         {
            if(data.index == 0)
               rom_modify(g_RomPointer, data.b, 2);
         }
         
         if(data.index == 0)
         {
           #if defined(__PCM__)
            g_RomPointer++;
           #elif defined(__PCH__)
            g_RomPointer += 2;
           #endif
         }
      }
      
      if (state != 0x80)
      {
         i2c_write(STREAM_I2C_BOOTLOADER, 0);  //dummy write to release the clock lines.  this does not actually TX
      }
   }
   
   if(state >= 0x80)                      //Master is requesting data
   {
      if (state == 0x80)
      {
         g_PointerIsRam = TRUE;
         if (g_LastCmd == I2C_LOADER_COMMAND_ROM_ADDRESS)
         {
            debug_printf(debug_putc, "R=ROM_ADDRESS %LX\r\n", g_RomPointer);
            g_RamPointer = &g_RomPointer;
         }
         else if (g_LastCmd == I2C_LOADER_COMMAND_ROM_ACCESS)
         {
            debug_printf(debug_putc, "R=ROM_ACCESS %LX\r\n", g_RomPointer);
            g_PointerIsRam = FALSE;
            rom_flush();
         }
         else if (g_LastCmd == I2C_LOADER_COMMAND_GOTO_BOOTLOADER)
         {
            debug_printf(debug_putc, "R=GOTO_BOOTLOADER\r\n");
            g_RamPointer = &g_Magic;
         }
         
         i2c_write(STREAM_I2C_BOOTLOADER, g_LastCmd);
      }
      else
      {
         if (g_RamPointer)
         {
            i2c_write(STREAM_I2C_BOOTLOADER, *g_RamPointer++);
         }
         else
         {
            read_program_memory(g_RomPointer, &val.b[0], 1);
            g_RomPointer++;
            i2c_write(STREAM_I2C_BOOTLOADER, val.b[0]);
         }
      }
   }
   
   clear_interrupt(INT_I2C_BOOTLOADER);
}

void bootloader_loop(void)
{
   g_RomPointer = 0;
   g_Magic = I2C_LOADER_MAGIC;
   g_LastCmd = I2C_LOADER_COMMAND_GOTO_BOOTLOADER;
   
   rom_init();
   
   debug_printf(debug_putc, "BOOTLOADER_STARTED\r\n");
   
   for(;;)
   {
      if (interrupt_active(INT_I2C_BOOTLOADER))
      {
         i2c_slave_interrupt();
      }
   }
}

#org PROGRAM_BOOTLOADER_START, PROGRAM_BOOTLOADER_START+0xf
void proxy_bootloader_start(void)
{
   bootloader_loop();
}

void main(void)
{
   if (IS_BOOTLOADER_FORCED())
   {
      debug_printf(debug_putc, "BOOTLOADER_FORCED\r\n");

      bootloader_loop();
   }
   
   debug_printf(debug_putc, "NO_BOOTLOADER\r\n");

   goto_application();
}

