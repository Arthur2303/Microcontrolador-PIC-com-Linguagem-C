///////////////////////////////////////////////////////////////////////////
////                              ex_cog.c                             ////
////                                                                   ////
////  An example showing how to setup the Complementary Output         ////
////  Generator (COG) module.  This example use Timer 2 and CCP1 to    ////
////  generate a 10kHz PWM signal that is used as the input to the COG ////
////  module.  The COG module is setup to generate a half-bridge PWM   ////
////  signal from that input.  Additionally it's setup to use the      ////
////  COGIN pin to shutdown the COG and is set to produce a falling    ////
////  dead-band of 10 instruction and a rising dead-band of 15         ////
////  instructions.                                                    ////
////                                                                   ////
////  This example will work with the PCM compiler on devices that     ////
////  have a COG module.  Change your device, #use delay(), ADC, and   ////
////  COG pin assignments as need for your hardware.                   ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////         (C) Copyright 1996, 2020 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                                                                   ////
////                       http://www.ccsinfo.com                      ////
///////////////////////////////////////////////////////////////////////////

#include <16F1717.h>
#fuses NOWDT
#device ADC=10
#use delay(internal=8MHz)

#include <tick.c>

#define PWM_PERIOD_VALUE      200       //10kHz period

//Assign COGA, COGB and COGIN pins
#pin_select COGA=PIN_B0
#pin_select COGB=PIN_B1
#pin_select COGIN = PIN_D0

/*
   SetPWMDuty()
   
   Set the PWM Duty for CCP1 from a percentage.
   
   Parameters:
      Percent - value from 0 - 100 to set the PWM duty to.
*/
void SetPWMDuty(unsigned int8 Percent)
{
   if(Percent > 100)
      Percent = 100;

   set_pwm1_duty(((unsigned int32)Percent * (PWM_PERIOD_VALUE * 4)) / 100);
}

/*
   SetPwmDutyFromADC()
   
   Set the PWM Duty from ADC reading, this function assumes ADC is already set
   for correct channel.
*/
void SetPWMDutyFromADC(void)
{
   unsigned int16 Reading;
   unsigned int8 Duty;
   
   Reading = read_adc();
   Duty = ((unsigned int32)Reading * 100) / 1023;
   
   SetPWMDuty(Duty);
}

void main(void)
{
   TICK ADCTick;
   
   //Setup ADC to read channel sAN0.
   setup_adc_ports(sAN0, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);
   
   delay_ms(100);
   
   //Setup timer 2 so CCP1 will generate a 10kHz PWM.
   setup_timer_2(T2_DIV_BY_1, (PWM_PERIOD_VALUE - 1), 1);
   
   //Set PWM duty based on ADC reading from channel sAN0.
   SetPWMDutyFromADC();
   //Setup CCP1 to produce a 10kHz PWM signal.
   setup_ccp1(CCP_PWM | CCP_TIMER2);
   
   //Enable COG for Half-Bridge PWM mode, Falling and Rising source is CCP1.
   //Enable Auto Shutdown on COGIN pin with auto restart.
   setup_cog(COG_ENABLED | COG_HALF_BRIDGE | COG_FALLING_SOURCE_CCP1 | COG_RISING_SOURCE_CCP1, COG_SHUTDOWN_ON_COGIN | COG_AUTO_RESTART, 0);
   
   //Set COG falling dead_band to 10 clock cycles, and rising dead_band to 15
   //clock cycles.
   set_cog_dead_band(10, 15);
   
   ADCTick = TickGet();

   while(TRUE)
   {
      if(TickIsExpired(ADCTick, (TICKS_PER_SECOND / 10)))   //every 100ms
      {
         //Update PWM duty from ADC.
         SetPWMDutyFromADC();
         
         ADCTick = TickGet();
      }
   }
}
