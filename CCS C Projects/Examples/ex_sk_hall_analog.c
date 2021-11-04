///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                      ex_sk_hall_analog.c                          ////
////                                                                   ////
//// Example code showing how to read the result of the                ////
//// magnetic field recorded by the magnetic hall effect sensor.       ////
//// Example code developed to run on the CCS E3 Mini development kit. ////
//// Sensor module should be inserted into the top right of the of     ////
//// the E3 I/O header: the G pin of module inserts into the GND pin   ////
//// of the E3 and the Y pin of the module inserts into the B5 pin of  ////
//// the E3.                                                           ////
////                                                                   ////
//// This module detects magnetic fields and returns an analog result  ////
//// based on the intensity of the field.                              ////
////                                                                   ////
//// When the example code is running, numbers will be outputted to    ////
//// SIOW. Move a magnet close to the sensor and the numbers will      ////
//// fall due to the magnetic field. Move the magnet away from the     ////
//// sensor and the numbers will rise due to no magnetic field.        ////
////                                                                   ////
//// This sensor module is sometimes called ky035.                     ////
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

/*
This main program will set up the on board ADC to read channel 11(PIN_B5)
and output via SIOW a number representation of a magnetic field.
*/
void main(void)
{
   int8 adc;
   
   printf("\r\n\r\nex_sk_hall_analog.c - example starting\r\n\r\n");
   
   //Set up for the adc's internal instruction clock and aquisistion clock
   setup_adc(ADC_CLOCK_INTERNAL|ADC_TAD_MUL_2);
   
   //Set up for PIN_B5 to become analog input
   setup_adc_ports(sAN11);
   
   //Sets the channel to 11 for the adc to reference
   set_adc_channel(11);
   
   while(TRUE)
   {
      delay_ms(1000);
      
      adc = read_adc();
      
      printf("hall=%u\r\n", adc);
   }
}

