/////////////////////////////////////////////////////////////////////////
////                        ex_linbus_slave.c                        ////
////                                                                 ////
//// Example of using CCS's #use rs232() built-in LIN Bus protocol   ////
//// support.                                                        ////
////                                                                 ////
//// This an example LIN Bus Slave, there can be multiple LIN Bus    ////
//// Slaves on the network.  A LIN Bus message frame consists of two ////
//// parts a header, only sent by the Master, and a response, which  ////
//// can be sent by the Master or one of the Slave devices on the    ////
//// network (see ex_linbus_master.c for more info about the LIN Bus ////
//// message frame).                                                 ////
////                                                                 ////
//// This example works in conjunction with ex_linbus_master.c after ////
//// the master initiates the message frames it will with transmit   ////
//// or receive the response based on the identifier field that was  ////
//// received.                                                       ////
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

#include <18F46Q43.h>
#device ADC=12
#fuses NOWDT
#use delay(oscillator=20MHz)

//Select the pins to use with the LIN Bus
#define LINBUS_TX_PIN      PIN_C6
#define LINBUS_RX_PIN      PIN_C7

//Assign the pins to UART1, the PIC18F47Q43 UART1 peripheral has built-in LIN
//Bus protocol support.
#pin_select U1TX=LINBUS_TX_PIN
#pin_select U1RX=LINBUS_RX_PIN

//Enable LIN Bus Slave mode, using the specified pins, baud rate and checksum type
#use rs232(xmit=LINBUS_TX_PIN, rcv=LINBUS_RX_PIN, baud=20000, LIN=SLAVE, LIN_CHECKSUM=ENHANCED, stream=LIN_STREAM)

#ifndef USE_ICD_FOR_RS232
 #define PC_TX_PIN         PIN_D6
 #define PC_RX_PIN         PIN_D7

 #pin_select U2TX=PC_TX_PIN
 #pin_select U2RX=PC_RX_PIN

 #use rs232(xmit=PC_TX_PIN, rcv=PC_RX_PIN, baud=115200, stream=PC_STREAM)
#else
 #use rs232(ICD, stream=PC_STREAM)
#endif

#define LED_TOGGLE_PIN  PIN_B0

void main(void)
{
   unsigned int8 Id;
   unsigned int32 Count;
   unsigned int16 ADCReading[2];
   unsigned int8 Checksum;
   
   delay_ms(10);
   
   output_low(LED_TOGGLE_PIN);
   
   fprintf(PC_STREAM, "\r\nex_linbus_slave.c - %s\r\n", getenv("DEVICE"));
   
   setup_adc_ports(sAN0 | sAN1, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_255);

   while(TRUE)
   {
      //Check is a LIN Bus Header has been received, for UART peripherals that
      //have built-in protocol support this will return TRUE after the identifier
      //field has been received.  For UART peripheral without built-in protocol
      //support this will return TRUE after the first 8 bits of the break is
      //received.
      if(linbus_header_hit(LIN_STREAM))
      {
         output_toggle(LED_TOGGLE_PIN);
      
         //Get header and return the identifier.  For UART peripherals without
         //built-in protocol support this function puts the UART into auto-baud
         //mode to receive the sync character, so this function must be called
         //before the master sends the sync field.
         Id = linbus_header_get(LIN_STREAM);
         
         //If identifier is 0x01 master is sending data so call linbus_rx_response()
         //to receive data and checksum.
         if(Id == 0x01)
         {
            Checksum = linbus_rx_response(LIN_STREAM, &Count, 4);
            
            fprintf(PC_STREAM, "\r\nReceived Count: %lu - Checksum: %02X", Count, Checksum);
         }
         //If identifier is 0x02 this device transmits response, so call linbus_tx_response()
         //to send data and checksum.
         else if(Id == 0x02)
         {
            set_adc_channel(0);
            ADCReading[0] = read_adc();
            
            set_adc_channel(1);
            ADCReading[1] = read_adc();
            
            linbus_tx_response(LIN_STREAM, ADCReading, 4);
         }
        #if getenv("UART1") < 3 //UART1 doesn't have built-in protocol support
         //If identifier is 0x03 another slave transmits response, since this
         //UART peripheral doesn't have build-in protocol support this device
         //must receive the response to keep if from getting out of sync and
         //locking up UART peripheral.
         else if(Id == 0x03)
         {
            linbus_rx_response(LIN_STREAM, ADCReading, 4);
         }
         else
         {
            fprintf(PC_STREAM, "\r\nUnknown ID Received - %02X", Id);   //if this happens it will likely cause it to get out
                                                                        //of sync with the LIN Bus and/or lockup the UART
         }
        #endif
      }
   }
}
