///////////////////////////////////////////////////////////////////////////
////                        ex_adc_calibration.c                       ////
////                                                                   ////
////  Example program showing how to implement a two point calibration ////
////  on an ADC channel. This program will read continuously read the  ////
////  voltage from Pot A0. The value for the voltage is calculated     ////
////  based on the adc calibration and is done using only integer      ////
////  math.                                                            ////
////                                                                   ////
////  Change the device, clock, rs232, adc channel and settings, and   ////
////  calibration storage method as needed for the hardware being      ////
////  used.                                                            ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2021 Custom Computer Services           ////
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

#include <18F45K22.h>
#fuses NOWDT
#device ADC=10
#use delay(clock=64M,crystal=16MHz)

#use rs232(xmit=PIN_C6, rcv=PIN_C7, baud=9600)

#include <internal_eeprom.c>

#define EE_ADDRESS   INT_EEPROM_ADDRESS

#case

#define ADC_CHANNELS             1

#define EE_ADC_CAL_SLOPE_SIZE    sizeof(unsigned int16)
#define EE_ADC_CAL_OFFSET_SIZE   sizeof(unsigned int16)

#define EE_ADDR_ADC_CAL_SLOPE    0  //size - EE_ADD_CAL_SLOPE_SIZE * ADC_CHANNELS
#define EE_ADDR_ADC_CAL_OFFSET   (EE_ADDR_ADC_CAL_SLOPE + (ADC_CHANNELS * EE_ADC_CAL_SLOPE_SIZE))  //size - EE_ADC_CAL_OFFSET_SIZE * ADC_CHANNELS

#define DEFAULT_HIGH_VOLTAGE     5000 //in mV (5.000V)

#include <tick.c>
#define TickDifference(a,b)      (a-b)

#include <stdlib.h>
#include <input.c>

typedef struct
{
   unsigned int16 Slope;
   unsigned int16 Offset;
} calibration_values_t;

calibration_values_t g_ADCCalibration[ADC_CHANNELS];

typedef struct
{
   unsigned int16 Low;
   unsigned int16 High;
} calibrate_points_t;

//////////////////////////// Calibration Functions ////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// InitADCCalibration()
//
// Function to initialize the ADC calibration values.
//
// Parameters - None.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void InitADCCalibration(void)
{
   unsigned int8 i;
   EE_ADDRESS Address;
   unsigned int16 Value;
   
   for(i=0;i<ADC_CHANNELS;i++)
   {
      Address = EE_ADDR_ADC_CAL_SLOPE + (i * EE_ADC_CAL_SLOPE_SIZE);
      
      //Read current calibration slope
      Value = read_int16_eeprom(Address);
      
      if(Value == 0xFFFF) //No calibration offset has been saved, set to default ((DEFAULT_HIGH_VOLTAGE  * 1000) / 1023)
      {
         g_ADCCalibration[i].Slope = ((DEFAULT_HIGH_VOLTAGE  * 1000) / 1023);
         
         write_int16_eeprom(Address, ((DEFAULT_HIGH_VOLTAGE  * 1000) / 1023));
      }
      else
         g_ADCCalibration[i].Slope = Value;
      
      Address = EE_ADDR_ADC_CAL_OFFSET + (i * EE_ADC_CAL_OFFSET_SIZE);
      
      //Read current calibration offset
      Value = read_int16_eeprom(Address);
      
      if(Value == 0xFFFF) //No calibration offset has been saved, set to default (0)
      {
         g_ADCCalibration[i].Offset = 0;
         
         write_int16_eeprom(Address, 0);
      }
      else
         g_ADCCalibration[i].Offset = Value;
   }
}

///////////////////////////////////////////////////////////////////////////////
// SaveCalibration()
//
// Function to save them for the ADC channel for the specified index.  Requires
// that the structure g_ADCCalibration to already be populated.
//
// Parameters:
//    Ch - the ADC channel to save the calibration values for.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void SaveCalibration(unsigned int8 Ch)
{
   //Write value to nonvolatile memory so the value aren't lost when power is removed
   write_int16_eeprom(EE_ADDR_ADC_CAL_SLOPE + (Ch * EE_ADC_CAL_SLOPE_SIZE), g_ADCCalibration[Ch].Slope);
   write_int16_eeprom(EE_ADDR_ADC_CAL_OFFSET + (Ch * EE_ADC_CAL_OFFSET_SIZE), g_ADCCalibration[Ch].Offset);
}

///////////////////////////////////////////////////////////////////////////////
// CalibrationRoutine()
//
// Function to set the calibration for the specified ADC Channel.
//
// Parmeters:
//    Ch - the ADC channel to do the calibration on.
//
// Returns - Nothing.
///////////////////////////////////////////////////////////////////////////////
void CalibrationRoutine(unsigned int8 Ch)
{
   unsigned int16 MaxVoltage;
   calibrate_points_t Calibration;
   char c;
   
   printf("\r\nSet Voltage to 0V, Press Any Key when ready");
            
   c = '\0';
   
   do
   {
      if(kbhit())
         c = getc();
   } while(c == '\0');
   
   Calibration.Low = read_adc();
   
   printf("\r\nEnter Max Voltage Reading (mV): ");
   MaxVoltage = get_Int16();
   
   printf("\r\nSet Voltage to %lu.%03luV, Press Any Key when ready", MaxVoltage / 1000, MaxVoltage % 1000);
   
   c = '\0';
   
   do
   {
      if(kbhit())
         c = getc();
   } while(c == '\0');
   
   Calibration.High = read_adc();
   
   g_ADCCalibration[Ch].Offset = Calibration.Low;
   g_ADCCalibration[Ch].Slope = ((unsigned int32)MaxVoltage * 1000)  / (Calibration.High - Calibration.Low);
   
   SaveCalibration(Ch);
   
   printf("\r\nADC Calibration Completed\r\n");
}

///////////////////////////////////////////////////////////////////////////////

void main(void)
{
   unsigned int16 Reading;
   unsigned int32 Voltage;
   TICK cTick, ADCTick;
   char c;
   
   InitADCCalibration();
   
   //Setup ADC peripheral
   setup_adc_ports(sAN0, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);
   
   ADCTick = TickGet() - (TICKS_PER_SECOND / 4);
   
   while(TRUE)
   {
      if(kbhit())
      {
         c = toupper(getc());
         
         if(c == 'C')
         {
            //Run Calibration Routine on ADC CH 0
            CalibrationRoutine(0);  
         }
         else if(c == 'P')
         {
            //Display Current Calibration Values for ADC CH 0
            printf("\r\nCurrent ADC Calibration - Slope: %lu, Offset: %lu\r\n", g_ADCCalibration[0].Slope, g_ADCCalibration[0].Offset);
         }
      }
      
      cTick = TickGet();
      
      if(TickDifference(cTick, ADCTick) >= (TICKS_PER_SECOND / 2))
      {
         Reading = read_adc();
         Voltage = (((unsigned int32)Reading * g_ADCCalibration[0].Slope) / 1000) + g_ADCCalibration[0].Offset;
         
         printf("Reading: %lu, Current Voltage: %5.3w\r\n", Reading, Voltage);
         
         ADCTick = cTick;
      }
   }
}
