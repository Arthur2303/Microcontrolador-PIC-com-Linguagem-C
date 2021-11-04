/////////////////////////////////////////////////////////////////////////
////                    EX_CAN_CCS_A_PCD_TX_ISR.C                    ////
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
//// This example uses a software TX buffer to send the CAN          ////
//// messages.  Additionally it is setup so that the CAN1 ISR occurs ////
//// when the CAN TX buffer finishes transmitting a message.  If     ////
//// there is one or more messages in the software buffer to send,   ////
//// the CAN ISR will load the next message into the HW TX buffer to ////
//// send.                                                           ////
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
////         (C) Copyright 1996,2021 Custom Computer Services        ////
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
#fuses NOWDT
#device adc=12
#use delay(crystal=20MHz)
#ifdef USE_ICD_FOR_RS232
 #use rs232(ICD)  //Text though the ICD
#else
 #use rs232(UART1, baud=38400, SHOW_WARNING, MAX_ERROR=1.0, BRGH1OK)   //Text through the UART
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

/////////////////////////////// CAN TX Interrupt //////////////////////////////

#define CAN_SOFTWARE_BUFFER_SIZE       32

typedef struct
{
   CAN_TX_HEADER Header;
   uint8_t Data[8];
} can_tx_struct_t;

struct
{
   can_tx_struct_t Buffer[CAN_SOFTWARE_BUFFER_SIZE];
   uint8_t Count;
   uint8_t NextIn;
   uint8_t NextOut;
} g_CanTx;

///////////////////////////////////////////////////////////////////////////////
// CanTxInit()
//
// Function to initialize the CAN TX Buffer and enabled the appropriate CAN
// Event peripheral interrupt and PIC interrupts.
///////////////////////////////////////////////////////////////////////////////
void CanTxInit(void)
{
   memset(&g_CanTx, 0x00, sizeof(g_CanTx));
   
   can_enable_interrupts(CAN_INTERRUPT_TX);
   enable_interrupts(INT_CAN1);
   
   enable_interrupts(GLOBAL);
}

///////////////////////////////////////////////////////////////////////////////
// CanPushData()
//
// Function to move data from software TX Buffer to CAN peripheral TX Buffer.
// Did it this way because the CAN TX interrupt occurs when a HW TX Buffer is 
// finished sending message, because of that this function is called by the
// the CanTxData() function and the can_tx_isr() to move data from software
// buffer to the hardware TX buffers.
///////////////////////////////////////////////////////////////////////////////
void CanPushData(void)
{
   can_putd(&g_CanTx.Buffer[g_CanTx.NextOut].Header, g_CanTx.Buffer[g_CanTx.NextOut].Data);
      
   if(++g_CanTx.NextOut >=  CAN_SOFTWARE_BUFFER_SIZE)
      g_CanTx.NextOut = 0;
   
   g_CanTx.Count--;
}

///////////////////////////////////////////////////////////////////////////////
// CanTxData() 
//
// Function used to load the data to transmit into the software TX buffer.
//
// Parameters:
//    Header - pointer to a CAN_TX_HEADER structure variable containing the
//             header to transmit the message with.
//
//    Data - pointer to an uint8_t array contain the data to transmit with the
//           message.
//
// Returns - TRUE if message as successfully loaded into the buffer.  FALSE
//           if the message was not loaded into the buffer, i.e. buffer is 
//           full.
///////////////////////////////////////////////////////////////////////////////
int1 CanTxData(CAN_TX_HEADER *Header, uint8_t *Data)
{
   if(g_CanTx.Count < CAN_SOFTWARE_BUFFER_SIZE)
   {
      memcpy(&g_CanTx.Buffer[g_CanTx.NextIn].Header, Header, sizeof(CAN_TX_HEADER));
      memcpy(g_CanTx.Buffer[g_CanTx.NextIn].Data, Data, Header->Length);
      
      if(++g_CanTx.NextIn >= CAN_SOFTWARE_BUFFER_SIZE)
         g_CanTx.NextIn = 0;
      
      g_CanTx.Count++;
      
      if(can_tbe())
         CanPushData();
      
      return(TRUE);
   }
   else
      return(FALSE);
}

#INT_CAN1
void can_tx_isr(void)
{
   //Indicates ISR occurred
   output_toggle(PIN_E4);
   
   while((g_CanTx.Count > 0) && can_tbe())
   {
      //Indicates that ISR sent a message
      output_toggle(PIN_E5);
   
      CanPushData();
   }
   
   //Clear CAN Event TX interrupt flag
   can_clear_interrupt(CAN_INTERRUPT_TX);
}

////////////////////////////////////////////////////////////////////////////////

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
   
   CanTxInit();
   
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
      
      if(TickDifference(cTick, ledTick) >= (2 * TICKS_PER_SECOND))       //every two seconds, send new data
      {
         //change leds on port b
         printf("Set LEDs on Port B to %U\r\n",b_leds);
         tHeader.Id = SET_LED_ID_B;
         tHeader.Length = 1;
         tHeader.Priority = 1;
         tHeader.ext = TRUE;
         tHeader.rtr = FALSE;
         
         CanTxData(&tHeader, &b_leds);
         
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
               printf("Asking for A/D reading on Port B...\r\n");
               printf("Incrementing LED on Port C\r\n");
               
               //ask for AD on port B
               tHeader.Id = ASK_FOR_ID_AD_B;
               tHeader.Length = 0;
               tHeader.Priority = 1;
               tHeader.ext = TRUE;
               tHeader.rtr = FALSE;
               
               CanTxData(&tHeader, 0);
      
               //change LEDs on port C
               tData[0]=0x1E;            //addr of gplat on 25050
               tData[1]=0x0E;            //mask
               tData[2]=~(c_leds << 1);  //new gplat values
               
               tHeader.Id = WRITE_REGISTER_C_ID;
               tHeader.Length = 3;
               tHeader.Priority = 1;
               tHeader.ext = TRUE;
               tHeader.rtr = FALSE;
               
               CanTxData(&tHeader, tData);
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
         
         CanTxData(&tHeader, tData);
      }
   }
}
