/////////////////////////////////////////////////////////////////////////
////                           EX_SSD1306.C                          ////
////                                                                 ////
//// This example program demonstrates the use of a graphic OLED.    ////
//// A reading is taken by the analog to digital converter and       ////
//// displayed on the OLED display. A bar shows the current reading  ////
//// relative to the minimum and maximum values. If the reading is   ////
//// greater than 4 volts, a warning message is displayed. A clock   ////
//// timer demonstrates the use of the circle and line functions     ////
//// and shows that the program is active.                           ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2020 Custom Computer Services        ////
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

// Uncomment this to enable faster drawing of the display, try it both ways
// to see the difference.
//#define OLED_FAST_DRAW

#include <math.h>
#include <ssd1306.c>

void displayVoltage(int adc) {
   char voltage[9];
   sprintf(voltage, "%f", (float)adc * .01960784); // Converts adc to text
   voltage[4] = '\0';                              // Limit shown digits to 3
   oled_rect(45, 18, 69, 25, 1, 0);                // Clear the old voltage
   oled_text57(45, 18, voltage, 1, 1);             // Write the new voltage
}

void main() {
   int1  warn = FALSE;
   int8  adc = 0, adc_old = 0;
   char  voltText[] = "Volts", warning[] = "Warning";
   float theta = 0;

   setup_adc_ports(sAN0);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);

   oled_init();                                // Must initialize the OLED display
   oled_clearScreen();
   
   oled_rect(1, 5, 126, 15, 0, 1);             // Outline the bar
   oled_text57(70, 18, voltText, 1, 1);        // Display "Volts"
   oled_circle(30, 47, 10, 0, 1);              // Draw the clock circle

   for(;;) {
      adc = read_adc();                         // Read a value from the ADC
      displayVoltage(adc);                      // Display the reading
      adc = (adc > 249) ? 249 : adc;            // Keep the value 249 or less

      if(adc != adc_old) {
         oled_rect(adc/2+1, 6, adc_old/2+1, 14, 1, 0);   // Clears the old bar
         oled_rect(1, 6, adc/2+1, 14, 1, 1);             // Draws a new bar
         adc_old = adc;                                  // Set old value to new

         if(adc > 200 && !warn) {                  // Check if over 4 volts
            oled_rect(45, 38, 124, 55, 1, 1);      // Draw a filled black rectangle
            oled_text57(47, 40, warning, 2, 0);    // Write "Warning" on the OLED display
            warn = TRUE; }
         else if(adc <=200 && warn) {
            oled_rect(45, 37, 125, 55, 1, 0);  // Draw a filled white rectangle
            warn = FALSE; }
      }

      // The following 3 lines make the clock hand spin around
      oled_line(30, 47, 30+(int)(8*sin(theta)+.5), 47-(int)(8*cos(theta)+.5), 0);
      theta = (theta > 5.9) ? 0 : (theta += .3);
      oled_line(30, 47, 30+(int)(8*sin(theta)+.5), 47-(int)(8*cos(theta)+.5), 1);
      
      oled_flush();

      delay_ms(100);    // Reduces flicker by allowing pixels to be on
                        // much longer than off
   }
}
