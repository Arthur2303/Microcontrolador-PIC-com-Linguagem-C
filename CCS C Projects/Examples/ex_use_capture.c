/////////////////////////////////////////////////////////////////////////
////                        EX_USE_CAPTURE.c                         ////
////                                                                 ////
////  This example shows how to use the #use capture() directive to  ////
////  setup the Input Capture peripheral for PCD or the CCP          ////
////  peripheral for PCM and PCH in capture mode.                    ////
////                                                                 ////
////  This example also has the option of generating a PWM signal    ////
////  using the Output Compare peripheral or CCP peripheral that     ////
////  #use capture can measure.  When the define GENERATE_PWM_SIGNAL ////
////  is used the example will generate a PWM that changes period    ////
////  every 5 seconds for a total of six different periods that can  ////
////  be measured.                                                   ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCM, PCH and PCD compilers.    ////
////  The following conditional compilation lines are used to        ////
////  include a valid device for each compiler.  Change the device,  ////
////  clock and RS232 pins for your hardware if needed.              ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2016,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
////                                                                 ////
//// http://www.ccsinfo.com                                          ////
/////////////////////////////////////////////////////////////////////////

#define GENERATE_PWM_SIGNAL

#if defined(__PCM__)
#include <16F887.h>
#elif defined(__PCH__)
#include <18F4520.h>
#elif defined(__PCD__)
#include <30F2010.h>
#endif

#use delay(crystal=20MHz)
 
#define RS232_XMIT     PIN_C6
#define RS232_RCV      PIN_C7
#define CAPTURE_PIN    PIN_C2
#define CAPTURE_TIMER  1
 
#ifdef GENERATE_PWM_SIGNAL
 #define PWM_PIN       PIN_C1
 #define PWM_TIMER     2
 #define __CCS_USE_TIMER_PERIPH 0
#endif 

#use rs232(icd)                                         //Text through the ICD
//#use rs232(xmit=RS232_XMIT, rcv=RS232_RCV, baud=9600) //Text through the UART
#use capture(input=CAPTURE_PIN,timer=CAPTURE_TIMER, CAPTURE_FALLING) 

#ifdef GENERATE_PWM_SIGNAL
 #use pwm(output=PWM_PIN, timer=PWM_TIMER, frequency=2kHz, duty=50)
 
 #define TICK_IS_16BITS 
 #include <tick.c>
  
 #define GetTickDifference(a,b) (a-b)
#endif

#if getenv("INSTRUCTION_CLOCK") >= 1000000
 #define PERIOD_DIVISOR    (getenv("INSTRUCTION_CLOCK") / 1000000)
#else
 #define PERIOD_DIVISOR    1
#endif

unsigned int16 FirstCapture, SecondCapture;
int1 NewCapture = FALSE;

#INT_CCP1

void Capture_Interrupt(void)
{
   static int1 First = TRUE;   
   unsigned int16 dummy;

   if(!NewCapture)
   {
      if(First)
         FirstCapture = get_capture_time();     //Read first capture event.
      else
      {
         SecondCapture = get_capture_time();   //Read second capture event.
         NewCapture = TRUE;
      }
      
      First++;
   }
   else
      dummy = get_capture_time();  
}

#define GetCaptureDifference(a,b)   (b-a)

#ifdef GENERATE_PWM_SIGNAL
//The following are values used to generate 6 different PWM signals for 
//the Capture to measure.  The periods are 500us, 333.3us, 250us, 125us,
//62.5us, 31.2us.
unsigned int32 PwmFrequency[6] = {2000, 3000, 4000, 8000, 16000, 32000};

#endif

void main()
{
  #ifdef GENERATE_PWM_SIGNAL
   TICK CurrentTick, PreviousTick;
  #endif
   unsigned int16 Time;
   unsigned int8 i = 0;

   printf("\n\rex_use_capture.c - %s\n\n\r", getenv("DEVICE"));
   
   //enable the Capture interrupt
   enable_interrupts(INT_CCP1);
   enable_interrupts(GLOBAL);
   
  #ifdef GENERATE_PWM_SIGNAL
   PreviousTick = CurrentTick = TickGet();
  #endif
   
   while (TRUE) 
   {
     #ifdef GENERATE_PWM_SIGNAL
      CurrentTick = TickGet();
      
      if(GetTickDifference(CurrentTick, PreviousTick) >= (5*TICKS_PER_SECOND))  //change PWM period every 5 seconds
      {
         if(++i >= 6)
            i = 0;
            
         pwm_set_frequency(PwmFrequency[i]);
         pwm_set_duty_percent(500);
         
         PreviousTick = CurrentTick;
      }
     #endif
   
      if(NewCapture)
      {
         Time = GetCaptureDifference(FirstCapture, SecondCapture);
         
         printf("\r\rPeriod: %8.1w us", ((unsigned int32)Time * 10) / PERIOD_DIVISOR);
         NewCapture = FALSE;
      }
   }
}
