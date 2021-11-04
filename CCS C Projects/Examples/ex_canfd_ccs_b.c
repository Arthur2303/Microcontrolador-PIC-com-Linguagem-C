/////////////////////////////////////////////////////////////////////////
////                         EX_CANFD_CCS_B.c                        ////
////                                                                 ////
//// Example of CCS's MCP2517FD CAN FD driver. This example was      ////
//// tested with and written for the CCS CAN Bus FD prototype board. ////
////                                                                 ////
//// This example provides the firmware for Node B on the CCS CAN    ////
//// Bus FD prototype board.  Node B responds to CAN ID 0x202 by     ////
//// setting the 3 LEDs to the value transmitted by Node A.  Node B  ////
//// also responds to requests from CAN ID 0x201 by transmitting an  ////
//// ADC reading.                                                    ////
////                                                                 ////
//// Using a serial port you can see message about what the firmware ////
//// is doing.  Select either the ICD or the UART1 peripheral for    ////
//// the serial communication to the PC by commenting or             ////
//// uncommenting the define USE_ICD_FOR_RS232.                      ////
////                                                                 ////
//// For more documentation on the MCP2517FD CAN FD driver see       ////
//// can-mcp2517.h.                                                  ////
////                                                                 ////
//// For more documentation on the CCS CAN Bus FD prototype board    ////
//// see ex_canfd_ccs_a.c.                                           ////
////                                                                 ////
//// This example will work with the PCM compiler.                   ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////         (C) Copyright 1996,2020 Custom Computer Services        ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
////                      http://www.ccsinfo.com                     ////
/////////////////////////////////////////////////////////////////////////

//#define USE_ICD_FOR_RS232

#include <16F18346.h>
#fuses NOWDT
#device ADC=10
#use delay(internal=32MHz)
#ifdef USE_ICD_FOR_RS232
 #use rs232(ICD)
#else
 #pin_select U1TX=PIN_B6
 #pin_select U1RX=PIN_B7
 
 #use rs232(xmit=PIN_B6, rcv=PIN_B7, baud=115200)
#endif

#define MCP2517_SPI_SCK_PIN   PIN_C0
#define MCP2517_SPI_SDO_PIN   PIN_C2
#define MCP2517_SPI_SDI_PIN   PIN_C1
#define MCP2517_SPI_CS_PIN    PIN_B4

#pin_select SCK1OUT=MCP2517_SPI_SCK_PIN
#pin_select SDO1=MCP2517_SPI_SDI_PIN
#pin_select SDI1=MCP2517_SPI_SDO_PIN

#define CAN_TX_BUFFERS     4

#include <can-mcp2517.c>

#define LED_RED            PIN_C3
#define LED_YELLOW         PIN_C4
#define LED_GREEN          PIN_C5

#define LED1_HIGH    output_high(LED_GREEN)
#define LED1_LOW     output_low(LED_GREEN)
#define LED2_HIGH    output_high(LED_YELLOW)
#define LED2_LOW     output_low(LED_YELLOW)
#define LED3_HIGH    output_high(LED_RED)
#define LED3_LOW     output_low(LED_RED)

#define RESPOND_TO_ID_AD   0x201
#define RESPOND_TO_ID_LED  0x202

void main(void)
{
   CAN_RX_HEADER rHeader;
   CAN_TX_HEADER tHeader;
   uint8_t Buffer[8];
   uint16_t aReading;
   
   setup_adc_ports(sAN2, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(2);
   
   LED1_LOW;
   LED2_LOW;
   LED3_LOW;
   delay_ms(1000);
   printf("\rCCS CAN FD Example Node B\r\n\n");
   LED1_HIGH;
   LED2_HIGH;
   LED3_HIGH;
   
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
               if(bit_test(Buffer[0], 0)) {LED1_LOW;} else {LED1_HIGH;}
               if(bit_test(Buffer[0], 1)) {LED2_LOW;} else {LED2_HIGH;}
               if(bit_test(Buffer[0], 2)) {LED3_LOW;} else {LED3_HIGH;}
            }
            else if(rHeader.Id == RESPOND_TO_ID_AD)
            {
               tHeader.Id = RESPOND_TO_ID_AD;
               tHeader.Length = 2;
               tHeader.ext = TRUE;
               tHeader.rtr = FALSE;
               tHeader.Format = CAN_MESSAGE_FORMAT_CAN_FD;
               
               aReading = read_adc();
               
               printf("Sending ADC Reading: %LX\r\n\n", aReading);
               can_putd(&tHeader, (uint8_t *)&aReading);
            }
         }
      }
   }
}
