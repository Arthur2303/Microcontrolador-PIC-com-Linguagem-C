/////////////////////////////////////////////////////////////////////////
////                            EX_RMSDB.C                           ////
////                                                                 ////
////  This program displays the RMS voltage and the dB level of the  ////
////  input voltage.  NOTE: The input voltage waveform must be       ////
////  between 0V and 5V.                                             ////
////                                                                 ////
////  Configure the CCS prototype card as follows:                   ////
////     Connect pin A1 to the voltage source.                       ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCM and PCH compilers.  The    ////
////  following conditional compilation lines are used to include a  ////
////  valid device for each compiler.  Change the device, clock and  ////
////  RS232 pins for your hardware if needed.                        ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2003,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F887.h>
#elif defined(__PCH__)
#include <18F4520.h>
#endif
#use delay(crystal=20mhz)
#use rs232(icd)                                  //Text throught the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART

#include <math.h>


void main() {
   const long NUM_DATA_POINTS = 3000;
   long i;
   int value;
   float voltage;

   printf("Sampling:\r\n");

   setup_port_a( ALL_ANALOG );
   setup_adc( ADC_CLOCK_INTERNAL );
   set_adc_channel( 1 );

   while(TRUE)
   {
      voltage = 0;
      for(i=0; i<NUM_DATA_POINTS; ++i)
      {
         value = read_ADC();
         voltage += (float)value*(float)value;
      }

      //voltage currently holds the sum of ADC values squared.
      //to convert an ADC count to voltage -> ADC VALUE * (5 Volts)/(0xFF)
      //0xFF is the resolution of the ADC.
      //since ADC values are squared, for this routine:
      //    Voltage=ADC VALUE * (5 Volts)/(0xFF) * (5 Volts)/(0xFF) = ADC VALUE/2601      voltage /=2601.0;
      voltage /=2601.0;
      
      voltage = sqrt(voltage/(NUM_DATA_POINTS));
      printf("\r\nInput =  %f V     %f dB\r\n", voltage, 20*log10(voltage));
   }
}
