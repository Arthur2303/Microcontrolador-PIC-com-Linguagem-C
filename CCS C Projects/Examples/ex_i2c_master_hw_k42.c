///////////////////////////////////////////////////////////////////////////
////                      EX_I2C_MASTER_HW_K42.c                       ////
////                                                                   ////
//// This program shows how to use the i2c_transfer() function when    ////
//// using the HW I2C peripheral in Master mode on a PIC18FxxK42       ////
//// device.                                                           ////
////                                                                   ////
//// This emulates communication with and I2C eeprom similar to        ////
//// Microchip's 24LC01B or 24LC02B.  This can be used in conjunction  ////
//// with ex_slave.c.                                                  ////
////                                                                   ////
//// Select either the ICD or your own RS232-to-PC connection          ////
//// for the text I/O.                                                 ////
////                                                                   ////
//// The new I2C peripheral that K42 devices have is very different    ////
//// compared to the SSP peripheral that other PIC18 devices have.     ////
//// One big difference is how to setup the clock, for these device    ////
//// several different clocks can be used as the clock source.  The    ////
//// following option has been added to #use i2c() to specify the I2C  ////
//// clock source: CLOCK_SOURCE=x, x being the clock source to use.    ////
//// The following source are supported by the peripheral:             ////
////                                                                   ////
////     FOSC/4 - the instruction clock, this is the default setting.  ////
////                                                                   ////
////     FOSC - the system clock.                                      ////
////                                                                   ////
////     HFINT or HFINTOSC - the high frequency internal oscillator.   ////
////                                                                   ////
////     MFINT or MFINTOSC - the 500 kHz internal oscillator.          ////
////                                                                   ////
////     REF or REFCLK - the reference clock.                          ////
////                                                                   ////
////     TMR0 or TIMER0 - Timer 0, must be setup to achieve desired    ////
////                      I2C clock rate.                              ////
////                                                                   ////
////     TMR2 or TIMER2 - Timer 2,  must be setup to achieve desired   ////
////                      I2C clock rate.                              ////
////                                                                   ////
////     TMR4 or TIMER4 - Timer 4,  must be setup to achieve desired   ////
////                      I2C clock rate.                              ////
////                                                                   ////
////     TMR6 or TIMER6 - Timer 6,  must be setup to achieve desired   ////
////                      I2C clock rate.                              ////
////                                                                   ////
////     SMT1 - Signal Measurement Timer,  must be setup to achieve    ////
////            desired I2C clock rate.                                ////
////                                                                   ////
//// Additionally the I2C peripheral divides the I2C clock source by   ////
//// either 4 or 5.  The divisor also controls how many times SCL is   ////
//// sampled high before driving it low, 4 - once, 5 - twice. The      ////
//// following options has been added to #use i2c() to specify the I2C ////
//// clock divisor: CLOCK_DIVISOR=x, x being 4 or 5.                   ////
////                                                                   ////
//// This example will work with the PCH compiler.  The following      ////
//// conditional compilation lines are used to include a valid device. ////
//// Change the device, clock, UART pins and I2C pins for your         ////
//// hardware if needed.                                               ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2018 Custom Computer Services           ////
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
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#include <18F46K42.h>
#use delay(internal=64MHz)

#pin_select U1TX=PIN_C6
#pin_select U1RX=PIN_C7
//#use rs232(icd)                  //Text through the ICD
#use rs232(UART1,baud=115200)    //Text through the UART

//These defines specify with PIC pins are used as the SDA and SCL pins, change
//to match your hardware
#define SCL_PIN     PIN_C3
#define SDA_PIN     PIN_C4

//The following assigns the specified SCL and SDA pins as the HW I2C pins, depending
//on if the pins being used can be assigned to the I2C1 or I2C2 peripherals the
//lines may need to be changed to the following:
//#pin_select SCL2OUT = SCL_PIN
//#pin_select SDA2OUT = SDA_PIN
#pin_select SCL1OUT = SCL_PIN
#pin_select SDA1OUT = SDA_PIN

//The following define sets up a couple example I2C clock sources, if a different clock
//source is desired then comment out following define and replace the #error line
//below with #define I2C_CLOCK_SOURCE defined to the desired clock source, see
//above comments for valid clock sources.
#define USE_I2C_CLOCK_SOURCE     2  //0 - FOSC/4, 1 - MFINTOSC, 2 - TIMER2, other unspecified

#if USE_I2C_CLOCK_SOURCE == 0
 #define I2C_CLOCK_SOURC      FOSC/4   //FOSC 8 MHz or lower
#elif USE_I2C_CLOCK_SOURCE == 1
 #define I2C_CLOCK_SOURCE     MFINT
#elif USE_I2C_CLOCK_SOURCE == 2
 #define I2C_CLOCK_SOURCE     TMR2
#else
 #error I2C Clock Source Not Specified, add I2C clock source here.
#endif

//The following define sets the I2C clock divisor, can be set to 4 or 5.
#define I2C_CLOCK_DIVISOR     4

#use i2c(MASTER, scl=SCL_PIN, sda=SDA_PIN, CLOCK_SOURCE=I2C_CLOCK_SOURCE, CLOCK_DIVISOR=I2C_CLOCK_DIVISOR)

//The following define sets the I2C slave address that will be used with this example.
#define I2C_SLAVE_ADDRESS     0xA0

//The following define sets the MAX buffer size used with this example, i.e. it
//controls the max number of bytes that can be written to or read from slave
//device.
#define MAX_BUFFER_SIZE       127   //If using with ex_slave.c the max size that can be used is 127, otherwise the max size is 254

#if MAX_BUFFER_SIZE > 254
 #undef MAX_BUFFER_SIZE
 #define MAX_BUFFER_SIZE      254
#endif

//The following define controls when reading data from slave if read function
//does a (Start Write Restart Read Stop) or (Start Write Stop Start Read Stop).
//Comment following define out to do (Start Write Stop Start Read Stop).
#define I2C_READ_DO_RESTART

//The following is used to setup Timer 2, when USE_I2C_CLOCK_SOURCE is defined
//as 2, so that it's setup to correctly generate the I2C clock for the specified
//system clock, I2C_CLOCK_DIVISOR, and I2C_BAUD_RATE.  If not specified the 
//I2C_BAUD_RATE is set for 400kHz.
#if USE_I2C_CLOCK_SOURCE == 2
 #ifndef I2C_BAUD_RATE
  #define I2C_BAUD_RATE    400000
 #endif
 
 #if (((getenv("CLOCK") / 4) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) <= 256
  #define TIMER_PERIOD     ((((getenv("CLOCK") / 4) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) - 1)  
  #if TIMER_PERIOD < 1
   #undef TIMER_PERIOD
   #define TIMER_PERIOD    1
  #endif  
  #define TIMER_SETTINGS   (T2_DIV_BY_1 | T2_CLK_INTERNAL)
 #elif ((((getenv("CLOCK") / 4) / 2) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) <= 256
  #define TIMER_PERIOD     (((((getenv("CLOCK") / 4) / 2) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) - 1)
  #define TIMER_SETTINGS   (T2_DIV_BY_2 | T2_CLK_INTERNAL)
 #elif ((((getenv("CLOCK") / 4) / 4) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) <= 256
  #define TIMER_PERIOD     (((((getenv("CLOCK") / 4) / 4) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) - 1)
  #define TIMER_SETTINGS   (T2_DIV_BY_4 | T2_CLK_INTERNAL)
 #elif ((((getenv("CLOCK") / 4) / 8) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) <= 256
  #define TIMER_PERIOD     (((((getenv("CLOCK") / 4) / 8) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) - 1)
  #define TIMER_SETTINGS   (T2_DIV_BY_8 | T2_CLK_INTERNAL)
 #elif ((((getenv("CLOCK") / 4) / 16) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) <= 256
  #define TIMER_PERIOD     (((((getenv("CLOCK") / 4) / 16) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) - 1)
  #define TIMER_SETTINGS   (T2_DIV_BY_16 | T2_CLK_INTERNAL)
 #elif ((((getenv("CLOCK") / 4) / 32) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) <= 256
  #define TIMER_PERIOD     (((((getenv("CLOCK") / 4) / 32) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) - 1)
  #define TIMER_SETTINGS   (T2_DIV_BY_32 | T2_CLK_INTERNAL)
 #elif ((((getenv("CLOCK") / 4) / 64) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) <= 256
  #define TIMER_PERIOD     (((((getenv("CLOCK") / 4) / 64) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) - 1)
  #define TIMER_SETTINGS   (T2_DIV_BY_64 | T2_CLK_INTERNAL)
 #else
  #if ((((getenv("CLOCK") / 4) / 128) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) <= 256
   #define TIMER_PERIOD     (((((getenv("CLOCK") / 4) / 128) / I2C_BAUD_RATE) / I2C_CLOCK_DIVISOR) - 1)
  #else
   #define TIMER_PERIOD      255
  #endif
  #define TIMER_SETTINGS   (T2_DIV_BY_128 | T2_CLK_INTERNAL)
 #endif
#endif

typedef unsigned int8 EEPROM_ADDRESS;

int1 ext_eeprom_ready(void);
int1 write_ext_eeprom_bytes(EEPROM_ADDRESS address, unsigned int8 *data, unsigned int8 count);
int1 read_ext_eeprom_bytes(EEPROM_ADDRESS address, unsigned int8 *data, unsigned int8 count);

#include <stdlib.h>
#include <input.c>

void PrintData(unsigned int8 *Data, unsigned int8 count)
{
   unsigned int8 i;
   
   printf("\r\nData:");
   
   for(i=0;i<count;i++)
   {
      if((i % 8) == 0)
         printf("\r\n");
      
      printf("%X ", Data[i]);
   }
   
   printf("\r\n");
}

void main(void)
{
   unsigned int8 rData[MAX_BUFFER_SIZE];
   unsigned int8 wData[MAX_BUFFER_SIZE];
   unsigned int8 cmd;
   unsigned int8 count;
   unsigned int8 address;
   unsigned int8 value;
   unsigned int8 inc_value;
   unsigned int8 i;
   int1 ack;
      
   delay_ms(1000);
   
   printf("\r\nEX_I2C_MASTER_HW_K42.c - %s\r\n", getenv("DEVICE"));
   
  #if USE_I2C_CLOCK_SOURCE == 2 //Timer 2, setup timer 2 to generate correct I2C clock
   setup_timer_2(TIMER_SETTINGS, TIMER_PERIOD, 1);
  #endif
   
   do {
      do {
         printf("\r\nRead or Write: ");
         cmd = toupper(getc());
         putc(cmd);
      } while((cmd != 'R') && (cmd != 'W'));
      
      printf("\r\nAddress: ");
      address = gethex();
      
      printf("\r\nCount (1-%u): ", MAX_BUFFER_SIZE);
      count = get_Int8();
      
      if((count > 0) && (count <= MAX_BUFFER_SIZE))
      {
         if(cmd == 'R')
         {
            ack = read_ext_eeprom_bytes(address, rData, count);
            
            if(ack == 0)
               PrintData(rData, count);
            else
               printf("\r\nNACK\r\n");
         }
         else
         {
            if(count == 1)
               printf("\r\nValue (HEX): ");
            else
               printf("\r\nStart Value (HEX): ");
               
            value = gethex();
            
            if(count > 1)
            {
               printf("\r\nInc Value (HEX): ");
               
               inc_value = gethex();
               
               for(i=0;i<count;i++)
                  wData[i] = value + (i * inc_value);
            }
            
            write_ext_eeprom_bytes(address, wData, count);
            
            printf("\r\n");
         }
      }
   } while(TRUE);
}

///////////////////////////////////////////////////////////////////////////////

#ifndef NULL
 #define NULL     0
#endif

int1 ext_eeprom_ready(void)
{
   int1 ack;
   
   ack = i2c_transfer(I2C_SLAVE_ADDRESS, NULL, 0);
   
   return (!ack);
}

int1 write_ext_eeprom_bytes(EEPROM_ADDRESS address, unsigned int8 *data, unsigned int8 count)
{
   int1 ack;
   unsigned int8 buffer[MAX_BUFFER_SIZE + 1];
   
   if((count > 0) && (count < (MAX_BUFFER_SIZE + 1)))
   {
      while(!ext_eeprom_ready());
      
      buffer[0] = make8(address, 0);
      memcpy(&buffer[1], data, count);
      
      ack = i2c_transfer(I2C_SLAVE_ADDRESS, buffer, count+1);
   }
   else
      ack = 1;
   
   return(ack);
}

int1 read_ext_eeprom_bytes(EEPROM_ADDRESS address, unsigned int8 *data, unsigned int8 count)
{
   int1 ack;
   unsigned int8 buffer[1];
   
   if(count > 0)
   {
      while(!ext_eeprom_ready());
      
      buffer[0] = make8(address, 0);
      
     #ifdef I2C_READ_DO_RESTART
      ack = i2c_transfer(I2C_SLAVE_ADDRESS, buffer, 1, data, count);  
     #else
      ack = i2c_transfer_out(I2C_SLAVE_ADDRESS, buffer, 1);
         
      if(ack == 0)
         ack = i2c_transfer_in(I2C_SLAVE_ADDRESS, data, count);
     #endif
   }
   else
       ack = 1;
   
   return(ack);
}
