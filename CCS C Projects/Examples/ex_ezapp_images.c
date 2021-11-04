///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          ex_ezapp_images.c                        ////
////                                                                   ////
////  This example shows how to use the image functionality of         ////
////  EZ App Lynx. It shows how to change the application logo,        ////
////  loading the CCS logo from the app's internal assets, loading     ////
////  images from web links, and setting the images as buttons.        ////
////                                                                   ////
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

// When this is defined, it will change the logo on the Settings screen
// of the application to a different image. Comment this out to make the logo
// change back to the default preloaded CCS logo.

//#define USE CUSTOM_LOGO

void main()
{
   uint16_t val;
   ezapp_image_cfg_t logoImgCfg;
   ezapp_image_cfg_t preloadedImgCfg;
   ezapp_image_cfg_t onestateButtonImgCfg;
   ezapp_image_cfg_t twostateButtonImgCfg;
   ezapp_field_index_t onestateButtonImgIdx;
   ezapp_field_index_t twostateButtonImgIdx;
   

   HW_INIT();  //init pins, peripherals, turn on global interrupts

   UARTConsoleInit(19200);
   UARTConsolePutsROM((rom char *)"ex_ezapp_images.c running\r\n\r\n");

   ADC_INIT();
   TickInit();
   EZAppInit();   //init EZ App Lynx API, init Bluetooth module
  
   EZAppSetTitleROM((rom char *)"ex_ezapp_images");
   EZAppSetValueStringROM(EZAppAddFieldString(), (rom char *)"Image Example"); 
    
   rom char *preloadedLogo = "logo";
    
   // This changes the logo on the settings screen of the app. 
   // If USE_CUSTOM_LOGO is defined above, this will load in an image of the
   // RN4020 module from the CCS website. Otherwise it loads the default logo.
   memset(&logoImgCfg, 0x00, sizeof(logoImgCfg));
  #if defined(USE_CUSTOM_LOGO)
   rom char *urlLogo = "http://www.ccsinfo.com/images/content/bluetooth-module.jpg";
  #else
   rom char *urlLogo = preloadedLogo;
   logoImgCfg.preloaded = TRUE;
  #endif
   logoImgCfg.useLogo = TRUE;
   EZAppAddFieldImage(logoImgCfg,urlLogo);
   
   // This displays the preloaded CCS logo scaled to full screen width.
   memset(&preloadedImgCfg, 0x00, sizeof(preloadedImgCfg));
   preloadedImgCfg.scaleType = EZAPP_IMAGE_SCALE_FULL;
   preloadedImgCfg.preloaded = TRUE;
   EZAppAddFieldImage(preloadedImgCfg,preloadedLogo);
   
   // This displays a one state image button scaled to half screen width where
   // the "On" state and "Off" state images are loaded from the web.
   memset(&onestateButtonImgCfg, 0x00, sizeof(onestateButtonImgCfg));
   rom char *urlOnestateButton = "http://www.ccsinfo.com/images/content/green-on-button.png\thttp://www.ccsinfo.com/images/content/red-off-button.png";
   onestateButtonImgCfg.scaleType = EZAPP_IMAGE_SCALE_HALF;
   onestateButtonImgCfg.button = TRUE;
   onestateButtonImgCfg.oneState = TRUE;
   onestateButtonImgIdx = EZAppAddFieldImage(onestateButtonImgCfg,urlOnestateButton);
   
   // This displays a two state image button with no scaling where
   // the "On" state and "Off" state images are preloaded app images.
   memset(&twostateButtonImgCfg, 0x00, sizeof(twostateButtonImgCfg));
   rom char *preloadedButton = "led_round_green\tled_round_red";
   twostateButtonImgCfg.scaleType = EZAPP_IMAGE_SCALE_NONE;
   twostateButtonImgCfg.button = TRUE;
   twostateButtonImgCfg.preloaded = TRUE;
   twostateButtonImgIdx = EZAppAddFieldImage(twostateButtonImgCfg,preloadedButton);
   
   // Initialize relays
   EZAppSetValue(twostateButtonImgIdx, 0);
   DIGOUT_SET(DIGOUT0_PIN, 0);
   DIGOUT_SET(DIGOUT1_PIN, 0);
  
   for(;;)
   { 
      restart_wdt();
      
      HW_TASK();       
     
      EZAppTask();
      
      // turn on LEDs if onestate button is held
      val = EZAppGetValue(onestateButtonImgIdx);
      LED_SET(LED0_PIN, bit_test(val, 0));
      LED_SET(LED1_PIN, bit_test(val, 0));
      LED_SET(LED2_PIN, bit_test(val, 0));
      
      // change relay depending on the state of the two state button
      if (EZAppGetKbhit(twostateButtonImgIdx))
      {
         val = EZAppGetValue(twostateButtonImgIdx);
         DIGOUT_SET(DIGOUT0_PIN, bit_test(val, 0));
      }
   }
}



