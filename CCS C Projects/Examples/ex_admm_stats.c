/////////////////////////////////////////////////////////////////////////
////                          EX_ADMM_STATS.C                        ////
////                                                                 ////
////  This program displays the min and max of 50 A/D conversions.   ////
////  The process is repeated forever.                               ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCM and PCH compilers.  The    ////
////  following conditional compilation lines are used to include a  ////
////  valid device for each compiler.  Change the device, clock and  ////
////  RS232 pins for your hardware if needed.                        ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2001,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCH__)
#include <18F4520.h>
#elif defined(__PCM__)
#include <16F887.h>
#endif

#use delay( crystal=20mhz )
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART

float sqrt(float x);
float fabs(float x);

void main() {

   int1 loopDoneFlag;
   int i,value[50],min,max,tempSum,numberOfSamples=51;
   float standardDeviation, mean;

   printf("EX_ADMM\r\nSampling:");

   setup_port_a( ALL_ANALOG );
   setup_adc( ADC_CLOCK_INTERNAL );
   set_adc_channel( 0 );


   while (TRUE)
   {
      loopDoneFlag = 0;
      min=255;
      max=standardDeviation=tempSum=0;

      for(i=0;i<50;++i)
      {
         delay_ms(10);
         value[i] = Read_ADC();
      }

      for(i=0;i<50;++i)
      {
         if (value[i]<min)   min=value[i];
         if (value[i]>max)   max=value[i];
         mean+=value[i];
      }

      mean /= numberOfSamples;

      for(i=0;i<50;++i)
      {
         tempSum = fabs(value[i] - mean);
         standardDeviation += tempSum;
      }

      standardDeviation /= numberOfSamples;
      standardDeviation = sqrt(standardDeviation);

      loopDoneFlag = 1;

      printf("\n\rMin: %2U  Max: %2U  Mean: %2F Std.Dev: %2F\r\n",
         min,max,mean,standardDeviation);
   }
}


float fabs(float x)
{
   if(x < 0)
      return(x*-1);
}

float sqrt(float x)
{
   float y, res;
   byte *p;

   #ifdef _ERRNO
   if(x < 0)
   {
      errno=EDOM;
   }
   #endif

   if( x<=0.0)
      return(0.0);

   y=x;
   p=&y;
   (*p)=(byte)((((int16)(*p)) + 127) >> 1);

   do {
      res=y;
      y+=(x/y);
      (*p)--;
   } while(res != y);

   return(res);
}


