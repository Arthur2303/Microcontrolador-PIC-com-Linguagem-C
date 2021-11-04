///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                   ex_sk_7seg_led_extracredit.c                    ////
////                                                                   ////
//// This example is similar to ex_sk_7seg_led.c, except:              ////
////  - An timer interrupt is used to toggle between the two segments. ////
////  - Values are displayed in hexadecimal instead of base 10         ////
////    decimal.                                                       ////
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
   0x6f, //9
   0x77, //a
   0x7c, //b
   0x39, //c
   0x5e, //d
   0x79, //e
   0x71  //f
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

unsigned int8 g_Led7SegDisplay;

#int_timer2
void IsrTimer2(void)
{
   static int1 toggle;
   
   if (toggle)
   {
      output_high(PIN_7SEG_COMMON1);
      output_high(PIN_7SEG_COMMON2);
      
      Led7SegOutputBitmap(g_7SegCharBitmaps[g_Led7SegDisplay / 16]);
      
      output_low(PIN_7SEG_COMMON1);
   }
   else
   {
      output_high(PIN_7SEG_COMMON1);
      output_high(PIN_7SEG_COMMON2);
      
      Led7SegOutputBitmap(g_7SegCharBitmaps[g_Led7SegDisplay % 16]);
      
      output_low(PIN_7SEG_COMMON2);
   }
   
   toggle = !toggle;
}

// value can be 0x00 to 0xFF
void Led7SegDisplayHex(int8 value)
{
   disable_interrupts(GLOBAL);
   
   g_Led7SegDisplay = value;
   
   enable_interrupts(GLOBAL);
}

void main(void)
{
   unsigned int8 adc;

   printf("\r\n\r\nex_sk_7seg_led_extracredit - starting.\r\n");
   
   setup_timer_2(T2_DIV_BY_16, 255, 15);
   enable_interrupts(INT_TIMER2);
   enable_interrupts(GLOBAL);
   
   setup_adc_ports(sAN4);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(4);
   
   while(true)
   {
      adc = read_adc();
      
      Led7SegDisplayHex(adc);
   }
}

