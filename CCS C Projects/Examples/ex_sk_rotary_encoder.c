///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                       ex_sk_rotary_encoder.c                      ////
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
#define ENCODER_DEBOUNCES_NEEDED 6

struct
{
   signed int8 quad;
   unsigned int8 newPort;
   unsigned int8 lastPort;
   unsigned int8 debounces;
} _g_Encoder;

// call this in a main loop.  this reads any changes in the decoder.
void EncoderScan(void)
{
   unsigned int8 encoderPort;
   
   encoderPort=ENCODER_READ_PORT();
   if(encoderPort!=_g_Encoder.newPort)
   {
      _g_Encoder.debounces=0;
   }
   else if(++_g_Encoder.debounces>=ENCODER_DEBOUNCES_NEEDED)//debounce,must hold for this long
   {
      if(encoderPort!=_g_Encoder.lastPort)
      {
         if(ENCODER_GET_BIT0(encoderPort)==ENCODER_GET_BIT1(_g_Encoder.lastPort))
            _g_Encoder.quad-=1;
         else
            _g_Encoder.quad+=1;
         
         _g_Encoder.lastPort=encoderPort;
      }
   }
   
   _g_Encoder.newPort=encoderPort;
}

// initialize encoder scanning
void EncoderInit(void)
{
   memset(&_g_Encoder,0,sizeof(_g_Encoder));
   ENCODER_PULLUP_INIT();
   delay_ms(1);
   _g_Encoder.lastPort=ENCODER_READ_PORT();
   _g_Encoder.newPort=_g_Encoder.lastPort;
}

// read and clear any pending encoder changes.
signed int8 EncoderGet(void)
{
   signed int8 ret = 0;
   
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
   
   return(ret);
}

void main(void)
{
   signed int8 position = 0;
   signed int8 change;
   
   printf("\r\n\r\nex_sk_rotary_encoder.c - example starting\r\n\r\n");
   
   EncoderInit();
   
   for(;;)
   {
      delay_us(10);
      
      EncoderScan();
      
      change = EncoderGet();
      
      if (change)
      {
         position += change;
         printf("Position changed %d to new position %d\r\n", change, position);
      }
   }
}

