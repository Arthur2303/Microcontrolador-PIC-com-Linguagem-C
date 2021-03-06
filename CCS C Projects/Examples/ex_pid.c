/////////////////////////////////////////////////////////////////////////
////                            EX_PID.C                             ////
////                                                                 ////
////  This example shows how to setup and use the PID module that    ////
////  the PIC16F161x family of devices have.  This example uses the  ////
////  ADC to read the current output voltage of a RC circuit, then   ////
////  inputa the reading to the PID controller and use the PID       ////
////  result to set the duty cycle of PWM3 to adjust the output of   ////
////  the RC circuit.  The desired output of the RC circuit can be   ////
////  set by the user over the RS232.                                ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCM compiler.  A 33 KOHM       ////
////  resistor and 10 uF capacitor were use for the RC circuit.      ////
////  Change the device, clock, RS232 pins, PWM3 pin, and ADC        ////
////  settings as needed for your hardware configuration.            ////
/////////////////////////////////////////////////////////////////////////
////    (C) Copyright 1996,2015,2018 Custom Computer Services        ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////
#include <16F1618.h>
#device ADC=10
#use delay(internal=32MHz)

#pin_select U1TX=PIN_B6
#pin_select U1RX=PIN_B7
#use rs232(icd)                 //Text through the ICD
//#use rs232(UART1,baud=115200) //Text through the UART

#pin_select PWM3=PIN_C7

#include <stdlib.h>
#include <input.c>

#use timer(timer=2, tick=1ms, bits=16, ISR)

//Set the sample frequency
#define SAMPLE_FREQ  20 //50ms sample period

//Set the PID Kp, Ki and Kd values
#define KP_VALUE  6
#define KI_VALUE  20
#define KD_VALUE  0

unsigned int16 GetTickDifference(unsigned int16 Current, unsigned int16 Previous)
{
   return(Current-Previous);
}

void CalculateKValues(int16 Kp, int16 Ki, int16 Kd, int16 &K1, int16 &K2, int16 &K3)
{
   K1 = Kp + Ki/SAMPLE_FREQ + Kd*SAMPLE_FREQ;
   K2 = -(Kp + (2*Kd)*SAMPLE_FREQ);
   K3 = Kd * SAMPLE_FREQ;
}

void main()
{
   char c;
   pid_struct_t PIDOutput;
   unsigned int16 ADCReading;
   unsigned int16 CurrentTick, PIDTick;
   unsigned int16 SetPoint;
   signed int16 K1, K2, K3;
   unsigned int16 PWMDuty;
   
   //100 ms delay for PLL to stablize
   delay_ms(100);
   
   enable_interrupts(GLOBAL);
      
   //Calculate PID Module K1, K2 and K3 values
   CalculateKValues(KP_VALUE, KI_VALUE, KD_VALUE, K1, K2, K3);
   
   printf("\r\nK1=%ld, K2=%ld, K3=%ld", K1, K2, K3);
   
   //Setup ADC 
   setup_adc_ports(sAN3, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(3);
   
   //Setup the PID module for PID mode and set the K1, K2 and K3 values
   setup_pid(PID_MODE_PID, K1, K2, K3);
   
   //Set the initial PID set point
   SetPoint = 500;   //~2.5 V
   
   //Setup PWM 3 to generate a 1 kHz signal
   setup_timer_4(T4_CLK_INTERNAL | T4_DIV_BY_32, 249, 1);   //1ms period
   set_pwm3_duty(0); //0% duty
   setup_pwm3(PWM_ENABLED | PWM_OUTPUT | PWM_TIMER4);
   
   CurrentTick = PIDTick = get_ticks();
   
   while(TRUE)
   {
      CurrentTick = get_ticks();
      
      if(GetTickDifference(CurrentTick, PIDTick) >= ((unsigned int16)TICKS_PER_SECOND / SAMPLE_FREQ))
      {
         ADCReading = read_adc();
         
         pid_get_result(SetPoint, ADCReading, &PIDOutput);
         
         PIDOutput.u &= 0x07;
         
         if(PIDOutput.u >= 4)          //PIDOutput is negative, set PWMDuty to Minimum
            PWMDuty = 0;         
         else if(PIDOutput.u != 0)     //PIDOutput > Maximum, set PWMDuty to Maximum
            PWMDuty = 1000;
         else if(PIDOutput.l > 1000)   //PIDOutput > Maximum, set PWMDuty to Maximum
            PWMDuty = 1000;
         else
            PWMDuty = PIDOutput.l;
       
         set_pwm3_duty(PWMDuty);
         
         PIDTick = CurrentTick;
      }
      
      if(kbhit())
      {
         c = toupper(getc());
         
         if(c == 'S')
         {
            printf("\r\nNew Set Point (0-5V): ");
            
            SetPoint = (unsigned int16)(get_float() * 1000) / 5;
         }
      }
   }
}  
