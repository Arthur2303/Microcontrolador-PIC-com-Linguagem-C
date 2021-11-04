/////////////////////////////////////////////////////////////////////////
////                           EX_RTCLK.C                            ////
////                                                                 ////
////  This program reads and writes to an external Real Time Clock.  ////
////  It communicates to the user using the LCD and keypad.          ////
////                                                                 ////
////  Configure the CCS prototype card as follows:                   ////
////      Install the LCD and keypad.                                ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCB, PCM and PCH compilers.    ////
////  The following conditional compilation lines are used to        ////
////  include a valid device for each compiler.  Change the device,  ////
////  clock and RS232 pins for your hardware if needed.              ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2003,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F887.h>
#elif defined(__PCB__)
#include <16F57.h>
#elif defined(__PCH__)
#include <18F4520.h>
#endif

#use delay(crystal=20mhz)
#use rs232(icd)                                   //Text through the ICD
//#use rs232 (baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART


#include <ds1302.c>
#include <lcd.c>
#include <kbd.c>

byte get_number() {
  char first,second;

  do {
    first=kbd_getc();
  } while ((first<'0') || (first>'9'));
  lcd_putc(first);
  first-='0';

  do {
    second=kbd_getc();
  } while ((second<'0') || (second>'9'));
  lcd_putc(second);
  second-='0';

  return((first*10)+second);
}

void set_clock(){
   byte day,mth,year,dow,hour,min;

   lcd_putc("\fYear 20: ");
   year=get_number();
   lcd_putc("\fMonth: ");
   mth=get_number();
   lcd_putc("\fDay: ");
   day=get_number();
   lcd_putc("\fWeekday 1-7: ");
   dow=get_number();
   lcd_putc("\fHour: ");
   hour=get_number();
   lcd_putc("\fMin: ");
   min=get_number();

   rtc_set_datetime(day,mth,year,dow,hour,min);
}

void main() {
   char cmd;
   byte day,mth,year,dow,hour,min,sec;

   rtc_init();
   lcd_init();
   kbd_init();

   lcd_putc("\f1: Change, 2: Display");

   do {
      cmd=kbd_getc();
   } while ((cmd!='1')&&(cmd!='2'));

   if(cmd=='1')
     set_clock();

   while (1) {
      lcd_putc('\f');
      rtc_get_date( day, mth, year, dow);
      rtc_get_time( hour, min, sec );
      printf(lcd_putc,"%2u/%2u/%2u\n%2u:%2u:%2u",mth,day,year,hour,min,sec);
      delay_ms(250);
   }
}
