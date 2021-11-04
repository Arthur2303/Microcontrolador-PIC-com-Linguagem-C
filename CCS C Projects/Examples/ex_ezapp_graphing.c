///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                          ex_ezapp_graphing.c                      ////
////                                                                   ////
////  This example shows how to use the graphing functionality of      ////
////  EZ App Lynx. It shows the temperature from the thermistor on a   ////
////  line graph and the two potentiometers on a bar graph.            ////
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

void main()
{
   int temp;
   uint16_t pot0;
   uint16_t pot1;
   
   ezapp_graph_cfg_t tempGraph;
   ezapp_graph_cfg_t potGraph;
   ezapp_field_index_t tempSeriesIndex;
   ezapp_field_index_t pot0SeriesIndex;
   ezapp_field_index_t pot1SeriesIndex;
   
   rom char consoleHeaderStr[] = "\r\n\r\nex_ezapp_graphing.c running\r\n\r\n";

   HW_INIT();  //init pins, peripherals, turn on global interrupts

   UARTConsoleInit(19200);
   UARTConsolePutsROM(consoleHeaderStr);

   ADC_INIT();
   
   TickInit();

   EZAppInit();   //init EZ App Lynx API, init Bluetooth module
  
   EZAppSetTitleROM((rom char *)"ex_ezapp_graphing");

   EZAppSetValueStringROM(EZAppAddFieldString(), (rom char *)"Graphing Example"); 
   
   // adc3

   rom char tempTitle[] = ADC3_STR;
   rom char tempAxis[] = "Time\tADC Value";
   rom char tempSeries[] = ADC3_STR;

   tempGraph.decimalX = 0;
   tempGraph.decimalY = 0;
   tempGraph.height = 10;
   tempGraph.maxX = 100;
   tempGraph.minX = 0;
   tempGraph.minY = 0;
   tempGraph.maxY = 60;
   tempGraph.scaleX = 1;
   tempGraph.scaleY = 1;
   tempGraph.ticksX = 10;
   tempGraph.ticksY = 12;
   tempGraph.scrollX = TRUE;
   tempGraph.scrollY = FALSE;
   tempGraph.showLegend = TRUE;
   tempGraph.numPoints = 20;
   tempGraph.type = EZAPP_GRAPH_LINE;
   tempGraph.showXGrid = FALSE;
   tempGraph.showYGrid = TRUE;
   tempGraph.labels = tempAxis;
   tempGraph.title = tempTitle;
      
   EZAppAddFieldGraph(&tempGraph);
   
   tempSeriesIndex = EZAppAddFieldGraphSeries(EZAPP_GRAPH_LINE_SOLID_TRIANGLE, 
      EZAppCreateRGB(255, 130, 130), 1, tempSeries);

   // ADC0 and ADC1
   
   rom char potTitle[] = ADC0_STR " + " ADC1_STR;
   rom char potAxis[] = "ADC Val";
   rom char pot0Series[] = ADC0_STR;
   rom char pot1Series[] = ADC1_STR;

   potGraph.decimalX = 0;
   potGraph.decimalY = 0;
   potGraph.height = 10;
   potGraph.maxX = 100;
   potGraph.minX = 0;
   potGraph.minY = 0;
   potGraph.maxY = ADC_MAX+200;
   potGraph.scaleX = 1;
   potGraph.scaleY = 1;
   potGraph.ticksX = 10;
   potGraph.ticksY = 10;
   potGraph.scrollX = TRUE;
   potGraph.scrollY = FALSE;
   potGraph.showLegend = FALSE;
   potGraph.numPoints = 20;
   potGraph.type = EZAPP_GRAPH_BAR;
   potGraph.showXGrid = FALSE;
   potGraph.showYGrid = TRUE;
   potGraph.labels = potAxis;
   potGraph.title = potTitle;

   EZAppAddFieldGraph(&potGraph);

   pot0SeriesIndex = EZAppAddFieldGraphSeries(EZAPP_GRAPH_LINE_SOLID, 
      EZAppCreateRGB(100, 255, 100), 1, pot0Series);

   pot1SeriesIndex = EZAppAddFieldGraphSeries(EZAPP_GRAPH_LINE_SOLID, 
      EZAppCreateRGB(100, 100, 255), 1, pot1Series);
  
   for(;;)
   { 
      restart_wdt();
      
      HW_TASK();       
     
      UARTConsoleTask();
      
      EZAppTask();
      
      // read temperature from thermistor, convert to Celsius
      ADC3_CHANNEL();
      temp = (ADC_READ() / -3) + 47;
      EZAppSetValue(tempSeriesIndex, temp);
      
      // read potentiometer 0
      ADC0_CHANNEL();
      pot0 = ADC_READ();
      EZAppSetValue(pot0SeriesIndex, pot0);
      
      // read potentiometer 1
      ADC1_CHANNEL();
      pot1 = ADC_READ();
      EZAppSetValue(pot1SeriesIndex, pot1);
      
      if (UARTConsoleKbhit() && (UARTConsoleGetc() == '?'))
      {
         UARTConsolePutsROM(consoleHeaderStr);
      }
   }
}



