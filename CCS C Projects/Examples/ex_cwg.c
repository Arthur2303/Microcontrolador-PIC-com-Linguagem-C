///////////////////////////////////////////////////////////////////////////////
////                                ex_cwg.c                               ////
////                                                                       ////
////  This example program show how to use the built-in Complementary      ////
////  Waveform Generator (CWG) peripheral which can be used to produce     ////
////  half-bridge, full-bridge, push-pull and steering of PWM waveforms.   ////
////                                                                       ////
////  This example will use the CWG peripheral to produce a half-bridge    ////
////  PWM waveform using the PWM3 peripheral as the PWM signal source, the ////
////  ADC peripheral to set the PWM signals duty cycle.                    ////
////                                                                       ////
////  This example will work with the PCM and PCH compilers.  The          ////
////  following conditional compilation lines are used to include a valid  ////
////  device for the PCH compiler.  Change the device, clock, PWM pin and  ////
////  CWG pins for your hardware if needed.                                ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////           (C) Copyright 1996,2020 Custom Computer Services            ////
////  This source code may only be used by licensed users of the CCS C     ////
////  compiler.  This source code may only be distributed to other         ////
////  licensed users of the CCS C compiler.  No other use, reproduction or ////
////  distribution is permitted without written permission.  Derivative    ////
////  programs created using this software in object code form are not     ////
////  restricted in any way.                                               ////
////                        http://www.ccsinfo.com                         ////
///////////////////////////////////////////////////////////////////////////////

#include <18F24K40.h>
#device ADC=10
#fuses NOWDT
#use delay(oscillator=20MHz)

//Assign a pin to output the PWM signal on, this assignment doesn't need
//to be made.
#pin_select PWM3=PIN_C0

//Assign the pins to output the CWG half-bridge signal on.
#pin_select CWG1OUTA=PIN_C1
#pin_select CWG1OUTB=PIN_C2

//Setup a tick timer to use for reading ADC at a periodic rate.
#define __CCS_USE_TIMER_PERIPH   1
#include <tick.c>
#define TickDifference(a,b)      (a-b)

//Function for calculated the PWM Signal's Duty from ADC Reading.
unsigned int16 GetPWMDuty(void)
{
   unsigned int16 ADCReading;
   unsigned int16 Result;
   
   ADCReading = read_adc();
   
   Result = ((unsigned int32)ADCReading * 1000) / 1023;
   
   return(Result);
}

void main(void)
{
   TICK cTick, ADCTick;
   unsigned int16 Duty, pDuty;
   
   //Setup the ADC
   setup_adc_ports(sAN0, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);
   
   delay_ms(10);  //delay for initial ADC sampling time
   
   //Get the initial PWM duty
   Duty = GetPWMDuty();
   pDuty = Duty;
   
   //Setup Timer 2 to produce a 10kHz PWM signal from a 20MHz system clock
   setup_timer_2(T2_CLK_INTERNAL | T2_DIV_BY_2, 249, 1); //100us period, 1/(20MHz/4/2) * (249+1) = 100us
   
   //Setup the PWM3 peripheral
   set_pwm3_duty(Duty);
   setup_pwm3(PWM_ENABLED | PWM_TIMER2);
   
   //Setup the CWG peripheral in half-bridge mode using FOSC as the CWG clock
   //source, with PWM3 as the PWM signal source, no auto-shutdown, and a rising
   //and following dead-time of 60 CWG clock periods.
   setup_cwg(CWG_ENABLED | CWG_MODE_HALF_BRIDGE | CWG_CLOCK_FOSC | CWG_INPUT_PWM3, CWG_NO_AUTO_SHUTDOWN, 60, 60);
   
   ADCTick = TickGet();

   while(TRUE)
   {
      cTick = TickGet();
      
      //Read the ADC every 10ms and set the PWM signal's duty
      if(TickDifference(cTick, ADCTick) >= (TICKS_PER_SECOND / 100))
      {
         Duty = GetPWMDuty();
         
         if(Duty != pDuty)
         {
            //Set PWM signal's duty
            set_pwm3_duty(Duty);
            
            pDuty = Duty;
         }
         
         ADCTick = cTick;
      }
   }
}
