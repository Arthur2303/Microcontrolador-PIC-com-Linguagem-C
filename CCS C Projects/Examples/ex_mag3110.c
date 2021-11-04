////////////////////////////////////////////////////////////////////////////
////                                                                    ////
////                           ex_mag3110.c                             ////
////                                                                    ////
//// An example of using the MAG3110 driver to read magnetic compass    ////
//// data from the MAG3110 chip.  This code was designed to run on the  ////
//// CCS Robot Development kit, which has a MAG3110 chip on it.         ////
////                                                                    ////
//// Twice a second the compass data is read and displayed on the       ////
//// console (serial terminal).  Try rotating the robot on all 3 axis   ////
//// to see how the readings change.                                    ////
////                                                                    ////
//// Pressing the button on the robot while running will start the      ////
//// calibration process.  To calibrate, slowly spin the robot in a     ////
//// circle until you have spun over 360 degrees.  Once finished        ////
//// spinning press the button again to finish and save calibration.    ////
//// Calibration data will be saved to the PIC's non-volatile memory    ////
//// so it is restored on power-up.  The calibration process removes    ////
//// the offset from the X, Y and Z readings, as well as determines     ////
//// how to find the heading degrees from the X and Y readings.         ////
//// Holding the button down while powering up the robot will erase     ////
//// previous calibration data.                                         ////
////                                                                    ////
////////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2020 Custom Computer Services            ////
////                                                                    ////
//// This source code may only be used by licensed users of the CCS C   ////
//// compiler or to users who purchased the rights to use this code.    ////
//// This source code may only be distributed to other licensed users   ////
//// of the CCS C compiler or other users who purchased the rights to   ////
//// this code.  The rights to use this code can only be granted        ////
//// by CCS.  No other use, reproduction or distribution is permitted   ////
//// without written permission.  Derivative programs created using     ////
//// this software in object code form are not restricted in any way.   ////
////                                                                    ////
////                     http://www.ccsinfo.com                         ////
////////////////////////////////////////////////////////////////////////////

#if 0
   // see documentation in devkit_picrobot.h
   #define USE_RS232_USB_CDC
   #define USE_RS232_USB_CDC_WAIT_FOR_CONNECTION
#endif

#include <devkit_picrobot.h>

#ifndef USE_RS232_USB_CDC
   #use rs232(ICD)
#endif

#include <mag3110.c>

#include <virtual_eeprom.c>
#define EE_LOC_MAG3110_CALIBRATION  0  //13 bytes are saved here

/*
Remove calibration from non-volatile EEPROM memory on the PIC, as well as the 
volatile offset calibrations on the MAG3110 chip.
*/
void calibrate_remove(void)
{
   printf("Erasing calibration.\r\n");

   Mag3110CalibrationRemove();
   
   write_virtual_eeprom(EE_LOC_MAG3110_CALIBRATION, 0);  //calibration not valid
}

/*
Load saved calibration data from the non-volatile EEPROM memory on the PIC and
write it back to the volatile offset calibration on the MAG3110 chip.
*/
void calibrate_load(void)
{
   int idx;
   mag3110_axis_t axisMax, axisMin;
   
   if (read_virtual_eeprom(EE_LOC_MAG3110_CALIBRATION) == 1)
   {
      unsigned int8 *p;
      
      printf("Loading calibration from EE.\r\n");
      
      p = &axisMax;
      for (idx=0; idx<sizeof(axisMax); idx++)
      {
         *p++ = read_virtual_eeprom(EE_LOC_MAG3110_CALIBRATION+1+idx);
      }
      
      p = &axisMin;
      for (idx=0; idx<sizeof(axisMin); idx++)
      {
         *p++ = read_virtual_eeprom(EE_LOC_MAG3110_CALIBRATION+1+sizeof(axisMax)+idx);
      }
      
      #ifdef _MAG3110_DO_DEBUG
      printf("LOAD MAX x=%ld y=%ld z=%ld\r\n", axisMax.x, axisMax.y, axisMax.z);
      printf("LOAD MIN x=%ld y=%ld z=%ld\r\n", axisMin.x, axisMin.y, axisMin.z);
      #endif
   
      Mag3110CalibrationSet(&axisMin, &axisMax);
   }
   else
   {
      printf("Calibration in EE is not valid, ignoring it.\r\n");

      Mag3110CalibrationRemove();
      
      #if 0
      printf("Writing some hard coded values into calibration!!!\r\n");
      
      axisMin.x=7622;
      axisMin.y=8449;
      axisMin.z=1041;
      
      axisMax.x=7931;
      axisMax.y=8660;
      axisMax.z=1167;
      
      Mag3110CalibrationSet(&axisMin, &axisMax);
      #endif
   }
}

/*
Calibrate the MAG3110 and then write it to the PIC's non-volatile EEPROM as
well as the MAG3110's volatile calibration registers.
Calibration is performed by finding minimum and maximum readings; while
calibration is running user needs to spin the unit in a circle.  Once spun
entire 360 degress, press button on board to stop and save calibration.
*/
void calibrate_do(void)
{
   mag3110_axis_t axisMax, axisMin;
   mag3110_axis_t curr;
   unsigned int8 error;
   int i, loc;
   unsigned int8 *p;
   unsigned int16 oldCtrl;
   
   do {
      delay_ms(1);
   } while (!Mag3110IsReady());
   
   error = Mag3110Read(&axisMax);
   if (error)
      return;
      
   memcpy(&axisMin, &axisMax, sizeof(axisMax));
   
   Mag3110Stop();
   
   oldCtrl = _g_Mag3110.ctrl;
   
   // clear DR bits for fastest rate
   // leave OS bits alone to keep same over sample ratio
   _g_Mag3110.ctrl &= ~(unsigned int16)0xE0;
   
   Mag3110Start();
   
   printf("Starting MAG3110 calibration.  Rotate the device slowly, press button when gone a full circle...\r\n");
   
   while(!BUTTON_PRESSED())
   {
      do {
         delay_ms(1);
      } while (!Mag3110IsReady());
      
      error = Mag3110Read(&curr);
      if (error)
         return;
      
      if (curr.x > axisMax.x)
         axisMax.x = curr.x;
      if (curr.y > axisMax.y)
         axisMax.y = curr.y;
      if (curr.z > axisMax.z)
         axisMax.z = curr.z;
         
      if (curr.x < axisMin.x)
         axisMin.x = curr.x;
      if (curr.y < axisMin.y)
         axisMin.z = curr.z;
      if (curr.z < axisMin.z)
         axisMin.z = curr.z;
   }
   
   Mag3110Stop();
   
   #ifdef _MAG3110_DO_DEBUG
   printf("FOUND MAX x=%ld y=%ld z=%ld\r\n", axisMax.x, axisMax.y, axisMax.z);
   printf("FOUND MIN x=%ld y=%ld z=%ld\r\n", axisMin.x, axisMin.y, axisMin.z);
   #endif
  
   write_virtual_eeprom(EE_LOC_MAG3110_CALIBRATION, 1);  //valid calibration
   loc = EE_LOC_MAG3110_CALIBRATION+1;
   p = &axisMax;
   for(i = 0; i < sizeof(axisMax); i++)
   {
      write_virtual_eeprom(loc++, *p++);
   }
   p = &axisMin;
   for(i = 0; i < sizeof(axisMin); i++)
   {
      write_virtual_eeprom(loc++, *p++);
   }
   
   printf("Calibration saved to EE.\r\n");
   
   while(BUTTON_PRESSED());
   delay_ms(144);
   
   _g_Mag3110.ctrl = oldCtrl;

   Mag3110Start();
}

void main(void) {
   int ms = 0;
   int1 doRead = 0;
   mag3110_axis_t axis;
   unsigned int16 heading;
   
   HW_INIT();
   
   init_virtual_eeprom();
   
   Mag3110Init();
   
   if (BUTTON_PRESSED())
   {
      while(BUTTON_PRESSED());
      delay_ms(144);
      calibrate_remove();
   }
   else
   {
      calibrate_load();
   }
   
   printf("Running.\r\n");
   
   for(;;) 
   {
      HW_TASK();
      
      if (BUTTON_PRESSED())
      {
         printf("MAG3110 calibration starting once you release the button...\r\n");
         while(BUTTON_PRESSED());
         delay_ms(144);
         calibrate_do();
         calibrate_load();
         printf("Done.\r\n");
      }
      
      if (doRead && Mag3110IsReady())
      {
         doRead = 0;
         ms = 0;

         Mag3110Read(&axis);
         
         heading = Mag3110CalculateHeading(&axis);
            
         printf("X=%ld, Y=%ld, Z=%ld heading=%ld\r\n", axis.x, axis.y, axis.z, heading);
      }

      if (++ms >= 250)
      {
         doRead = 1;
      }
      
      delay_ms(2);
   }
}
