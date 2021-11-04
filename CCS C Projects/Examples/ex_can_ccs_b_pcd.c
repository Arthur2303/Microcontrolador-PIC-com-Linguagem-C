/////////////////////////////////////////////////////////////////////////
////                       EX_CAN_CCS_B_PCD.C                        ////
////                                                                 ////
//// Example of CCS's MCP2515 CAN library.  This example was tested  ////
//// with and written for the CCS CAN Bus 24 prototype board.        ////
////                                                                 ////
//// This example provides the firmware for the B node on the CCS    ////
//// CAN Bus prototyping board.  Node B responds to CAN ID 0x202 by  ////
//// setting the 3 LEDs to the value transmitted by Node A.  Node B  ////
//// also responds to requests from CAN ID 0x201 by transmitting an  ////
//// A/D reading.                                                    ////
////                                                                 ////
//// Using a serial port you can see messages about what the         ////
//// firmware is doing.                                              ////
////                                                                 ////
//// Select either the ICD or the UART1 peripheral for the RS232 to  ////
//// PC communication by commenting out our uncommenting the define  ////
//// USE_ICD_FOR_RS232.                                              ////                                              ////
////                                                                 ////
//// For more documentation on the MPC2515 CAN library, see          ////
//// can-mcp2515.h.                                                  ////
////                                                                 ////
//// For more documentation on the CCS CAN Bus 24 Prototype boards   ////
//// see ex_can_ccs_a_pcd.c                                          ////
////                                                                 ////
//// This example will work with the PCD compiler.                   ////
/////////////////////////////////////////////////////////////////////////
////         (C) Copyright 1996,2019 Custom Computer Services        ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
////                      http://www.ccsinfo.com                     ////
/////////////////////////////////////////////////////////////////////////

//#define USE_ICD_FOR_RS232

#include <30F4012.h>
#device ADC=8
#fuses NOWDT, NOBROWNOUT
#use delay(internal=117.92MHz)

#ifdef USE_ICD_FOR_RS232
 #use rs232(ICD)                 //Text though ICD
#else
 #use rs232(UART1A, baud=38400)  //Text through the UART
#endif

#define PIN_LED1    PIN_E1
#define PIN_LED2    PIN_E2
#define PIN_LED3    PIN_E4

#case

#include <can-mcp2515.c>

#define LED1_HIGH output_low(PIN_LED1)
#define LED1_LOW  output_high(PIN_LED1)
#define LED2_HIGH output_low(PIN_LED2)
#define LED2_LOW  output_high(PIN_LED2)
#define LED3_HIGH output_low(PIN_LED3)
#define LED3_LOW  output_high(PIN_LED3)

#define RESPOND_TO_ID_AD   0x201
#define RESPOND_TO_ID_LED  0x202

void main(void)
{
   CAN_RX_HEADER rHeader;
   CAN_TX_HEADER tHeader;
   uint8_t Buffer[8];
   uint8_t i;
   
   setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_31);
   setup_adc_ports(sAN5, VSS_VDD);
   set_adc_channel(5);
   
   enable_interrupts(GLOBAL);

   for(i=0;i<8;i++) {
      Buffer[i]=0;
   }

   LED1_HIGH;
   LED2_HIGH;
   LED3_HIGH;
   printf("\rCCS CAN EXAMPLE\r\n\n");
   delay_ms(1000);
   LED1_LOW;
   LED2_LOW;
   LED3_LOW;
   
   can_init();

   printf("Running...\r\n\n");

   while(TRUE)
   {
      if(can_kbhit())   //if data is waiting in buffer...
      {
         if(can_getd(&rHeader, Buffer) == CAN_EC_OK) //...then get data from buffer
         {
            if(rHeader.Id == RESPOND_TO_ID_LED)
            {
               printf("Chaining LEDs\r\n\n");
               if (bit_test(Buffer[0],0)) {LED1_HIGH;} else {LED1_LOW;}
               if (bit_test(Buffer[0],1)) {LED2_HIGH;} else {LED2_LOW;}
               if (bit_test(Buffer[0],2)) {LED3_HIGH;} else {LED3_LOW;}
            }
            if (rHeader.Id ==  RESPOND_TO_ID_AD)
            {
               tHeader.Id = RESPOND_TO_ID_AD;
               tHeader.Length = 1;
               tHeader.ext = TRUE;
               tHeader.rtr = FALSE;
               tHeader.Priority = 1;
               
               i = read_adc();
               
               printf("Sending AD reading: %X\r\n\n",i);
               can_putd(&tHeader, &i);    //put data on transmit buffer
            }
         }
      }
   }
}
