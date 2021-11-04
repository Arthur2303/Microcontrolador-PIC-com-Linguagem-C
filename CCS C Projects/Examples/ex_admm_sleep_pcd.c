/////////////////////////////////////////////////////////////////////////
////                    EX_ADMM_SLEEP_PCD.C                          ////
////                                                                 ////
////  This program displays the min and max of 30 A/D samples over   ////
////  the RS-232 interface.  The process is repeated forever.        ////
////  During A/D conversion, the processor is put into "sleep"       ////
////  mode, which reduces power consumption and noise during the     ////
////  conversion time.  While in sleep, the microcontroller will     ////
////  continue running peripherals from their respective internal    ////
////  clocks (if enabled), but processor cycles will not be clocked. ////
////  Any enabled interrupts, excluding GLOBAL, will wake the        ////
////  microcontroller from sleep.  Take care when using sleep with   ////
////  interrupt driven programming.  In addition, peripheral         ////
////  interrupts in PCM require the setting of the PEIE pin, which   ////
////  is shown in the code below.                                    ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCD compiler.                  ////
////  The following conditional compilation lines are used to        ////
////  include a valid device for each compiler.  Change the device,  ////
////  clock and RS232 pins for your hardware if needed.              ////
/////////////////////////////////////////////////////////////////////////
////          (C) Copyright 2018 Custom Computer Services            ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <24FJ128GA006.h>      // Preprocessor directive that selects the chip

#device ADC=8
#use delay( crystal=20mhz )
#use rs232(icd)                //Text through the ICD
//#use rs232(baud=9600, UART1) // Text through the UART


void adc1_isr(void)
{
}

#ifndef sAN0
#define sAN0 AN0
#endif


void main() { 

   unsigned int8 i, value, min, max;

   printf("Sampling:");                      // Printf function included in RS232 library

   setup_adc_ports(sAN0);                    // Built-in A/D setup function
   
   setup_adc(ADC_CLOCK_INTERNAL);            // Built-in A/D setup function
   set_adc_channel(0);                       // Built-in A/D setup function
   
   enable_interrupts(INT_ADC1);           // Enable A/D to interrupt sleep

   enable_interrupts(INTR_ALTERNATE);                // Enable Peripheral Interrupts (not global interrupts)
   
   value = read_adc();
   while (TRUE) {
      min=255;
      max=0;
      for(i=0; i<=30; ++i) {
         delay_ms(100);                      // Built-in delay function
         read_adc(ADC_START_ONLY);           // Built-in start A/D conversion
         sleep();                            // Reduce power and noise during conversion
         value = read_adc(ADC_READ_ONLY);    // Built-in A/D read function
         //value = read_adc();
         if(value<min)
            min=value;
         if(value>max)
            max=value;
      }
      
      printf("\r\nMin: %2X  Max: %2X\n\r",min,max);     
   } 
}
