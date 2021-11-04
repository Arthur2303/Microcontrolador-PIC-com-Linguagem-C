/////////////////////////////////////////////////////////////////////////
////                         EX_COMP.C                               ////
////                                                                 ////
////  This example demonstrates the use of the built in comparator.  ////
////  The program compares the input voltage with the internal       ////
////  reference voltage or DAC.  Turn the POT to change the voltage. ////
////                                                                 ////
////  If using CCS's PIC24EP256GP206 development kit for this        ////
////  example to function correctly, jumper pins B1 and C11 together ////
////  using header block.                                            ////
////                                                                 ////
////  NOTE: Make sure the POT is turned all the way counter clock-   ////
////  wise before starting the program.                              ////
////                                                                 ////
////  This example will work with the PCM, PCH and PCD compilers.    ////
////  The following conditional compilation lines are used to        ////
////  include a valid device for each compiler.  Change the device,  ////
////  clock and RS232 pins for your hardware if needed.              ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2017 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
////                     http://www.ccsinfo.com                      ////
/////////////////////////////////////////////////////////////////////////


#if defined(__PCM__)
#include <12F683.h>
#use delay(internal=8MHz)
#use rs232(baud=9600, xmit=PIN_A5, rcv=PIN_A4)

#elif defined(__PCH__)
#include <18F45K22.h>
#use delay(crystal=16MHz)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#elif defined(__PCD__)
#include <24EP256GP206.h>
#use delay(crystal=20MHz)

#pin_select U1TX=PIN_A4
#pin_select U1RX=PIN_A9
#use rs232(baud=9600, UART1)
#endif

int1 safe_conditions = TRUE;

#INT_COMP
void isr(void)
{
   #if defined(__PCD__)
   C1ENV = 0;
   #endif
   
   if(C1OUT)
      safe_conditions = FALSE;
   else
      safe_conditions=TRUE;
}


void main(void)
{
   printf("\r\nRunning voltage test...\r\n\n");
   
   #if defined(__PCD__)
   setup_comparator(1, CXIN2_VREF | COMP_INTR_CHANGE | COMP_INVERT);
   setup_vref(VREF_VSS_VDD | VREF_HIGH | 14);   //~2.3V
   #elif defined(__PCH__)
   setup_comparator(CP1_A0_DAC | CP1_INVERT);
   setup_dac(DAC_VSS_VDD);
   dac_write(23); //~3.6V
   #else
   setup_comparator(A1_VR_OUT_ON_A2 | COMP_INVERT);
   setup_vref(VREF_HIGH | 15);   //~3.6V
   #endif
   
   enable_interrupts(INT_COMP);
   enable_interrupts(GLOBAL);

   while(TRUE)
   {
      if(safe_conditions)
      {
         #if defined(__PCD__)
         printf("Voltage level is below 2.3 V.\r\n");
         #else
         printf("Voltage level is below 3.6 V.\r\n");
         #endif
      }
      else
      {
         #if defined(__PCD__)
         printf("WARNING!!  Voltage level is above 2.3 V.\r\n");
         #else
         printf("WARNING!!  Voltage level is above 3.6 V.\r\n");
         #endif
      }
      
      delay_ms(500);
   }
}
