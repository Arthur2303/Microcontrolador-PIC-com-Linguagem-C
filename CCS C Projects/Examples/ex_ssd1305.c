/////////////////////////////////////////////////////////////////////////
////                           EX_SSD1305.C                          ////
////                                                                 ////
//// This example program demonstrates the use of a graphic OLED.    ////
//// Showing the use of the OLED function to draw rectangles,        ////
//// circles and text on the screen.                                 ////
////                                                                 ////
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

#include <18LF4520.h>
#use delay(clock=64M, crystal=16M)

// Uncomment this to enable faster drawing of the display, try it both ways
// to see the difference.
//#define OLED_FAST_DRAW

#include <ssd1305.c>

void main(void) {
   char text[] = "Hello World";

   oled_init();   // Must initialize the OLED display
   
   while(TRUE)
   {
      oled_clearScreen();              //turn all pixels off
      oled_rect(0, 5, 127, 15, 0, 1);  //draw rectangle not filled
      oled_flush();
      delay_ms(750);
      
      oled_rect(0, 5, 127, 15, 1, 1);  //draw rectangle filled
      oled_flush();
      delay_ms(750);
      
      oled_fillScreen();               //turn all pixels on
      delay_ms(750);
      
      oled_clearScreen();              //turn all pixels off
      oled_text57(5, 15, text, 2, 1);  //draw text on screen, character pixels on
      oled_flush();
      delay_ms(750);
      
      oled_text57(5, 15, text, 2, 0);  //draw text on screen, character pixels off
      oled_flush();
      delay_ms(750);
      
      oled_circle(64, 16, 8, 0, 1);    //draw circle not filled
      oled_circle(100, 8, 4, 0, 1);    //draw circle not filled
      oled_flush();
      delay_ms(750);
      
      oled_circle(64, 16, 8, 1, 1);    //draw circle filled
      oled_circle(100, 8, 4, 1, 1);    //draw circle filled
      oled_flush();
      delay_ms(750);   
   }
}
