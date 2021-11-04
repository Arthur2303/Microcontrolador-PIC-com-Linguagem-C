///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                     ex_sk_rotary_encoder_isr.c                    ////
////                                                                   ////
//// Example code showing to to use the rotary encoder to count up     ////
//// and down based on rotation. Example code developed to run on      ////
//// the CCS E3 Mini development kit. Sensor module should use male to ////
//// male and female to female wires: the GND pin of module connects   ////
//// into the GND pin of the E3 and the + pin of the module            ////
//// connects to the +5 pin.DT connects to B4 and CLK connects to B7.  ////
////                                                                   ////
//// This module detects position change in either the positive(right) ////
//// or negative(left) direction of the rotary encoder.                ////
////                                                                   ////
//// When the example code is running, numbers will be outputted to    ////
//// SIOW. The numbers will increment if the rotary encoder is moved   ////
//// to the right and decrement if moved to the left.                  ////
////                                                                   ////
//// This sensor module is sometimes called ky040.                     ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                                                                   ////
//// http://www.ccsinfo.com                                            ////
///////////////////////////////////////////////////////////////////////////
#include <e3.h>

// following defines are for reading an encoder on PIN_B4 and PIN_B7
#define ENCODER_PULLUP_INIT() output_float(PIN_B4); output_float(PIN_B7)
#define ENCODER_READ_PORT() (input_b() & 0x90)
#define ENCODER_GET_BIT1(v) (bit_test(v,4))
#define ENCODER_GET_BIT0(v) (bit_test(v,7))
#define ENCODER_START_IOC()   enable_interrupts(INT_RB4); enable_interrupts(INT_RB7)
#define ENCODER_CLEAR_IOC()   input_a();  input_b();  clear_interrupt(INT_RAB)

struct
{
   signed int8 quad;
   unsigned int8 lastPort;
} _g_Encoder;

#INT_RAB noclear
void EncoderScan(void)
{
   unsigned int8 encoderPort;
   
   encoderPort=ENCODER_READ_PORT();
   
   ENCODER_CLEAR_IOC();

   if (encoderPort == _g_Encoder.lastPort)
      return;

   if(ENCODER_GET_BIT0(encoderPort)==ENCODER_GET_BIT1(_g_Encoder.lastPort))
      _g_Encoder.quad-=1;
   else
      _g_Encoder.quad+=1;
   
   _g_Encoder.lastPort=encoderPort;
}

// initialize encoder scanning
void EncoderInit(void)
{
   memset(&_g_Encoder,0,sizeof(_g_Encoder));
   ENCODER_PULLUP_INIT();
   delay_ms(1);
   _g_Encoder.lastPort=ENCODER_READ_PORT();
   ENCODER_CLEAR_IOC();
   ENCODER_START_IOC();
}

// read and clear any pending encoder changes.
signed int8 EncoderGet(void)
{
   signed int8 ret = 0;
   
   disable_interrupts(GLOBAL);
   
   while (_g_Encoder.quad >= 4)
   {
      ret++;
      _g_Encoder.quad -= 4;
   }
   while (_g_Encoder.quad <= -4)
   {
      ret--;
      _g_Encoder.quad += 4;
   }
   
   enable_interrupts(GLOBAL);
   
   return(ret);
}

void main(void)
{
   signed int8 position = 0;
   signed int8 change;
   
   printf("\r\n\r\nex_sk_rotary_encoder.c - example starting\r\n\r\n");
   
   EncoderInit();
   
   enable_interrupts(GLOBAL);
   
   for(;;)
   {
      delay_ms(1000);
      
      //putc('.');
      
      change = EncoderGet();
      
      if (change)
      {
         position += change;
         printf("Position changed %d to new position %d\r\n", change, position);
      }
   }
}

