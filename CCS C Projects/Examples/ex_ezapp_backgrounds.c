///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                      ex_ezapp_backgrounds.c                       ////
////                                                                   ////
//// A demo that shows how to add background images to the application ////
//// and button fields, as well as making field colors transparent.    ////
////                                                                   ////
//// For full EZ App Lynx documentation, EZApp.PDF contains an         ////
//// overview of the entire system while EZApp.h documents the full    ////
//// API that can be used with this library.                           ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2015 Custom Computer Services           ////
////                                                                   ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler or to users who purchased the rights to use this code.   ////
//// This source code may only be distributed to other licensed users  ////
//// of the CCS C compiler or other users who purchased the rights to  ////
//// this code.  The rights to use this code can only be granted       ////
//// by CCS.  No other use, reproduction or distribution is permitted  ////
//// without written permission.  Derivative programs created using    ////
//// this software in object code form are not restricted in any way.  ////
////                                                                   ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#if defined(__PCB__)||defined(__PCM__)||defined(__PCH__)||defined(__PCD__)
   // CCS C Compiler

   // Common definitions for all the examples.
   // Hardware is configured in this .h file and then populates several
   // macros/definitions used by all the EZ App Lynx examples.
   #include "ex_ezapp_common.h"

#elif defined(__XC8) || defined(__XC16) || defined(__XC32)
   // Microchip XC compiler

   // Choose hardware by setting the project's include path to the directory
   // containing system.c/system.h for your hardware.  system.c contains
   // any run-time code by the hardware.  system.h contains all
   // macros/definitions needed by the EZ App Lynx examples.
   #include <system.c>
   #include <system.h>

#endif

#include <EZApp.h>

#ifndef BUTTON0_PRESSED
#define BUTTON0_PRESSED() (false)
#endif

#ifndef BUTTON1_PRESSED
#define BUTTON1_PRESSED() (false)
#endif

// text style:
//    left column: white color, bold, one point bigger.
//    right column: white color, bold, one point bigger.
//
// row colors (extended) style:
//    5dp light gray border (50% transparent) with 5dp radius/rounding.
//    background gradient is gray (top) to dark gray (bottom), at 50% transparent.
//
// button style:
//    5dp border, 5dp border radius, bordor color white, text color white,
//    top background color dark gray, bottom background color black,
//    text size default, innerPadding 4dp
//
// button style extra:
//    text alignment is center horizontal and center vertical
//    max button height is 4 lines of text
//    off/on images are preloaded in the app and are called "button_red.png" and "button_green.png"
//
// background style:
//    background image is preloaded and is called "app_background.png"
rom char style[] =
{
   EZAPP_STYLE_TEXT_CREATE(
         true,
         true,
         1,
         EZAppCreateRGB(0xFF, 0xFF, 0xFF),
         1,
         EZAppCreateRGB(0xFF, 0xFF, 0xFF),
         EZAppCreateRGB(0xFF, 0, 0)
      ),

   EZAPP_STYLE_ROW_COLORS_EX_CREATE(
         5,
         5,
         EZAppCreateARGB(0x80, 0xEE, 0xEE, 0xEE),
         EZAppCreateARGB(0x80, 0xAA, 0xAA, 0xAA),
         EZAppCreateARGB(0x80, 0x33, 0x33, 0x33)
      ),
   
   EZAPP_STYLE_BUTTON_CREATE(
         5, 
         5, 
         EZAppCreateRGB(0xFF, 0xFF, 0xFF), 
         0, 
         EZAppCreateRGB(0xFF, 0xFF, 0xFF), 
         EZAppCreateRGB(0x33, 0x33, 0x33), 
         EZAppCreateRGB(0, 0, 0),
         4
      ),
   
   EZAPP_STYLE_BUTTON_EXTRA_CREATE2(
         EZAPP_ALIGN_HOR_CENTER | EZAPP_ALIGN_VER_CENTER,
         4,
         true,
         "button_red.png",
         "button_green.png"
      ),
      
   EZAPP_STYLE_BACKGROUND_CREATE(
         true,
         "app_background.png"
      )
};

// button style extra:
//    text alignment is center horizontal and bottom vertical
//    max button height is 4 lines of text
//    off/on images are to be downloaded from the provided web URLs
rom char style2[] = 
{
   EZAPP_STYLE_BUTTON_EXTRA_CREATE2(
         EZAPP_ALIGN_HOR_CENTER | EZAPP_ALIGN_VER_BOTTOM,
         4,
         false,
         "http://www.ccsinfo.com/images/content/red-off-button.png",
         "http://www.ccsinfo.com/images/content/green-on-button.png"
      )
};

void main(void)
{
   ezapp_field_index_t ezIdxButtons = EZAPP_FIELD_INDEX_INVALID;
   ezapp_buttons_t ezButtonConfig;
   ezapp_field_index_t ezIdxButtons2 = EZAPP_FIELD_INDEX_INVALID;
   ezapp_buttons_t ezButtonConfig2;
   rom char consoleHeaderStr[] = "\r\n\r\nex_ezapp_backgrounds.c running\r\n\r\n";
   uint16_t val;
   
   HW_INIT();
   
   UARTConsoleInit(19200);
   
   ADC_INIT();
   ADC0_CHANNEL();

   EZAppInit();
   
   UARTConsolePutsROM(consoleHeaderStr);
   
   EZAppSetTitleROM((rom char *)"ex_ezapp_backgrounds.c");   //0
   
   // this style contains built-in background and button images
   EZAppAddStylesROM(style, sizeof(style)); // 1

   EZAppSetValueStringROM(EZAppAddFieldString(), (rom char*)"Backgrounds Example"); // 2
   
   memset(&ezButtonConfig, 0x00, sizeof(ezButtonConfig));
   ezButtonConfig.numButtons = 3;
   ezButtonConfig.fillRow = true;
   ezButtonConfig.oneState = false;
   
   ezIdxButtons = EZAppAddFieldButtonsROM(
         ezButtonConfig, 
         (rom char *)"LEDS:",
         (rom char *)"R OFF\tR ON\tY OFF\tY ON\tG OFF\tG ON"
      ); // 3
   
   // change the button style to load images from the web
   EZAppAddStylesROM(style2, sizeof(style2)); // 4
   
   memset(&ezButtonConfig2, 0x00, sizeof(ezButtonConfig2));
   ezButtonConfig2.numButtons = 2;
   ezButtonConfig2.fillRow = true;
   ezButtonConfig2.oneState = true;
   
   ezIdxButtons2 = EZAppAddFieldButtonsROM(
         ezButtonConfig2, 
         (rom char *)"RELAYS:",
         NULL
      ); // 5
   
   EZAppSetValue(ezIdxButtons, 0);
  #if defined(LED0_PIN)
   LED_SET(LED0_PIN, 0);
  #endif
  #if defined(LED1_PIN)
   LED_SET(LED1_PIN, 0);
  #endif
  #if defined(LED2_PIN)
   LED_SET(LED2_PIN, 0);
  #endif
   
   EZAppSetValue(ezIdxButtons2, 0);
  #if defined(DIGOUT0_PIN)
   DIGOUT_SET(DIGOUT0_PIN, 0);
  #endif
  #if defined(DIGOUT1_PIN)
   DIGOUT_SET(DIGOUT1_PIN, 0);
  #endif

   for(;;)
   {
      restart_wdt();
      
      HW_TASK();
      
      UARTConsoleTask();
      
      EZAppTask();
      
      if (EZAppGetKbhit(ezIdxButtons))
      {
         val = EZAppGetValue(ezIdxButtons);
        #if defined(LED0_PIN)
         LED_SET(LED0_PIN, bit_test(val, 0));
        #endif
        #if defined(LED1_PIN)
         LED_SET(LED1_PIN, bit_test(val, 1));
        #endif
        #if defined(LED2_PIN)
         LED_SET(LED2_PIN, bit_test(val, 2));
        #endif
      }

      val = EZAppGetValue(ezIdxButtons2);
     #if defined(DIGOUT0_PIN)
      DIGOUT_SET(DIGOUT0_PIN, bit_test(val, 0));
     #endif
     #if defined(DIGOUT1_PIN)
      DIGOUT_SET(DIGOUT1_PIN, bit_test(val, 1));
     #endif
     
      if (UARTConsoleKbhit() && (UARTConsoleGetc() == '?'))
      {
         UARTConsolePutsROM(consoleHeaderStr);
      }
   }
}
