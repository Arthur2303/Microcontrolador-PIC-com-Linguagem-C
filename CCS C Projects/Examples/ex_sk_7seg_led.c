///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          ex_sk_7seg_led.c                         ////
////                                                                   ////
//// Example code showing how to use the 7 segment LED display in the  ////
//// sensor kit with the E3.  The run this example the display must be ////
//// wired as shown in the sensor kit exercise book.                   ////
////                                                                   ////
//// When this example is running the analog voltage read from the pot ////
//// will be displayed on the 7 segment LED display.  Turn the pot     ////
//// and the value will change.                                        ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#include <e3.h>

#define PIN_7SEG_A         PIN_B4
#define PIN_7SEG_B         PIN_B5
#define PIN_7SEG_C         PIN_B6
#define PIN_7SEG_D         PIN_B7
#define PIN_7SEG_E         PIN_C5
#define PIN_7SEG_F         PIN_C6
#define PIN_7SEG_G         PIN_C7
#define PIN_7SEG_COMMON1   PIN_C3
#define PIN_7SEG_COMMON2   PIN_C4

const char g_7SegCharBitmaps[] =
{
   0x3f, //0
   0x06, //1
   0x5b, //2
   0x4f, //3
   0x66, //4
   0x6d, //5
   0x7d, //6
   0x07, //7
   0x7f, //8
   0x6f  //9
};

void Led7SegOutputBitmap(int8 bitmap)
{
   output_bit(PIN_7SEG_A, bit_test(bitmap, 0));
   output_bit(PIN_7SEG_B, bit_test(bitmap, 1));
   output_bit(PIN_7SEG_C, bit_test(bitmap, 2));
   output_bit(PIN_7SEG_D, bit_test(bitmap, 3));
   output_bit(PIN_7SEG_E, bit_test(bitmap, 4));
   output_bit(PIN_7SEG_F, bit_test(bitmap, 5));
   output_bit(PIN_7SEG_G, bit_test(bitmap, 6));
}

// value can range from 0 to 99
void Led7SegDisplayNumber(int8 value)
{
   int8 tens;
   int8 ones;
   
   if (value > 99)
      value = 99;
   
   tens = value / 10;
   ones = value % 10;
   
   output_high(PIN_7SEG_COMMON1);
   output_high(PIN_7SEG_COMMON2);
   
   Led7SegOutputBitmap(g_7SegCharBitmaps[tens]);
   
   output_low(PIN_7SEG_COMMON1);
   
   delay_ms(2);
 
   output_high(PIN_7SEG_COMMON1);
   output_high(PIN_7SEG_COMMON2);
   
   Led7SegOutputBitmap(g_7SegCharBitmaps[ones]);
   
   output_low(PIN_7SEG_COMMON2);
   
   delay_ms(2);
}

void main(void)
{
   unsigned int8 adc;
   
   setup_adc_ports(sAN4);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(4);
   
   printf("\r\n\r\nex_sk_7seg_led - starting.\r\n");
   
   while(true)
   {
      adc = read_adc();
      
      Led7SegDisplayNumber(adc / 3);
   }
}

