///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                        ex_sk_accelerometer.c                      ////
////                                                                   ////
//// Example code showing how to use the ADXL345 sensor board. Example ////
//// code developed to run on the CCS E3 Mini development Kit.         ////
//// Connect the sensor's SCL pin to the E3's B5 pin, the SDA pin to   ////
//// the E3's B4 pin, the sensor's SDO pin to E3's G pin, the sensor's ////
//// CS pin to E3's +5 pin, the sensor's GND pin to the E3's G pin     ////
//// and the sensor's VCC to E3's +5 pin.                              ////
////                                                                   ////
//// This sensor reads the acceleration measured in the X, Y and Z     ////
//// axis.                                                             ////
////                                                                   ////
//// When this example is running, the acceleration measured in the    ////
//// X, Y and Z axis will be displayed in SIOW.  The values displayed  ////
//// in G's (1G is one gravity, or 9.8m/s^2).                          ////
////                                                                   ////
//// This sensor module is sometimes called adxl345.                   ////
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

#include <adxl345.c>

void main(void)
{
   signed int16 x, y, z;
   float xg, yg, zg;
   
   printf("\r\n\r\nex_sk_accelerometer.c - ADXL345 example\r\n\r\n");
   
   adxl345_init();
   
   while(1)
   {
      adxl345_read(&x, &y, &z);
      
      xg = (float)x * 0.0039;
      yg = (float)y * 0.0039;
      zg = (float)z * 0.0039;
      
      printf("x=%fg, y=%fg, z=%fg\r\n", xg, yg, zg);
      
      delay_ms(1000);
   }
}
