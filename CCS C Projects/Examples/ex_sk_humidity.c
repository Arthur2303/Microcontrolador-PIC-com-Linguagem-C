///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                       ex_sk_humidity.c                            ////
////                                                                   ////
//// Example code showing how to read the humidity and temperature     ////
//// from the humidity sensor.  Example code developed to run on the   ////
//// CCS E3 Mini development kit.  Sensor module should be inserted    ////
//// into the top right of the of the E3 I/O header: the - pin of      ////
//// module inserts into the GND pin of the E3 and the S pin of the    ////
//// module inserts into the B5 pin of the E3.                         ////
////                                                                   ////
//// The humidity sensor module utilizies a low-cost DHT11 humidity    ////
//// module that reports relative humidity and temperature in C.  The  ////
//// DHT11 uses 1-wire for both TX and RX, of which this example code  ////
//// shows how to use to read the data from the module.  A more        ////
//// expensive DHT22 module also exists that utilizes a similar        ////
//// data protocol and yields higher precision data.                   ////
////                                                                   ////
//// When this example code is running, the humidty and temperature    ////
//// is read every 2 seconds and displayed over USB.  Use SIOW serial  ////
//// monitor software to view the data sent over USB.                  ////
////                                                                   ////
//// This sensor module is sometimes called ky015.                     ////
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

#include <dht11.c>

void main(void)
{
   unsigned int8 relativeHumidity;
   unsigned int8 tempC;
   
   printf("\r\n\r\nex_sk_humidity.c - DHT11 example starting\r\n\r\n");
   
   dht11_init();
   
   for(;;)
   {
      delay_ms(2000);
      
      dht11_read(&relativeHumidity, &tempC);
      
      printf("HUM=%03u%%, TEMP=%02uC\r\n", relativeHumidity, tempC);
   }
}

