///////////////////////////////////////////////////////////////////////////////
//                               ex_adc_epgs.c                               //
//                                                                           //
// Example showing how to setup and use the ADC peripheral on the            //
// dsPIC33EPxxGS50x family of devices.  This family has a different type of  //
// ADC them most other dsPIC33 devices, including 4 dedicated ADC cores and  //
// a shared ADC core.  Dedicated ADC Core 0 can be used to read channels     //
// AN0, AN7, PGA1 or AN0ALT, dedicated ADC Core 1 can be used to read        //
// channels AN1, AN18, PGA2 or AN1ALT, dedicated ADC Core 2 can be used to   //
// read channels AN2, AN11 or VBG and dedicated ADC Core 3 can be used to    //
// read channels AN3 or AN15.  The shared ADC core can be used to read       //
// channels AN4 to AN21.                                                     //
//                                                                           //
// This example will show how to setup the ADC to read three channels use    //
// both the dedicated ADC cores and the shared ADC core.  The readings will  //
// be taken once a second and the raw reading will be displayed using a      //
// RS232 connection.                                                         //
//                                                                           //
// This example will work with the PCD compiler on devices in the            //
// dsPIC33EPxxGS50x, dsPIC33EPxxxGS70x and dsPIC33EPxxxGS80x families.  The  //
// following conditional compilation lines are used to include a valid       //
// device.  Change the device, clock, RS232 pins, and ADC settings as need   //
// for your hardware configuration.                                          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//              (C) Copyright 1996,2017 Custom Computer Services             //
// This source code may only be used by licensed users of the CCS C          //
// compiler.  This source code may only be distributed to other licensed     //
// users of the CCS C compiler.  No other use,reproduction or distribution   //
// is permitted without written permission.  Derivative programs created     //
// using this software in object code form are not restricted in any way.    //
//                           http://www.ccsinfo.com                          //
///////////////////////////////////////////////////////////////////////////////

#include <33EP16GS506.h>
#device ADC=12
#fuses NOWDT
#use delay(internal=7.37MHz)

#define TX_PIN    PIN_B4
#define RX_PIN    PIN_C14

#pin_select U1TX=TX_PIN
#pin_select U1RX=RX_PIN

#use rs232(xmit=TX_PIN, rcv=RX_PIN, baud=9600)

#define TICK_USES_ISR
#include <tick.c>
#define GetTickDifference(a,b)   (a-b)

void main(void)
{
   unsigned int16 ADCResult[3];
   TICK cTick, pTick;
   
   enable_interrupts(GLOBAL);
   
   printf("\r\nex_adc_epgs.c - %s\r\n\n", getenv("DEVICE"));
   
   //Make pins A0, B0 and D13 analog pins, set adc reference to Vss and Vdd.
   setup_adc_ports(sAN0 | sAN3 | sAN13, VSS_VDD);
   
   //Enable ADC setup ADC clock, and enable shared ADC core and set shared core clock divisor and sample time.
   setup_adc(ADC_CLOCK_SYSTEM | ADC_CLOCK_DIV_1 | ADC_SHARED_CLOCK_DIV_2 | 
             ADC_SHARED_TAD_MUL_33 | ADC_ENABLE_SHARED_CORE);
   
   //Enable dedicated ADC core 0 and set core clock divisor.
   setup_dedicated_adc(0, ADC_DEDICATED_CLOCK_DIV_2);
   
   //Enable dedicated ADC core 3 and set core clock divisor.
   setup_dedicated_adc(3, ADC_DEDICATED_CLOCK_DIV_2);
   
   //Set dedicated ADC core 0 channel.
   set_dedicated_adc_channel(0, ADC_CHANNEL_AN0);
   
   //Set dedicated ADC core 3 channel.
   set_dedicated_adc_channel(3, ADC_CHANNEL_AN3);
   
   pTick = TickGet();
   
   while(TRUE)
   {
      cTick = TickGet();
      
      if(GetTickDifference(cTick, pTick) >= TICKS_PER_SECOND)
      {
         //Set ADC channel to 13, pin D13, and read channel.
         set_adc_channel(13);
         ADCResult[0] = read_adc();
         
         //Set ADC channel to 0, pin A0, and read channel.
         set_adc_channel(0);
         ADCResult[1] = read_adc();
         
         //Set ADC channel to 3, pin B0, and read channel.
         set_adc_channel(3);
         ADCResult[2] = read_adc();
         
         printf("\rADC Readings - CH13: %04LX", ADCResult[0]);
         printf(", CH0: %04LX", ADCResult[1]);
         printf(", CH3: %04LX", ADCResult[2]);
         
         pTick = cTick;
      }
   }
}
