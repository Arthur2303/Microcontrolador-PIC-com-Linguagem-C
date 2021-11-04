///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                      ex_ezapp_colorpicker.c                       ////
////                                                                   ////
//// A demo that shows how to add a color picker field and how to      ////
//// update its color and receive color changes from the smartphone or ////
//// tablet.                                                           ////
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

void main(void)
{
   ezapp_field_index_t ezIdxColorPicker = EZAPP_FIELD_INDEX_INVALID;
   ezapp_field_index_t viewTextIdx = EZAPP_FIELD_INDEX_INVALID;
   char viewTextStr[32] = "0x0000FF";
   uint32_t val;
   
   HW_INIT();
   
   UARTConsoleInit(19200);
   
   ADC_INIT();
   ADC0_CHANNEL();

   EZAppInit();
   
   UARTConsolePutsROM((rom char *)"\r\n\r\nex_ezapp_colorpicker.c running\r\n\r\n");
   
   EZAppSetTitleROM((rom char *)"ex_ezapp_colorpicker.c");   //0

   EZAppSetValueStringROM(EZAppAddFieldString(), (rom char*)"Color Picker Example");   //1
   
   ezIdxColorPicker = EZAppAddFieldColorPicker((rom char*)"Color:"); //2-3
   
   viewTextIdx = EZAppAddFieldText((rom char*)"Color Hex:", 0);  //4
   EZAppSetValueString(viewTextIdx, viewTextStr);
   
   

   EZAppSetValue32(ezIdxColorPicker, 0x000000FF); // initial color to blue

   for(;;)
   {
      restart_wdt();
      
      HW_TASK();
      
      EZAppTask();

      if (EZAppGetKbhit(ezIdxColorPicker))
      {
         val = EZAppGetValue32(ezIdxColorPicker);
         sprintf(viewTextStr,"0x%06lX",val);
         EZAppSetValueString(viewTextIdx, viewTextStr);
      }
   }
}
