/////////////////////////////////////////////////////////////////////////
////                         EX_CANFD_CCS_A.C                        ////
////                                                                 ////
//// Example of CCS's CAN FD drivers, using the dsPIC33CH128MP506.   ////
//// This example was tested with and written for the CCS CAN Bus FD ////
//// prototype board.                                                ////
////                                                                 ////
//// The CCS CAN Bus FD prototype board has four CAN FD nodes that   ////
//// communicate with each other.  Node A has a dsPIC33CH128MP506 on ////
//// it with an CAN FD peripheral to connect to the CAN Bus, Node B  ////
//// has a PIC16F18346 using a MCP2517FD controller to connect to    ////
//// the CAN Bus.  This example is the firmware for Node A.          ////
////                                                                 ////
//// Every two seconds this firmware sends out a command to Node B   ////
//// to change it's LEDs (CAN ID 0x202).                             ////
////                                                                 ////
//// Every 50 ms the channel sAN0 is read and if the ADC average has ////
//// changed the current ADC average is sent to Node D to be         ////
//// displayed on the 3x7-segment LED display (CAN ID 0x400).        ////
////                                                                 ////
//// Pressing the Node A button sends a request to Node B (CAN ID    ////
//// 0x201) for Node B's ADC reading, which Node B will respond with ////
//// a CAN message with it's ADC reading (with CAN ID 0x201).  Also  ////
//// pressing the Node A button will change the LEDs on Node C (CAN  ////
//// ID 0x300).                                                      ////
////                                                                 ////
//// Pressing one of Node C's push buttons will cause Node A's       ////
//// corresponding LED to turn on while the push button is being     ////
//// pressed.  (Node C transmits changes in the push button state,   ////
//// pressed or unpressed with CAN ID 0x303).                        ////
////                                                                 ////
//// Using a serial port you can see message about what the firmware ////
//// is doing.  Select either the ICD or the UART1 peripheral for    ////
//// the serial communication to the PC by commenting or             ////
//// uncommenting the define USE_ICD_FOR_RS232.                      ////
////                                                                 ////
//// For more documentation on the CCS CAN FD driver,                ////
//// can-dspic33_fd.c                                                ////
////                                                                 ////
//// This example will work with the PCD compiler.                   ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////                                                                 ////
//// Node C and D have PIC16F15324 and PIC16F18346 respectively      ////
//// using a MCP2517FD controllers to connect to the CAN Bus.  These ////
//// Nodes have been preprogrammed by CCS to work with these         ////
//// examples.  However, if you will you can reprogram then with     ////
//// the provided .HEX file, see the Examples/devkit_canfd folder,   ////
//// an ICD.  Or build your own HEX file for them, the original      ////
//// source code for Node C and D are also provided in the           ////
//// Examples/devkit_canfd folder.                                   ////
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

#include <33CH128MP506.h>
#device ADC=12
#fuses NOWDT
#use delay(clock=180MHz, crystal=20MHz, AUX: clock=80MHz)
#ifdef USE_ICD_FOR_RS232
 #use rs232(ICD)
#else
 #pin_select U1TX=PIN_C8
 #pin_select U1RX=PIN_C9

 #use rs232(xmit=PIN_C8, rcv=PIN_C9, baud=115200)
#endif

#define CAN_TX_PIN      PIN_C14
#define CAN_RX_PIN      PIN_C15

#pin_select C1TX = CAN_TX_PIN
#pin_select C1RX = CAN_RX_PIN

#define CAN_TX_BUFFERS     16

#include <can-dspic33_fd.c>

//Node D ID and functions

#define WRITE_7SEG_D_ID    0x400
#define NODE_D_7SEG_0      0
#define NODE_D_7SEG_1      1
#define NODE_D_7SEG_2      2

const uint8_t lcd_seg[16] = { 0x3F, 0x30, 0x6D, 0x79, 0x72, 0x5B, 0x5F, 0x31, 
                              0x7F, 0x73, 0x77, 0x5E, 0x0F, 0x7C, 0x4F, 0x47};
                              
void write_7_segment(uint8_t segment, uint8_t mask, uint8_t value)
{
   CAN_TX_HEADER Header;
   uint8_t Data[3];
   
   Header.Id = WRITE_7SEG_D_ID;
   Header.Length = 3;
   Header.ext = TRUE;
   Header.rtr = FALSE;
   Header.Format = CAN_MESSAGE_FORMAT_CAN_FD;
   
   Data[0] = segment;
   Data[1] = mask;
   Data[2] = value;
   
   can_putd(&Header, Data);
}

void write_7_segments(uint16_t value, uint8_t base)
{
   write_7_segment(NODE_D_7SEG_0, 0x7F, lcd_seg[value % base]);
   
   value /= base;
   write_7_segment(NODE_D_7SEG_1, 0x7F, lcd_seg[value % base]);
   
   value /= base;
   write_7_segment(NODE_D_7SEG_2, 0x7F, lcd_seg[value]);
}

void clear_7_segments(void)
{
   write_7_segment(NODE_D_7SEG_0, 0xFF, 0);
   write_7_segment(NODE_D_7SEG_1, 0xFF, 0);
   write_7_segment(NODE_D_7SEG_2, 0xFF, 0);
}

// Node C IDs and functions

#define WRITE_LEDS_C_ID    0x300
#define NODE_C_PUSHBUTTON_ID  0x303 

void write_c_leds(uint8_t Value)
{
   CAN_TX_HEADER Header;
   uint8_t Data[2];
   
   Header.Id = WRITE_LEDS_C_ID;
   Header.Length = 2;
   Header.ext = TRUE;
   Header.rtr = FALSE;
   Header.Format = CAN_MESSAGE_FORMAT_CAN_FD;
   
   Data[0] = 0x07;
   Data[1] = ~Value;
   
   can_putd(&Header, Data);
}

///////////

#define PUSH_BUTTON  PIN_C0

#define LED_GREEN    PIN_C12
#define LED_RED      PIN_B14
#define LED_YELLOW   PIN_B15

#define BUTTON_PRESSED  (!input(PUSH_BUTTON))

#define LED1_HIGH    output_high(LED_RED)
#define LED1_LOW     output_low(LED_RED)
#define LED2_HIGH    output_high(LED_YELLOW)
#define LED2_LOW     output_low(LED_YELLOW)
#define LED3_HIGH    output_high(LED_GREEN)
#define LED3_LOW     output_low(LED_GREEN)

#include <tick.c>
#define TickDifference(a,b)      (a-b)

#define ASK_FOR_ID_AD_B      0x201  //ask for AD info from CAN port B
#define SET_LED_ID_B         0x202  //set LEDs for CAN port B

#define ADC_AVG_COUNT      16

uint16_t GetADCAverage(uint16_t *Reading, uint8_t Count)
{
   uint8_t i;
   uint32_t Sum = 0;
   
   for(i=0;i<Count;i++)
      Sum += (uint32_t)Reading[i];
   
   Sum /= Count;
   
   return((uint16_t)Sum);
}

void main(void)
{
   uint8_t b_leds=0;
   uint8_t a_leds=0;
   uint8_t c_leds=0;
   CAN_TX_HEADER tHeader;
   CAN_RX_HEADER rHeader;
   uint8_t rData[8];
   uint16_t aReading[ADC_AVG_COUNT];
   uint8_t aCount = 0, aIndex = 0;
   uint16_t cAverage, pAverage=0xFFFF;
   TICK cTick, ledTick, ADCTick;
   int1 cLevel, pLevel;
   
   setup_adc_ports(sAN0, VSS_VDD);
   setup_adc(ADC_CLOCK_SYSTEM | ADC_CLOCK_DIV_2 | ADC_WARMTIME_32768 | 
             ADC_SHARED_CLOCK_DIV_2 | ADC_SHARED_TAD_MUL_513 | ADC_ENABLE_SHARED_CORE);
   set_adc_channel(0);
   
   LED1_LOW;
   LED2_LOW;
   LED3_LOW;
   delay_ms(1000);
   printf("\rCCS CAN FD Example Node A\r\n\n");
   LED1_HIGH;
   LED2_HIGH;
   LED3_HIGH;
   
   can_init();
   
   printf("Running...\r\n\n");
   
   clear_7_segments();
   write_c_leds(c_leds);
   
   ledTick = TickGet();
   ADCTick = ledTick - (TICKS_PER_SECOND / 20);
   
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
               printf("Node B AD: %03LX\r\n", make16(rData[1], rData[0]));
            }
            else if(rHeader.Id == NODE_C_PUSHBUTTON_ID)
            {
               printf("Chaining LEDs: %X\r\n", rData[0]);
               a_leds = rData[0];
               
               if(bit_test(a_leds, 2)) {LED1_HIGH;} else {LED1_LOW;}
               if(bit_test(a_leds, 1)) {LED2_HIGH;} else {LED2_LOW;}
               if(bit_test(a_leds, 0)) {LED3_HIGH;} else {LED3_LOW;}
            }
         }
      }
      
      if(can_tbe() && (TickDifference(cTick, ledTick) >= (2 * TICKS_PER_SECOND)))       //every two seconds, send new data if transmit buffer is empty
      {
         //change leds on node b
         printf("Set LEDs on Node B to %u\r\n", b_leds);
         tHeader.Id = SET_LED_ID_B;
         tHeader.Length = 1;
         tHeader.ext = TRUE;
         tHeader.rtr = FALSE;
         tHeader.Format = CAN_MESSAGE_FORMAT_CAN_FD;
         
         can_putd(&tHeader, &b_leds);
         b_leds++;
         if (b_leds > 7) {b_leds=0;}
         
         ledTick = cTick;
      }
      
      cLevel = BUTTON_PRESSED;

      if(cLevel != pLevel)
      {
         delay_us(50);
         
         if(BUTTON_PRESSED != pLevel)
         {
            if(cLevel)
            {
               //ask for AD on Node B
               printf("Asking for A/D reading on Node B...\r\n");
               tHeader.Id = ASK_FOR_ID_AD_B;
               tHeader.Length = 0;
               tHeader.ext = TRUE;
               tHeader.rtr = FALSE;
               tHeader.Format = CAN_MESSAGE_FORMAT_CAN_FD;
               
               can_putd(&tHeader, 0);
               
               //change LEDs on port C
               printf("Incrementing LEDs on Node C\r\n");
               if(++c_leds > 7)
                  c_leds = 0;
               
               write_c_leds(c_leds);
            }
            
            pLevel = cLevel;
         }
      }
      
      if(TickDifference(cTick, ADCTick) >= (TICKS_PER_SECOND / 20))  //read ADC every 50ms
      {
         aReading[aIndex] = read_adc();
         
         if(++aIndex >= ADC_AVG_COUNT)
            aIndex = 0;
         
         if(++aCount > ADC_AVG_COUNT)
            aCount = ADC_AVG_COUNT;
         
         cAverage = GetADCAverage(aReading, aCount);
         
         //If average changed update Node D 7 segment display
         if(cAverage != pAverage)
         {
            printf("Changing Node D 7-segments to current ADC average (%03LX)\r\n", cAverage);
            write_7_segments(cAverage, 16);
            
            pAverage = cAverage;
         }
         
         ADCTick = cTick;
      }
   }
}
