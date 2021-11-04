/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                            EX_TIME.C                            ////
////                                                                 ////
////  Demo of using CCS's time.h standard library with timebase      ////
////  drivers.  See time.h for more documentation and CCS provided   ////
////  timebase drivers.                                              ////
////                                                                 ////
////  Look for ** TIMEBASE SELECTION ** in this example, which has   ////
////  a series of #if/#elif that allows you to select the timebase   ////
////  for time.h.                                                    ////
////                                                                 ////
////  When running, current time is continously displaced on the     ////
////  serial port.  If LCD is on your board, time is also displayed  ////
////  over the LCD.  When the program starts, it will ask for        ////
////  current date/time over the serial port.                        ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2012,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <16F887.h>

#use delay(crystal=20Mhz)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, stream=STREAM_USER_UART) //Text through the UART

#ifndef BUTTON_PRESSED
   // if your hardware doesn't have a button, then everytime you power up
   // it will ask you to init the clock.
   #define BUTTON_PRESSED()   (TRUE)
#endif

#include <stdlib.h>
#include <string.h>
#include <input.c>

#include <time.h>

// ** TIMEBASE SELECTION **
// you must select one of these timebases.

#if 0   //set to 1 if you have an external DS1305 real time clock.
   #include <ds1305.c>
#elif 0 //set to 1 if you have a PIC with internal real time clock.
   #include <rtcperipheral.c>
#else
   // if you don't use one of the above real time clock devices, the
   // example will use a PIC timer.  this is the least accurate
   // but is compatabile with every PIC.
   #include <rtcticks.c>
#endif

void GetTime(void)
{
   struct_tm tm;
   
   GetTime(&tm);
   
   fprintf(STREAM_USER_UART, "\r\nMinute (0-59): ");
   tm.tm_min = get_Int8Edit(tm.tm_min);
   
   fprintf(STREAM_USER_UART, "\r\nHour (0-23): ");
   tm.tm_hour = get_Int8Edit(tm.tm_hour);

   fprintf(STREAM_USER_UART, "\r\nDay (1-31): ");
   tm.tm_mday += 1;
   tm.tm_mday = get_Int8Edit(tm.tm_mday);
   tm.tm_mday -= 1;

   fprintf(STREAM_USER_UART, "\r\nMonth (1-12): ");
   tm.tm_mon += 1;
   tm.tm_mon = get_Int8Edit(tm.tm_mon);
   tm.tm_mon -= 1;   
   
   fprintf(STREAM_USER_UART, "\r\nYear (ex: 2012): ");
   tm.tm_year += 1900;
   tm.tm_year = get_Int16Edit(tm.tm_year);
   tm.tm_year -= 1900;
   
   SetTime(&tm);
}

void main(void)
{
   time_t oldTime, newTime;
   char str[40];
   struct_tm tm;
   
  #if defined(__LCD_C__)
   lcd_init();
  #endif
   
   delay_ms(72);
   
   printf("\r\n\nCCS C Compiler ex_time.c example starting\r\n\n");
   
   TimeInit();
   
   if (BUTTON_PRESSED())
   {
      //ask for current time from user and save it RTC
      GetTime();
      fprintf(STREAM_USER_UART, "\r\n\n");
   }
   
   for(;;)
   {
      restart_wdt();
      
      delay_ms(10);
      
      newTime = time(NULL);

      if (oldTime != newTime)
      {
         oldTime = newTime;

         GetTime(&tm);
   
         asctime(&tm, str);
         
         fprintf(STREAM_USER_UART, "\r%s - %lu - %lu", str, newTime, clock());
         
        #if defined(__LCD_C__)
         //by using &str[4], we skip the day portion of asctime()
         printf(lcd_putc, "\aCCS ex_time.c\n%s\n", &str[4]);
        #endif
      }
   }
}
