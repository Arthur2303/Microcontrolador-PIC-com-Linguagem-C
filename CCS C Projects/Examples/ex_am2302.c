/////////////////////////////////////////////////////////////////////////
////                           ex_am2303.c                           ////
////                                                                 ////
////  Example program showing how to read the humidity and           ////
////  temperature from an AM2303 sensor.                             ////
////                                                                 ////
////  This example can be used to read a single reading or average   ////
////  multiple readings from the sensor.  Additionally it can be set ////
////  to automatically read from the sensor or only take readings    ////
////  when requested.                                                ////
////                                                                 ////
////  This example will work the PCB, PCM, PCH and PCD compilers.    ////
////  Change the device, clock, rs232 and AM2303_DATA_PIN define as  ////
////  need for your hardware.                                        ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2021 Custom Computer Services        ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
////                      http://www.ccsinfo.com                     ////
/////////////////////////////////////////////////////////////////////////

#include <18F45K22.h>
#fuses NOWDT
#use delay(clock=64M, crystal=16M)

#use rs232(xmit=PIN_C6, rcv=PIN_C7, baud=9600)

// Optional if AM2302_USE_CYCLE_DELAY is defined the am2302_read() function 
// will add an appropriate delay to the end of it so it can't be called more
// frequency then once every 2 seconds.  When commented out this example uses
// a tick timer, tick.c, to only call the am2302_read() once every 2 seconds
// when set for automatic reading mode.
//#define AM2302_USE_CYCLE_DELAY

//Used to select the I/O pin to communicate with sensor
#define AM2302_DATA_PIN    PIN_C0

#include <am2302.c>

#ifndef AM2302_USE_CYCLE_DELAY
 #define __CCS_USE_TIMER_PERIPH  1
 #include <tick.c>
 #define TickDifference(a,b)     (a-b)
#else
 #define TickGet()               1
 #define TickDifference(a,b)     2
 #define TICK                    unsigned int8
 #define TICKS_PER_SECOND        1
#endif

// Number of samples to average when using read_average()
#define AVERAGE_N    4

// Note: Each am2302_read requires a 2 second delay at the end of the process.
//       This function will take (n*2 seconds) to complete
void read_average(unsigned int16* rh, signed int16* t, unsigned int16 n)
{
   unsigned int16 tempH = 0;
   signed int16 tempT = 0;
   
   *rh = 0;
   *t = 0;
   
   for(int i = 0; i < n; i++)
   {
      am2302_read(&tempH, &tempT);
      *rh += tempH;
      *t += tempT;
     #ifndef AM2302_USE_CYCLE_DELAY
      delay_ms(2000);
     #endif
   }
   
   *rh /= n;
   *t /= n;
   
   return;
}

int1 DoAverage = FALSE;
int1 DoReadings = TRUE;

void PrintMenu(void)
{
   printf("\r\nAutomatic Readings - ");
   if(DoReadings)
      printf("Enabled");
   else
      printf("Disabled");
   printf(", Average Readings - ");
   if(DoAverage)
      printf("True");
   else
      printf("False");
   printf("\r\n   1) Toggle Continuous Reading\r\n");
   printf("   2) Toggle Single/Average Reading\r\n");
   printf("   R) Start Instantaneous Reading, when Automatic Reading is Disabled\r\n");
   printf("   A) Start Average Reading, when Automatic Reading is Disabled\r\n\n");
}

void main(void)
{
   char c;
   unsigned int16 rh;
   signed int16 t;
   TICK cTick, pTick;
   
   printf("Initializing... ");
   am2302_init(); //initialize the AM2302
   printf("Ready\r\n");
   
   PrintMenu();
   
   pTick = TickGet();
   
   while(TRUE)
   {
      rh = 0;
      t = 0;
      
      if(kbhit())
      {
         c = toupper(getc());
         
         if(c == '1')
         {
            DoReadings++;
            
            printf("Automatic Readings - ");
            
            if(DoReadings)
            {
               printf("Enabled");
               
               pTick = TickGet();
            }
            else
               printf("Disabled");
            
            printf("\r\n");
         }
         else if(c == '2')
         {
            DoAverage++;
            
            printf("Average Readings - ");
            
            if(DoAverage)
               printf("True");
            else
               printf("False");
            
            printf("\r\n");
         }   
         else if(c == 'R')
         {
            if(DoReadings == FALSE)
            {
               //Get instantaneous reading of humidity and temperature
               printf("Reading Instantaneous - ");
               am2302_read(&rh, &t);
               printf("H: %.1f%% T: %.1f C\r\n", am2302_to_float(rh), am2302_to_float(t));
            }
         }
         else if(c == 'A')
         {
            if(DoReadings == FALSE)
            {
               //Get average values over AVERAGE_N samples
               printf("Reading Average Over %d Samples - ", AVERAGE_N);
               read_average(&rh, &t, AVERAGE_N);
               printf("H: %.1f%% T: %.1f C\r\n", am2302_to_float(rh), am2302_to_float(t));
            }
         }
         else
            PrintMenu();
      }
      
      if(DoReadings == TRUE)
      {
         cTick = TickGet();
         
         if(TickDifference(cTick, pTick) >= (2 * TICKS_PER_SECOND))
         {
            if(DoAverage)
            {
               printf("Auto Average Over %d Samples - ", AVERAGE_N);
               
               read_average(&rh, &t, AVERAGE_N);
            }
            else
            {
               printf("Auto Instantaneous - ");
               
               am2302_read(&rh, &t);
            }
               
            printf("H: %.1f%% T: %.1f C\r\n", am2302_to_float(rh), am2302_to_float(t));
            
            pTick = cTick;
         }
      }
   }
}
