/////////////////////////////////////////////////////////////////////////
////                       EX_CAN_CCS_A_PCD.C                        ////
////                                                                 ////
//// Example of CCS's CAN libraries, using the PIC24HJ256GP610.      ////
//// This example was tested with and written for the CCS CAN Bus 24 ////
//// prototype board.                                                ////
////                                                                 ////
//// The CCS CAN Bus 24 prototype board has four CAN nodes that      ////
//// communicate with each other.  Node A has a PIC24HJ256GP610 on   ////
//// it with an ECAN peripheral to connect to the CAN Bus, Node B    ////
//// This a dsPIC30F4012 using an MCP2515 CAN controller to connect  ////
//// to the CAN Bus, and Node C and Node D both have MCP250xx        ////
//// stand-alone CAN I/O expanders to connect to the CAN Bus.  This  ////
//// example is the firmware for Node A.                             ////
////                                                                 ////
//// Every two seconds this firmware sends out a command to node B   ////
//// to change it's LEDs (CAN ID 0x202).                             ////
////                                                                 ////
//// Upon change of the A/D reading, a value of 0-9 is sent to       ////
//// Node D which is displayed on the 7-segment LED display (CAN ID  ////
//// 0x400).                                                         ////
////                                                                 ////
//// Pressing the Node A button sends a request to Node B (CAN ID    ////
//// 0x201) for Node B's A/D reading, which Node B will respond      ////
//// with a CAN message with it's A/D reading (with CAN ID 0x201).   ////
//// Also, pressing the Node A button will change the LEDs on Node   ////
//// C (CAN ID 0x300).                                               ////
////                                                                 ////
//// Pressing one of Node C's push buttons will cause Node A's       ////
//// corresponding LED to turn on, firmware is written such that     ////
//// one Node A LED can be on at a time (Node C transmits push       ////
//// button presses with CAN ID 0x303).                              ////
////                                                                 ////
//// Using a serial port you can see messages about what the         ////
//// firmware is doing.                                              ////
////                                                                 ////
//// Select either the ICD or the UART1 peripheral for the RS232 to  ////
//// PC communication by commenting out our uncommenting the define  ////
//// USE_ICD_FOR_RS232.                                              ////
////                                                                 ////
//// For more documentation on the CCS CAN library, see              ////
//// can-PIC24_dsPIC33.h                                             ////
////                                                                 ////
//// This example will work with the PCD compiler.                   ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// Node C and D are sperate stand-alone MCP250xx CAN I/O           ////
//// expanders.  The CCS CAN Prototype board has these chips already ////
//// programmed correctly.  However, if you wish to program your own ////
//// to work with this example, then use the provided .HEX files     ////
//// a programmer capable of programming these chips.  Or, make a    ////
//// a new HEX file with these properties:                           ////
////                                                                 ////
//// NODE C: Set RX ID mask and buffers to receive ID 0x3**. (The ** ////
//// means make the least significant 8-bits no-care in the mask).   ////
//// Set TX1 buffer to ID 0x301, TX2 buffer to ID 0x302, TX3 buffer  ////
//// to ID 0x303. Set GP0 to analog (and enable the A/D).  Set GP1,  ////
//// GP2 and GP3 to OUTPUT.  Set GP4, GP5 and GP6 as INPUT with edge ////
//// trigger enable.  Leave OPTREG2 clear, disable PWM1 and PWM2,    ////
//// and disable scheduled transmission.  Also, see the baud rate    ////
//// settings above.                                                 ////
////                                                                 ////
//// NODE D: Set RX ID mask and buffers to receive ID 0x4**. (The ** ////
//// means make the least significant 8-bits no-care in the mask).   ////
//// Set TX1 buffer to ID 0x401, TX2 buffer to ID 0x402, TX3 buffer  ////
//// to ID 0x403. Configure all ports as OUTPUT.  Leave OPTREG2      ////
//// clear, disable PWM1 and PWM2, and disable scheduled             ////
//// transmission.  Also, see the baud rate settings above.          ////
////                                                                 ////
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

#include <24HJ256GP610.h>
#device adc=12
#use delay(crystal=20MHz)
#ifdef USE_ICD_FOR_RS232
 #use rs232(ICD)  //Text though the ICD
#else
 #use rs232(UART1, baud=38400)   //Text through the UART
#endif

#case

#include <can-PIC24_dsPIC33.c>

#define PIN_LED1 PIN_B4
#define PIN_LED2 PIN_A5
#define PIN_LED3 PIN_B1
 
#define BUTTON    PIN_A4

#define LED1_HIGH output_low(PIN_LED1)
#define LED1_LOW  output_high(PIN_LED1)
#define LED2_HIGH output_low(PIN_LED2)
#define LED2_LOW  output_high(PIN_LED2)
#define LED3_HIGH output_low(PIN_LED3)
#define LED3_LOW  output_high(PIN_LED3)

#define BUTTON_PRESSED  !input(BUTTON)

const char lcd_seg[10]={0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};   //0 for on, 1 for off

#define __CCS_USE_TIMER_PERIPH   1
#include <tick.c>
#define TickDifference(a,b)      (a-b)

#define ASK_FOR_ID_AD_B      0x201  //ask for AD info from CAN port B
#define SET_LED_ID_B         0x202  //set LEDs for CAN port B
#define RESPOND_TO_LED_C_ID  0x303
#define WRITE_REGISTER_C_ID  0x300
#define WRITE_REGISTER_D_ID  0x400

void main(void)
{
   uint8_t b_leds=0;
   uint8_t c_leds=1;
   uint8_t a_leds=0;
   CAN_TX_HEADER tHeader;
   CAN_RX_HEADER rHeader;
   uint8_t tData[8];
   uint8_t rData[8];
   unsigned int8 curr_lcd_output,last_lcd_output=0xFF;
   unsigned int16 i;
   TICK cTick, ledTick;
   int1 cLevel, pLevel;

   setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_31);
   setup_adc_ports(sAN20 | VSS_VDD);
   set_adc_channel(20);
   
   enable_interrupts(GLOBAL);
   
   for(i=0;i<8;i++) {
      rData[i] = 0;
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

   ledTick = TickGet();
   
   pLevel = BUTTON_PRESSED;

   while(TRUE)
   {
      cTick = TickGet();
      
      if(can_kbhit())
      {
         if(can_getd(&rHeader, rData) == CAN_EC_OK)
         {
            if(rHeader.Id == ASK_FOR_ID_AD_B)
            {
               printf("Channel B AD: %X\r\n",rData[0]);
            }
            else if (rHeader.Id == RESPOND_TO_LED_C_ID)    //node C is an mcp250x0 which sends out a message upon edge detection on IO
            {
               printf("Chaining LEDs\r\n");            //in_data[0]=iointfl, in_data[1]=gpio
               a_leds = rData[0];
               if (bit_test(a_leds,4)) {LED1_HIGH;} else {LED1_LOW;}
               if (bit_test(a_leds,5)) {LED2_HIGH;} else {LED2_LOW;}
               if (bit_test(a_leds,6)) {LED3_HIGH;} else {LED3_LOW;}
            }
         }
      }

      if(can_tbe() && (TickDifference(cTick, ledTick) >= (2 * TICKS_PER_SECOND)))       //every two seconds, send new data if transmit buffer is empty
      {
         //change leds on port b
         printf("Set LEDs on Port B to %U\r\n",b_leds);
         tHeader.Id = SET_LED_ID_B;
         tHeader.Length = 1;
         tHeader.Priority = 1;
         tHeader.ext = TRUE;
         tHeader.rtr = FALSE;
         
         can_putd(&tHeader, &b_leds);
         b_leds++;
         if (b_leds > 7) {b_leds=0;}
         
         ledTick = cTick;
      }
      
      cLevel = BUTTON_PRESSED;

      if(cLevel != pLevel)
      {
         delay_us(10);
         
         if(BUTTON_PRESSED != pLevel)
         {
            if(cLevel)
            {
               //ask for AD on port B
               printf("Asking for A/D reading on Port B...\r\n");
               tHeader.Id = ASK_FOR_ID_AD_B;
               tHeader.Length = 0;
               tHeader.Priority = 1;
               tHeader.ext = TRUE;
               tHeader.rtr = FALSE;
               
               can_putd(&tHeader, 0);
      
               //change LEDs on port C
               tData[0]=0x1E;            //addr of gplat on 25050
               tData[1]=0x0E;            //mask
               tData[2]=~(c_leds << 1);  //new gplat values
               printf("Incrementing LED on Port C\r\n");
               tHeader.Id = WRITE_REGISTER_C_ID;
               tHeader.Length = 3;
               tHeader.Priority = 1;
               tHeader.ext = TRUE;
               tHeader.rtr = FALSE;
               
               can_putd(&tHeader, tData);
               c_leds++;
               if (c_leds > 7) {c_leds=0;}
            }
            
            pLevel = cLevel;
         }
      }

      //change lcd segment on port d
      i=read_adc();
      curr_lcd_output=i/410;   //scale to 0-9
      if (curr_lcd_output != last_lcd_output)
      {
         last_lcd_output=curr_lcd_output;
         printf("Changing 8-seg LCD on D to current A/D reading (%X, %X)\r\n",i,curr_lcd_output);
         tData[0]=0x1E;                    //addr of gplat
         tData[1]=0x7F;             //mask
         tData[2]=lcd_seg[curr_lcd_output];                //new gplat values
         
         tHeader.Id = WRITE_REGISTER_D_ID;
         tHeader.Length = 3;
         tHeader.Priority = 1;
         tHeader.ext = TRUE;
         tHeader.rtr = FALSE;
         
         can_putd(&tHeader, tData);
      }
   }
}
