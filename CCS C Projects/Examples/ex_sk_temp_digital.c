///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                      ex_sk_temp_digital.c                         ////
////                                                                   ////
//// Example code showing how to read the result of the                ////
//// temperature recorded by the digital temperature sensor.  This     ////
//// sensor uses a Dallas DS18B20 digital thermometer.  Example code   //// 
//// developed to run on the CCS E3 Mini development kit.              ////
//// Sensor module should be inserted into the top right of the of     ////
//// the E3 I/O header: the G pin of module inserts into the GND pin   ////
//// of the E3 and the Y pin of the module inserts into the B5 pin of  ////
//// the E3.                                                           //// 
////                                                                   ////
//// This sensor reads the temperature by using a ds18B20 module.      ////
//// The value is read from the DS18B20 using it's one wire            ////
//// communication protocol.                                           ////
////                                                                   ////
//// When the example code is running, temperature in C will be        ////
//// outputted to USB serial (can be viewed in SIOW).  Breathe on or   ////
//// touch the sensor to see that the temperature changes accordingly. ////
////                                                                   ////
//// This sensor module is sometimes called ky001.                     ////
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

#include <ds18b20.c>

void main(void)
{
   signed int16 val;
   
   printf("\r\n\r\nex_sk_temp_digital.c - example starting\r\n\r\n");
   
   ds18b20_init();
   
   while(true)
   {
      delay_ms(1000);
      
      ds18b20_read(&val);
         
      printf("temp=%ldC\r\n", val/(signed int16)16);
   }
}

