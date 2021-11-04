///////////////////////////////////////////////////////////////////////////
////                              ex_hspwm.c                           ////
////                                                                   ////
////  This program is a simple example showing how to setup and use    ////
////  the High-Speed PWM peripheral (HSPWM) using the compiler's       ////
////  built-in functions.  This program takes an analog input and uses ////
////  the digital value to set the duty cycle. The frequency is set by ////
////  the user over the RS-232.                                        ////
////                                                                   ////
////  Select either the ICD or your own RS232-to-PC connection         ////
////  for the text I/O.                                                ////
////                                                                   ////
////  This example will work with the PCD compiler on most devices     ////
////  with a HSPWM peripheral.  The following conditional compilation  ////
////  lines are used to include a valid device for the compiler.       ////
////  Change the device, clock, ADC defines and RS232 pins for your    ////
////  hardware if needed.                                              ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2016,2018 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                                                                   ////
//// http://www.ccsinfo.com                                            ////
///////////////////////////////////////////////////////////////////////////

#include <33EP512MC502.h>
#device ADC=10
#use delay(internal=32MHz)

#define ADC_CHANNEL  0
#define ADC_PIN      sAN0
#define TX_PIN       PIN_B5
#define RCV_PIN      PIN_B6

#pin_select U1TX = TX_PIN
#pin_select U1RX = RCV_PIN

#use rs232(icd)                                   //Text through the ICD
//#use rs232(xmit=TX_PIN, rcv=RCV_PIN, baud=9600) //Text through the UART

unsigned int16 PwmPeriod;

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
         PwmPeriod = 800;
         break;
      case '2':
         PwmPeriod = 3200;
         break;
      case '3':
         PwmPeriod = 16000;
         break;
   }
   
   set_hspwm_phase(1, PwmPeriod - 1);  //Set the frequency of the PWM signal.
}

void main(void)
{
   char c;
   unsigned int16 value;
   
   delay_ms(1000);
   
   printf("\r\nex_hspwm.c - %s\r\n", getenv("DEVICE"));
   
   setup_adc_ports(ADC_PIN, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(ADC_CHANNEL);
   
   setup_hspwm_unit(1, HSPWM_ENABLE | HSPWM_REDUNDANT | HSPWM_TIME_BASE_FROM_PHASE_REGS | //Enable PWM1L and PWM1H with same signal and faults disabled.
                       HSPWM_FAULT_MODE_DISABLED);                                        //Frequency set with phase register, independent from Master Period register (PTPER).
                                                                                          //Duty cycle set with duty cycle register, independent from Master Duty Cycle register (MDC).
   setup_hspwm(HSPWM_ENABLED | HSPWM_CLOCK_DIV_BY_2, 0xFFFF);  //HSPWM enabled, PWM Clock = Fosc/2                             
   
   SetFrequency();
    
   while(TRUE)
   {
      value = read_adc();
      
      printf("%4LX\r",value);
      
      set_hspwm_duty(1, ((unsigned int32)value * PwmPeriod) / 1023); //Set duty of PWM signal.
      
      if(kbhit())
      {
         c = toupper(getc());
         
         if(c == 'F')
            SetFrequency();
      }
      
      delay_ms(10);
   }
}
