/////////////////////////////////////////////////////////////////////////
////                          ex_dmx_slave.c                         ////
////                                                                 ////
//// An example program showing how to use the DMX Driver to create  ////
//// a DMX Slave Device.  The DMX Driver requires the use of one of  ////
//// PIC's UART peripherals.                                         ////
////                                                                 ////
//// This is an example of DMX Slave device which uses two channels  ////
//// to set the duty cycle of a PWM signal, starting at channel 1.   ////
//// This example can be used in conjunction with                    ////
//// ex_dmx_controller.c to demonstrate its functionality.           ////
////                                                                 ////
//// Select either the ICD or your own RS232-to-PC connection        ////
//// for the text I/O.                                               ////
////                                                                 ////
//// This example will work on the PCM, PCH and PCD compiler.  The   ////
//// following conditional compilation lines have been made.  Change ////
//// the device, clock, rs232, etc. as need for your hardware        ////
//// configuration.                                                  ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2016,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCH__)
#include <18F45K22.h>
#elif defined(__PCM__)
#include <16F887.h>
#elif defined(__PCD__)
#include <30F2010.h>
#endif

#use delay(clock=64MHz,crystal=16MHz)
#use rs232(icd)                          //Text through the ICD
//#use rs232(UART1,baud=9600, stream=PC) //Text through the UART

#use pwm(CCP1, timer=2, frequency=5000, duty=0)

//Setup Tick timer for DMX Driver
#use timer(timer=1, tick=1us, bits=16)

typedef unsigned int16 TICK;

TICK GetTickDifference(TICK Current, TICK Previous)
{
   return(Current - Previous);
}

//Required Macros for DMX Driver when using it for a slave device
#define DMX_TICK                    TICK
#define DMXGetTick()                get_ticks()
#define DMXGetTickDifference(x, y)  GetTickDifference(x, y)
#define DMX_TICKS_PER_MS            (TICKS_PER_SECOND  / 1000)

//Setup DMX Driver Settings
#define DMX_TYPE           DMX_SLAVE_DEVICE
#define DMX_INT            INT_RDA2
#define DMX_RCV            PIN_D7
#define DMX_START_CHANNEL  1
#define DMX_CHANNELS       2  //16 bit LSB first

//Include the DMX Driver
#include <dmx.c>

void main()
{
   unsigned int16 PWMDuty;
   unsigned int16 pPWMDuty = 0;
   
   delay_ms(100);
   
   printf("\r\nex_dmx_slave.c\r\n");
   
   //Initialize the DMX Driver
   DMXInit();
   
   //Enable Global Interrupts
   enable_interrupts(GLOBAL);
   
   while(TRUE)
   {
      //Get for new DMX Data
      if(DMXKbhit())
      {
         //Get new DMX Data
         DMXGetd(&PWMDuty, DMX_CHANNELS);
         
         if(PWMDuty != pPWMDuty)
         {
            if(PWMDuty > 1000)
               PWMDuty = 1000;
               
            pwm_set_duty_percent(PWMDuty);
            
            pPWMDuty = PWMDuty;
         }
      }
   }
}
