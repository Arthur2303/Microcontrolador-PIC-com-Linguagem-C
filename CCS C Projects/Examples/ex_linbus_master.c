/////////////////////////////////////////////////////////////////////////
////                       ex_linbus_master.c                        ////
////                                                                 ////
//// Example of using CCS's #use rs232() built-in LIN Bus protocol   ////
//// support.                                                        ////
////                                                                 ////
//// This an example LIN Bus Master, there is only one LIN Bus       ////
//// Master on the network.  A LIN Bus message frame consists of two ////
//// parts a header, only sent by the Master, and a response, which  ////
//// can be sent by the Master or one of the Slave devices on the    ////
//// network.                                                        ////
////                                                                 ////
//// The header consist of the following three parts the BREAK, the  ////
//// SYNC and the IDENTIFIER fields.  The BREAK field is used to     ////
//// get the attention of the slave devices attached to the LIN Bus. ////
//// It consists of one start bit and at least 11 dominant bits (0), ////
//// the current standard is 13 dominant bits.  The SYNC field is    ////
//// used to synchronize the slave devices on the bus the current    ////
//// bit time on the bus.  This is done by the master sending 0x55   ////
//// and the slave devices measuring the time between the rising and ////
//// falling edges.  Finally the IDENTIFIER field is used to         ////
//// determine which device transmits during the response and which  ////
//// device or devices receive the response.                         ////
////                                                                 ////
//// The response consist of the following two parts the DATA and    ////
//// CHECKSUM fields.  The DATA field is the data to be transmitted  ////
//// during the LIN Bus message frame.  The CHECKSUM field is the    ////
//// checksum of the LIN Bus message frame.  There are two checksums ////
//// types that can be used, either a legacy checksum or a enhanced  ////
//// checksum.  The legacy checksum is only the checksum done on the ////
//// data field, and the enhanced checksum is the checksum done on   ////
//// the identifier and data fields.                                 ////
////                                                                 ////
//// This example initiates a total of three LIN Bus messages.  The  ////
//// first is with identifier 0x01, which indicates that the master  ////
//// will be transmitting the response, the response will be the     ////
//// current count value which the master increments each time it    ////
//// sends it.  The second and third messages are with identifiers   ////
//// 0x02 and 0x03 respectively.  These are identifies telling       ////
//// one of two slave devices to respond with their current ADC      ////
//// readings for two channels.                                      ////
////                                                                 ////
//// Using a serial port you can see messages about what the example ////
//// is doing.  Select either the ICD or the specified RS232 pins    ////
//// for doing the serial communication with the PC by commenting or ////
//// uncommenting the define USE_ICD_FOR_RS232.                      ////
////                                                                 ////
//// This example will work with the PCM, PCH and PCD compilers.     ////
//// The following lines are used to include a valid device.  Change ////
//// the device, clock, RS232, LED and push button pins for your     ////
//// hardware if needed.                                             ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2020 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
////                     http://www.ccsinfo.com                      ////
/////////////////////////////////////////////////////////////////////////

//#define USE_ICD_FOR_RS232

#include <18F47Q43.h>
#fuses NOWDT
#use delay(oscillator=20MHz)

//Select the pins to use with the LIN Bus
#define LINBUS_TX_PIN      PIN_C6
#define LINBUS_RX_PIN      PIN_C7

//Assign the pins to UART1, the PIC18F47Q43 UART1 peripheral has built-in LIN
//Bus protocol support.
#pin_select U1TX=LINBUS_TX_PIN
#pin_select U1RX=LINBUS_RX_PIN

//Enable LIN Bus Master mode, using the specified pins, baud rate and checksum type
#use rs232(xmit=LINBUS_TX_PIN, rcv=LINBUS_RX_PIN, baud=20000, LIN=MASTER, LIN_CHECKSUM=ENHANCED, stream=LIN_STREAM)

#ifndef USE_ICD_FOR_RS232
 #define PC_TX_PIN         PIN_D6
 #define PC_RX_PIN         PIN_D7

 #pin_select U2TX=PC_TX_PIN
 #pin_select U2RX=PC_RX_PIN

 #use rs232(xmit=PC_TX_PIN, rcv=PC_RX_PIN, baud=115200, stream=PC_STREAM)
#else
 #use rs232(ICD, stream=PC_STREAM);
#endif

#include <tick.c>
#define TickDifference(a,b)   (a-b)

#define PUSH_BUTTON  PIN_D0

#define LED_TOGGLE_PIN     PIN_B0
#define LED_CS_TYPE_PIN    PIN_B1
#define LED_LIN_TX_PIN     PIN_B2

void main(void)
{
   unsigned int8 tId = 0x01;
   unsigned int8 rId;
   TICK cTick, pTick;
   unsigned int32 Count = 0;
   unsigned int8 Checksum;
   unsigned int16 ADCReading[2];
   int1 cLevel, pLevel;
   int1 ChecksumType = 1;
   
   delay_ms(100);
   
   enable_interrupts(GLOBAL);
   
   fprintf(PC_STREAM, "\r\nex_linbus_master.c - %s\r\n", getenv("DEVICE"));
   
   output_low(LED_TOGGLE_PIN);
   output_bit(LED_CS_TYPE_PIN, ChecksumType);
   output_low(LED_LIN_TX_PIN);
   
   pTick = TickGet();
   pLevel = input(PUSH_BUTTON);
   
   while(TRUE)
   {
      cTick = TickGet();
      
      if(TickDifference(cTick, pTick) >= (TICKS_PER_SECOND / 4))
      {
         output_toggle(LED_TOGGLE_PIN);
         output_high(LED_LIN_TX_PIN);
         
         //Start a LIN Bus message frame with the specified identifier field
         rId = linbus_header(LIN_STREAM, tId);
        
         if(++tId > 0x03)
            tId = 0x01;
         
         pTick = cTick;
      }
      else
         rId = 0x00;
      
      if(rId != 0x00)
      {
         if(rId == 0x01)
         {
            //Send a the LIN Bus response
            linbus_tx_response(LIN_STREAM, &Count, 4);
            
            Count++;
         }
         else if((rId == 0x02) || (rId == 0x03))
         {
            //Receive the LIN Bus response
            Checksum = linbus_rx_response(LIN_STREAM, ADCReading, 4);
            
            fprintf(PC_STREAM, "\r\nADC Readings Identifier %02X - CH0: %04LX, CH1: %04LX", rId, ADCReading[0], ADCReading[1]);
         }
         else
            fprintf(PC_STREAM, "\r\nUnknown Identifier: %02X", rId);
         
         output_low(LED_LIN_TX_PIN);
      }
      
      cLevel = input(PUSH_BUTTON);
      
      if(cLevel != pLevel)
      {
         delay_us(50);
         
         if(input(PUSH_BUTTON) != pLevel)
         {
            if(cLevel == 0)
            {
               ChecksumType++;
               
               //Set the LIN Bus functions to use the specified checksum type
               linbus_checksum_type(LIN_STREAM, ChecksumType);
               
               output_bit(LED_CS_TYPE_PIN, ChecksumType);
            }
            
            pLevel = cLevel;
         }
      }
   }
}
