///////////////////////////////////////////////////////////////////////////////
////                          EX_EMT_PERIOD_DUTY.c                         ////
////                                                                       ////
//// This program shows how to setup and use the Signal Measurement Timer  ////
//// (SMT) peripheral in Period and Duty Cycle Acquisition mode to capture ////
//// and display the period and duty cycle of an input signal.             ////
////                                                                       ////
//// Select either the ICD or your own RS232-to-PC connection for the      ////
//// text I/O.                                                             ////
////                                                                       ////
//// This example will work with the PCM and PCH compilers.  The following ////
//// conditional compilation lines are used to include a valid device for  ////
//// each compiler, change the device, clock, rs232 and pin assignments as ////
//// needed for your hardware configuration.                               ////
///////////////////////////////////////////////////////////////////////////////
////         (C) Copyright 1996,2016,2018 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS C      ////
//// compiler.  This source code may only be distributed to other licensed ////
//// users of the CCS C compiler.  No other use, reproduction or           ////
//// reproduction or distribution is permitted without written permission. ////
//// Derivative programs created using this software in object code form   ////
//// are not restricted in any way.                                        ////
///////////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F1618.h>
#elif defined(__PCH__)
#include <18F4520.h>
#endif

#define SMT_INPUT    PIN_B4

#use delay(internal=8MHz)

#pin_select U1TX=PIN_C6
#pin_select U1RX=PIN_C7
#use rs232(icd)                //Text through the ICD
//#use rs232(UART1, baud=9600) //Text through the UART

#pin_select SMT1SIG=SMT_INPUT

unsigned int32 SignalDuty;
unsigned int32 SignalPeriod;
int1 NewCapture = FALSE;

#INT_SMT1PRA
void period_isr(void)
{
   SignalPeriod = smt1_read(SMT_CAPTURED_PERIOD_REG); //read captured period
   NewCapture = TRUE;
}

#INT_SMT1PWA
void duty_isr(void)
{
   SignalDuty = smt1_read(SMT_CAPTURED_PULSE_WIDTH_REG); //read captured duty
}

char* CalculatePeriod(void)
{
   static char str[10];
   float period;
   int8 t = 0;
   int8 Count;
   
   period = (float)SignalPeriod * (1.0 / (float)getenv("CLOCK"));
   
   while(period < 1)
   {
      period *= 1000.0;
      t++;
   }
   
   Count = sprintf(str, "%0.2f ", period);
   
   switch(t)
   {
      case 0:
         sprintf(&str[Count], "s");
         break;
      case 1:
         sprintf(&str[Count], "ms");
         break;
      case 2:
         sprintf(&str[Count], "us");
         break;
      default:
         sprintf(&str[Count], "ns");
         break;
   }
   
   return(str);
}

float CalculateDuty(void)
{
   float duty;
   
   duty = ((float)SignalDuty * 100.0) / (float)SignalPeriod;
   
   return(duty);
}

void main(void)
{
   printf("\r\nex_smt_period_duty.c - %s\r\n\n", getenv("DEVICE"));
   
   //setup SMT1 for Period and Duty Cycle Acquisition Mode, Input signal on SMT1SIG pin
   setup_smt1(SMT_ENABLED | SMT_MODE_PERIOD_DUTY_CYCLE_ACQ | SMT_CLK_FOSC | SMT_SIG_INPUT_SMTSIGx); 
   
   enable_interrupts(INT_SMT1PRA);  //enable SMT1PRA interrupt to capture period
   enable_interrupts(INT_SMT1PWA);  //enable SMT1PWA interrupt to capture duty
   enable_interrupts(GLOBAL);
   
   smt1_start();  //start SMT1

   while(TRUE)
   {
      if(NewCapture)
      {
         printf("\r\nSignal Period: %s, Duty: %0.2f%%", CalculatePeriod(), CalculateDuty());
         
         NewCapture = FALSE;
         smt1_start();  //re-start SMT1
      }
   }
}
