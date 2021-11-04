///////////////////////////////////////////////////////////////////////////
////                       EX_SLAVE_K42.C                              ////
////                                                                   ////
////  This program uses the PIC in I2C slave mode to emulate the       ////
////  24LC01 EEPROM. You can write to addresses 00h to FFh with it.    ////
////                                                                   ////
////  This program is to be used in conjunction with the ex_extee.c    ////
////  example, using the "#include <2402.C>" or "#include <2401.c>",   ////
////  or ex_i2c_master_hw_k42.c.  Only 256 bytes of address space are  ////
////  implemented.                                                     ////
////                                                                   ////
//// This example will work with the PCH compiler.  The following      ////
//// conditional compilation lines are used to include a valid device. ////
//// Change the device, clock, and I2C pins for your hardware if       ////
//// needed.                                                           ////
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
#use delay(internal=8MHz)

#define SCL_PIN     PIN_C3
#define SDA_PIN     PIN_C4

#pin_select SCL1OUT = SCL_PIN
#pin_select SDA1OUT = SDA_PIN

#use I2C(SLAVE, scl=SCL_PIN, sda=SDA_PIN, address=0xA0)

unsigned int8 address, buffer[256];

#INT_I2C1 NOCLEAR
void i2c_interrupt(void)
{
   unsigned int8 state, incoming;

   state = i2c_isr_state();

   if(state <= 0x80)                      //Master is sending data
   {
      if(state == 0x80)
         incoming = i2c_read(2);          //Passing 2 as parameter, causes the function to read the SSPBUF without releasing the clock
      else
         incoming = i2c_read();

      if(state == 1)                      //First received byte is address
         address = incoming;
      else if(state >= 2 && state != 0x80)   //Received byte is data
         buffer[address++] = incoming;
   }

   if(state >= 0x80)                      //Master is requesting data
   {
      i2c_write(buffer[address++]);
   }
}

void main(void)
{
   unsigned int16 i;
   
   for(i=0;i<256;i++)
      buffer[i] = i;
   
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_I2C1);
   
   while(TRUE)
   {
   }
}
