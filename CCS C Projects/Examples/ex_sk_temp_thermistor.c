///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                      ex_sk_temp_thermistor.c                      ////
////                                                                   ////
//// Example code showing how to read the result of the                ////
//// temperature recorded by the thermistor sensor module.  Example    //// 
//// code developed to run on the CCS E3 Mini development kit.         ////
//// Sensor module should be inserted into the top right of the of     ////
//// the E3 I/O header: the - pin of module inserts into the GND pin   ////
//// of the E3 and the S pin of the module inserts into the B5 pin of  ////
//// the E3.                                                           //// 
////                                                                   ////
//// This module detects the temperature by using a thermistor.        ////
//// Thermistor resistance changes due to temperature; acts as a       ////
//// resistor in a voltage divider from which we read the voltage      ////
//// at the voltage divider.  The voltage is read by using an ADC      ////
//// on the PIC of the E3 and decoded into a temperature.              ////
////                                                                   ////
//// When the example code is running, temperature (in Fahrenheit)     ////
//// will be outputted to USB CDC Serial (can be viewed in SIOW).      ////
////                                                                   ////
//// This sensor module is sometimes called ky013.                     ////
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

#include <thermistor.c>

void main(void)
{
   int8 adc;
   float tempF;
   printf("\r\n\r\nex_sk_temp_thermistor.c - example starting\r\n\r\n");
   
   // setup ADC to read from channel 11 (PIN_B5)
   setup_adc(ADC_CLOCK_INTERNAL|ADC_TAD_MUL_2);
   setup_adc_ports(sAN11);
   set_adc_channel(11);
   
   thermistor_config(25, 10000, 3470, FALSE, 10000, 8);
   
   while(TRUE)
   {
      delay_ms(1000);
      
      adc = read_adc();
      tempf = (float)(thermistor_convert(adc))/10.0;
      
      printf("temp=%fF\r\n", tempF);
   }
}

