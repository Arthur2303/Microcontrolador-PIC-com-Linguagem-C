
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                    ex_sk_photo_resistor.c                         ////
////                                                                   ////
//// Example code showing how to read the result of the                ////
//// brightness recorded by the photo resistor.  Example code          //// 
//// developed to run on the CCS E3 Mini development kit.              ////
//// Sensor module should be inserted into the top right of the of     ////
//// the E3 I/O header: the - pin of module inserts into the GND pin   ////
//// of the E3 and the S pin of the module inserts into the B5 pin of  ////
//// the E3.                                                           //// 
////                                                                   ////
//// The photo resistor module uses a photo resistor that reports      ////
//// relative amounts of light in resistance. When the photo resistor  ////
//// is subjected to light, the resistance goes down and when the      ////
//// photo resistor is in absense of light, the resistance goes up.    //// 
////                                                                   ////
//// When the example code is running, numbers will be outputted to    ////
//// SIOW. Move your hand close to the sensor and the numbers will     ////
//// rise due to lack of light. Move your hand away from the sensor    ////
//// and the numbers will fall due to the presence of light.           ////
////                                                                   ////
//// This sensor module is sometimes called ky018.                     ////
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
and output via SIOW. It will also light the green LED if the photo resistor
senses darkness above 50.
*/
void main(void)
{
   unsigned int8 adc;
   
   printf("\r\n\r\nex_sk_photo_resistor.c - example starting\r\n\r\n");
   
   //Set up for the adc's internal instruction clock and aquisistion clock
   setup_adc(ADC_CLOCK_INTERNAL|ADC_TAD_MUL_2);
   
   //Set up for PIN_B5 to become analog input
   setup_adc_ports(sAN11);
   
   //Sets the channel to 11 for the adc to reference
   set_adc_channel(11);
   
   while(TRUE)
   {
      delay_ms(250);

      adc = read_adc();
      
      printf("light=%u\r\n", adc);
   }
}

