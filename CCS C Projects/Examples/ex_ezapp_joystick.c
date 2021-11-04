///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                      ex_ezapp_joystick.c                          ////
////                                                                   ////
//// A demo that shows how to add a joystick field and receive         ////
//// position changes from the smartphone or tablet.                   ////
////                                                                   ////
//// For full EZ App Lynx documentation, EZApp.PDF contains an         ////
//// overview of the entire system while EZApp.h documents the full    ////
//// API that can be used with this library.                           ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2017 Custom Computer Services           ////
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
   ezapp_field_index_t ezIdxJoystick = EZAPP_FIELD_INDEX_INVALID;
   ezapp_field_index_t angleTextIdx = EZAPP_FIELD_INDEX_INVALID;
   ezapp_field_index_t strengthTextIdx = EZAPP_FIELD_INDEX_INVALID;
   
   char angleTextStr[32] = "0";
   char strengthTextStr[32] = "0";
   
   ezapp_joystick_cfg_t joyCfg;
   
   uint16_t val;
   uint16_t angle;
   uint8_t strength;
   
   HW_INIT();
   
   UARTConsoleInit(19200);
   
   ADC_INIT();
   ADC0_CHANNEL();

   EZAppInit();
   
   UARTConsolePutsROM((rom char *)"\r\n\r\nex_ezapp_joystick.c running\r\n\r\n");
   
   EZAppSetTitleROM((rom char *)"ex_ezapp_joystick.c");   //0

   EZAppSetValueStringROM(EZAppAddFieldString(), (rom char*)"Joystick Example");   //1
   
   angleTextIdx = EZAppAddFieldText((rom char*)"Angle:", 0);  //2
   EZAppSetValueString(angleTextIdx, angleTextStr);
   
   strengthTextIdx = EZAppAddFieldText((rom char*)"Strength:", 0);  //3
   EZAppSetValueString(strengthTextIdx, strengthTextStr);
   
   // Set up the joystick and add it to the screen
   memset(&joyCfg, 0, sizeof(joyCfg));
   joyCfg.backgroundColor = EZAppCreateRGB(0x8A,0x89,0x8D);
   joyCfg.borderColor = EZAppCreateRGB(0x3D,0x3B,0x41);
   joyCfg.stickColor = EZAppCreateRGB(0x9F,0x35,0x34);
   joyCfg.borderWidth = 5;
   joyCfg.maxHeight = 15;
   joyCfg.autoReCenter = true;
   ezIdxJoystick = EZAppAddFieldJoystick(&joyCfg, (rom char*)"Control:"); //4
   
   for(;;)
   {
      restart_wdt();
      
      HW_TASK();
      
      EZAppTask();

      if (EZAppGetKbhit(ezIdxJoystick))
      {
         // get the new value and extract the angle/strenth parts
         val = EZAppGetValue(ezIdxJoystick);
         strength = EZAppGetJoystickStrength(val);
         angle = EZAppGetJoystickAngle(val);
         
         // display the current Angle (in degrees)
         sprintf(angleTextStr,"%lu deg",angle);
         EZAppSetValueString(angleTextIdx, angleTextStr);
         
         // display the current Strength (in percent)
         sprintf(strengthTextStr,"%u %%",strength);
         EZAppSetValueString(strengthTextIdx, strengthTextStr);
      }
   }
}
