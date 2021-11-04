////////////////////////////////////////////////////////////////////////////
////                          ex_adc2_trigger.c                         ////
////                                                                    ////
////  This example demonstrates setting up the ADC2 peripheral in burst ////
////  average operation mode using the peripheral's built-in trigger to ////
////  start the conversion.                                             ////
////                                                                    ////
////  The ADC2 peripheral is setup for burst average operation mode     ////
////  running from the internal ADC clock, a sample time of 255 Tad and ////
////  to perform ten conversions and return the average of those        ////
////  conversions.  Additionally the ADC is setup to generate the ADT   ////
////  interrupt when the calculation is completed.                      ////
////                                                                    ////
////  The ADC2 peripheral's trigger source is set for the Comparator 1  ////
////  peripheral, so the ADC conversion will be triggered when the      ////
////  Comparator 1 would interrupt.  When the ADT interrupt occurs it   ////
////  will print the new average ADC reading using the serial           ////
////  connection.  Pressing the push button will cause the last average ////
////  ADC reading to be printed.                                        ////
////                                                                    ////
////  This example with work for the PCM and PCH compilers for devices  ////
////  with and ADC2 peripheral.  The following conditional compilation  ////
////  lines are used to include a valid device for the PCH compiler.    ////
////  Change the device, clock, UART pins, ADC pin, push button and     ////
////  LED pins as need for your hardware.                               ////
////                                                                    ////
////////////////////////////////////////////////////////////////////////////
////          (C) Copyright 1996,2020 Custom Computer Services          ////
////  This source code may only be used by licensed users of the CCS C  ////
////  compiler.  This source code may only be distributed to other      ////
////  licensed users of the CCS C compiler.  No other use, reproduction ////
////  or distribution is permitted without written permission.          ////
////  Derivative programs created using this software in object code    ////
////  form are not restricted in any way.                               ////
////                       http://www.ccsinfo.com                       ////
////////////////////////////////////////////////////////////////////////////

#include <18F47K42.h>
#device ADC=12
#fuses NOWDT, NOBROWNOUT
#use delay(internal=16MHz)

#pin_select U1TX=PIN_C6
#pin_select U1RX=PIN_C7
#use rs232(UART1, baud=115200)

#define LED          PIN_B0
#define PUSH_BUTTON  PIN_B1

#pin_select C1OUT=PIN_D0

unsigned int16 ADCReading = 0;
int1 ADCNewReading = FALSE;

#INT_ADT
void adt_isr(void)
{
   ADCReading = read_adc(ADC_READ_ONLY);
   
   ADCNewReading = TRUE;
}

#define __CCS_USE_TIMER_PERIPH   1
#include <tick.c>
#define TickDifference(a,b)      (a-b)

void main(void)
{
   TICK cTick, pTick;
   int1 cLevel, pLevel;
   
   printf("\r\nex_adc2_trigger.c - %s\r\n", getenv("DEVICE"));
   
   output_low(LED);
   
   //Setup comparator to interrupt when voltage on PIN_A2 goes above FVR which
   //is setup for ~2.048 V.
   setup_vref(VREF_COMP_DAC_2v048);
   setup_comparator(CP1_FVR_A2 | CP1_INT_L2H);

   setup_adc_ports(sAN0, VSS_VDD);
   
   //Set the ADC Trigger source to be the Comparator 1 interrupt.
   set_adc_trigger(ADC_TRIGGER_COMP1);
   
   //Setup ADC for burst average operation mode, to perform 10 conversions, and
   //to generate an ADT interrupt when the calculation is completed.
   setup_adc(ADC_BURST_AVERAGE_MODE | ADC_CLOCK_INTERNAL | ADC_TAD_MUL_255 | ADC_THRESHOLD_INT_END_OF_CALCULATION, 0, 10);
   
   set_adc_channel(0);
   
   enable_interrupts(INT_ADT);
   enable_interrupts(GLOBAL);
   
   while(TRUE)
   {
      //If an ADT ISR occurred print out the reading that caused it.
      if(ADCNewReading)
      {
         printf("\r\nNew ADC Reading: %lu", ADCReading);
         
         ADCNewReading = FALSE;
      }
      
      cLevel = input(PUSH_BUTTON);
      
      if(cLevel != pLevel)
      {
         delay_us(10);
         
         if(input(PUSH_BUTTON) != pLevel)
         {
            if(cLevel == 0)
            {
               printf("\r\nLast ADC Reading: %lu", ADCReading);
            }
            
            pLevel = cLevel;
         }
      }
      
      //Toggle a PIN once ever 100 ms to indicate that the main loop is running
      cTick = TickGet();
      
      if(TickDifference(cTick, pTick) >= (TICKS_PER_SECOND / 10))
      {
         output_toggle(LED);
         
         pTick = cTick;
      }
   }
}
