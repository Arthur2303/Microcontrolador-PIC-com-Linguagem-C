///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                        ex_sk_barometer.c                          ////
////                                                                   ////
//// Example code showing how to read the temperature, pressure and    ////
//// altitude from the barometer module using I2C. Example code        ////
//// developed to run on the CCS E3 Mini development kit.  Wire the    ////
//// sensor module so VIN is wired to E3's +5V, GND to E3's G,         ////
//// SCL to E3's B5 and SDA to E3's B4.                                ////
////                                                                   ////
//// This module detects the temperature, pressure and altitude of     ////
//// the surrounding area.  When running this example on an E3, the    ////
//// altitude calculations are disabled because there is not enough    ////
//// memory (see BMP180_NO_ALTITUDE).                                  ////
////                                                                   ////
//// When running, the temperature, pressure and altitude detected     ////
//// are displayed over SIOW.                                          ////
////                                                                   ////
//// This sensor module is sometimes called bmp180.                    ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#include <e3.h>

// the e3 doesn't have enough memory to perform the math to calculate the
// pressure.
#define BMP180_NO_ALTITUDE

#include <bmp180.c>

void main(void)
{
   signed int32 pressure;
   signed int32 temperature;
   signed int32 altitude;

   printf("\r\n\r\nex_sk_barometer.c - BMP180 example starting\r\n\r\n");

   bmp180_init();
   
   while(1)
   {
      bmp180_read(BMP180_MODE_STANDARD, &pressure, &temperature, &altitude);
      
      printf("temp=%ld C, pressure=%ld Pa, altitude=%ld m\r\n", 
            temperature,
            pressure, 
            altitude
         );
      
      delay_ms(1000);
   }
}

