///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                         ex_sk_rgb_led.c                           ////
////                                                                   ////
//// Example code showing how to use a PWM to control the brightness   ////
//// of an LED.  This is shown by controlling the individual           ////
//// brightness of an RGB led.  Each color (R, G and B) is tied to     ////
//// a separate PWM.                                                   ////
////                                                                   ////
//// Example code developed to run on the CCS E3 Mini development kit. ////
//// Use four male to male and female to female wires to hook up the   ////
//// module. Connect the - pin to G, the B pin to B7, the G pin        ////
//// to B6, and the R pin to B4.                                       ////
////                                                                   ////
//// Initially the RGB led will be set for max brightness, and then    ////
//// using the pot will adjust the R level.  Pressing the button       ////
//// will then cause the pot to adjust the G level.  Pressing          ////
//// the button again will cause the pot to adjust the B level.        ////
//// Pressing the button again will cause the pot to go back to        ////
//// adjust the R level.                                               ////
////                                                                   ////
//// This sensor module is sometimes called ky016.                     ////
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

#define BUTTON_PRESSED()   (!input(BUTTON_1))

#define BLUE  PIN_B7
#define GREEN PIN_B6
#define RED   PIN_B4

#use PWM(OUTPUT = BLUE, TIMER = 0, frequency=6000, BITS = 8, PWM_ON, STREAM = B)
#use PWM(OUTPUT = GREEN, TIMER = 1, frequency=6000, BITS = 8, PWM_ON, STREAM = G)
#use PWM(OUTPUT = RED, TIMER = 3, frequency=6000, BITS = 8, PWM_ON, STREAM = R)

typedef enum
{
   USE_R = 0,
   USE_G = 1,
   USE_B = 2,
   USE_MAX
} which_led_t;

void set_rgb_duty(which_led_t which, unsigned int8 duty)
{
   unsigned int32 percent;
   
   // convert 8bit ADC value (0-255) to percent (0-1000)
   percent = duty;
   percent *= (unsigned int16) 1000;
   percent /= 255;
   
   switch(which)
   {
      case USE_R: pwm_set_duty_percent(R, percent);  break;
      case USE_G: pwm_set_duty_percent(G, percent);  break;
      case USE_B: pwm_set_duty_percent(B, percent);  break;
   }
}

void main(void)
{
   unsigned int8 adc;
   which_led_t which = USE_R;
   
   printf("\r\n\r\nex_sk_rgb_led.c - example starting\r\n\r\n");
   
   enable_interrupts(GLOBAL);
   setup_adc_ports(sAN4);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(4);
   
   only_led_on(RED_LED);
   
   set_rgb_duty(USE_R, 0);
   set_rgb_duty(USE_G, 0);
   set_rgb_duty(USE_B, 0);
   
   while(true)
   {
      adc = read_adc();
      
      set_rgb_duty(which, adc);
      
      if (BUTTON_PRESSED())
      {
         while(BUTTON_PRESSED());
         
         delay_ms(144);
         
         if (++which >= USE_MAX)
            which = 0;
         
         if (which == USE_R)
            only_led_on(RED_LED);
         else if (which == USE_G)
            only_led_on(GREEN_LED);
         else
            only_led_on(YELLOW_LED);
      }
   }
}
   


