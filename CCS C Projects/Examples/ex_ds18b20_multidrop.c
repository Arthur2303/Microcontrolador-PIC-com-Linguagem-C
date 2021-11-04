/////////////////////////////////////////////////////////////////////////
////                    EX_DS18B20_MULTIDROP.C                       ////
////                                                                 ////
////  This example program demonstrates using the DS18B20_MULTIDROP  ////
////  Driver to get temperature conversions from multiple DS18B20    ////
////  temperature sensors.                                           ////
////                                                                 ////
////  The define WAIT_FOR_CONVERSION can be used to select how the   ////
////  example is build.  If defined it uses the ds18b20_read_temp()  ////
////  function to read the temperature from the sensor, which starts ////
////  the conversion, waits for it to finish and returns the result  ////
////  as a float value in degrees C.  If not defined uses the        ////
////  ds18b20_start_conversion() function to start conversion, the   ////
////  ds18b20_done() function to periodically check if the           ////
////  conversion is finished, the ds18b20_get_conversion() function  ////
////  to get the results and the ds18b20_convert_temp() to convert   ////
////  the result to a float value in degrees C.                      ////
////                                                                 ////
////  This example is written for the PCH compiler but can be used   ////
////  with the PCB, PCM and PCD compiler as well.  Change the        ////
////  included device, #use delay(), #use rs232() and                ////
////  PIN_DS18B20_DATA define as needed for your hardware.           ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2020 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
////                                                                 ////
////                     http://www.ccsinfo.com                      ////
/////////////////////////////////////////////////////////////////////////

#include <18F45K22.h>
#fuses NOWDT
#use delay(crystal=16MHz)
#use rs232(UART1,baud=115200)

// The following define selects the pin to use as the data pin for the DS18B20
// sensors.  Must be defined before the DS18B20_multidrop.c driver is included
// to change the pin, the default pin is PIN_B5 if not defined.
#define PIN_DS18B20_DATA         PIN_B2

// The following define sets the max number of DS18B20 sensors driver can be
// used with.  Must be defined before the DS18B20_multidrop.c driver is included
// to change the number of sensor, the default value is 8 if not defined.
#define  MAX_DS18B20_SENSORS     8

#include <DS18B20_multidrop.c>

// The following define selects which functions are used to read the temperature
// from the DS18B20 sensors.
//#define WAIT_FOR_CONVERSION

void PrintSerialNumber(DS18B20_ROM_CODE ROMCode)
{
   uint8_t i;
   
   printf(" %X", ROMCode.FamilyCode);
   
   for(i=0;i<6;i++)
      printf(" %X", ROMCode.SerialNumber[i]);
   
   printf(" %X", ROMCode.CRC);
}

void main(void)
{
   DS18B20_ROM_CODE ROMCode[MAX_DS18B20_SENSORS];
   uint8_t Count;
   float Temp[MAX_DS18B20_SENSORS];
  #ifndef WAIT_FOR_CONVERSION
   int16_t Reading;
  #endif
   uint8_t i;

   port_b_pullups(0b00000100);      // Enable the internal pullup on PIN_B2, this is only needed if an external
                                    // pullup resistor is not use
   
   delay_ms(100);
   
   printf("ex_ds18b20_multidrop.c - %s\r\n", getenv("DEVICE"));
   
   ds18b20_init();
   
   Count = ds18b20_search_rom(ROMCode);   // Determine how many DS18B20 sensor are connected
   
   printf("Sensors - %u\r\n", Count);
   
   if(Count > 0)
   {
      for(i=0;i<Count;i++) {
         printf("   %u) SN:", i);
         PrintSerialNumber(ROMCode[i]);
         printf("\r\n");
      }
      
      printf("\r\n");
   }
   
   while(TRUE)
   {
      if(Count > 0)
      {
        #ifdef WAIT_FOR_CONVERSION
          ds18b20_read_temp(Temp, ROMCode, Count);    // Start conversion and read results to Temp for all sensors
        #else
         int1 Converting;
        
         ds18b20_start_conversion(NULL);  // Start conversion on all sensors
         
         Converting = FALSE;
         
         while(!Converting)      // While conversions are occurring
         {
           if(ds18b20_done())    // Check if conversions are finished
           {
               Converting = 1;
           }
            
            output_toggle(PIN_B5);  // Toggle pin, Red LED on PIC18F45K22 development kit
            delay_ms(100);
         }
         
         for(i = 0; i < Count; i++)
         {
            ds18b20_get_conversion(&Reading, &ROMCode[i]);  // Get conversion results from each sensor
            Temp[i] = ds18b20_convert_temp(Reading);        // Convert result to a float value in degrees C
         }
        #endif
         
         for(i = 0; i < Count; i++)    //Print temperature for each sensor
         {
            printf("%i) SN:", i);
            PrintSerialNumber(ROMCode[i]);
            printf(", Temperature: %0.4f C\r\n", Temp[i]);
         }
         
         printf("\r\n");
      }
   }
}
