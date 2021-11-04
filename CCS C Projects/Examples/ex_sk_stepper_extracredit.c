///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                   ex_sk_stepper_extracredit.c                     ////
////                                                                   ////
//// This is similar to ex_sk_stepper.c, but pressing and releasing    ////
//// the button causes it to move one rotation (2048 steps).  This     ////
//// demonstrates how accurate movement of a stepper is.               ////
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

#define PIN_IN1 PIN_B4  //motor A, positive
#define PIN_IN2 PIN_B5  //motor B, positive
#define PIN_IN3 PIN_B6  //motor A, negative
#define PIN_IN4 PIN_B7  //motor B, negative

#define BUTTON1_PRESSED()  (!input(BUTTON_1))
#define BUTTON2_PRESSED()  (!input(BUTTON_2))

#define PIN_STEPPER_A_PLUS  PIN_IN1
#define PIN_STEPPER_A_MINUS PIN_IN3
#define PIN_STEPPER_B_PLUS  PIN_IN2
#define PIN_STEPPER_B_MINUS PIN_IN4

//#define STEPPER_ONE_PHASE
#define STEPPER_TWO_PHASE
//#define STEPPER_HALF_STEPS

#include <stepper.c>

void main(void)
{
   unsigned int8 adc;
   
   setup_adc_ports(sAN4);
   setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_2);
   set_adc_channel(4);

   StepperInit();
   
   for(;;)
   {
      adc = read_adc();
      adc /= 16;
      StepperSetSpeed((TICKS_PER_SECOND*(TICK)adc)/1000);
      
      StepperTask();
      
      if (BUTTON1_PRESSED())
      {
         while(BUTTON1_PRESSED());
         delay_ms(144);
         
         StepperMove(+2048);
      }
      else if (BUTTON2_PRESSED())
      {
         while(BUTTON2_PRESSED());
         delay_ms(144);
         
         StepperMove(-2048);
      }
   }
}
