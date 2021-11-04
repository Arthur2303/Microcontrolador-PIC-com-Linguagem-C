/////////////////////////////////////////////////////////////////////////
////                           EX_PWM_A.C                            ////
////                                                                 ////
////  This program will show how to use the built-in #use pwm        ////
////  libarary to generate a pwm signal.  This program takes an      ////
////  analog input and uses the digital value to set the duty cycle. ////
////  The frequency is set by the user over the RS-232.              ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCM, PCH and PCD compilers.    ////
////  The following conditional compilation lines are used to        ////
////  include a valid device for each compiler.  Change the device,  ////
////  clock and RS232 pins for your hardware if needed.              ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2016,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F887.h>
#elif defined(__PCH__)
#include <18F4520.h>
#elif defined(__PCD__)
#include <30F2010.h>
#endif
#device ADC=8
#use delay(crystal=20MHz)

#define TX_PIN    PIN_C6
#define RCV_PIN   PIN_C7

#define PWM_PIN   PIN_C2

#define ADC_CHANNEL  0
#define ADC_PIN      sAN0


#use rs232(icd)                                   //Text through the ICD
//#use rs232(xmit=TX_PIN, rcv=RCV_PIN, baud=9600) //Text through the UART
#use pwm(output=PWM_PIN, frequency=20kHz)

void SetFrequency(void)
{
   char selection;

   printf("\r\nFrequency:\r\n");
   printf("    1) 20 khz\r\n");
   printf("    2) 5 khz\r\n");
   printf("    3) 1 khz\r\n");

   do {
     selection=getc();
   } while((selection<'1')||(selection>'3'));
   
   printf("%c\r\n\n", selection);
   
   switch(selection)
   {
      case '1':
         pwm_set_frequency(20000);
         break;
      case '2':
         pwm_set_frequency(5000);
         break;
      case '3':
         pwm_set_frequency(1000);
         break;
   }
}

void main(void)
{
   char c;
   unsigned int8 value;
   
   setup_adc_ports(ADC_PIN, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(ADC_CHANNEL);
   
   while(TRUE)
   {
      value = read_adc();
      
      printf("%2X\r",value);
      
      pwm_set_duty_percent(((unsigned int32)value*1000) / 256);
      
      if(kbhit())
      {
         c = toupper(getc());
         
         if(c == 'F')
            SetFrequency();
      }
      
      delay_ms(10);
   }
}
