/////////////////////////////////////////////////////////////////////////
////                          EX_CRC_HW.C                            ////
////                                                                 ////
////  This example program shows how to send messages between two    ////
////  PICs using the HW CRC peripheral.  When the push button is     ////
////  pressed the PIC sends a message from that PIC to the other PIC ////
////  that is connected.  The receiving PIC then sends back an ACK   ////
////  after it correctly receives the message.                       ////
////                                                                 ////
////  Two sperate PICs are needed for this example.  Compile the     ////
////  code as is and program the first PIC.  Then switch the         ////
////  MACHINE_ADDRESS and SEND_ADDRESS and program the second PIC.   ////
////                                                                 ////
////  Packet Protocol:                                               ////
////  +------------+--------------+---------+--------+------+-----+  ////
////  | Address TO | Address FROM | Control | Length | Data | CRC |  ////
////  +------------+--------------+---------+--------+------+-----+  ////
////                                                                 ////
////  Address TO:    1 byte         Address of sending PIC           ////
////  Address FROM:  1 byte         Address of receiving PIC         ////
////  Control:       1 byte         Used for ACK and NACK            ////
////  Length:        2 bytes        Number of bytes in Data field    ////
////  Data:          0 to N bytes   Data being sent                  ////
////  CRC:           2 bytes        16 Bit CRC                       ////
////                                                                 ////
////  This example will work with the PCM, PCH and PCD compilers on  ////
////  devices with a hardware CRC peripheral.  The following         ////
////  conditional compilation lines are used to include a valid      ////
////  device for each compiler.  Change the device,  clock and RS232 ////
////  pins for your hardware as needed.                              ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////          (C) Copyright 1996,2020 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
////                     http://www.ccsinfo.com                      ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F1618.h>
#use delay(internal=8MHz)

#define PIC_TX_PIN      PIN_C0
#define PIC_RX_PIN      PIN_A2

#define PC_TX_PIN       PIN_C6
#define PC_RX_PIN       PIN_C7

#define PUSH_BUTTON     PIN_C5

#pin_select U1TX=PC_TX_PIN
#pin_select U1RX=PC_RX_PIN

#elif defined(__PCH__)
#include <18F45K40.h>
#use delay(internal=8MHz)

#define PIC_TX_PIN      PIN_B0
#define PIC_RX_PIN      PIN_B1

#define PC_TX_PIN       PIN_C6
#define PC_RX_PIN       PIN_C7

#define PUSH_BUTTON     PIN_D0

#pin_select U1TX=PC_TX_PIN
#pin_select U1RX=PC_RX_PIN

#pin_select U2TX=PIC_TX_PIN
#pin_select U2RX=PIC_RX_PIN

#elif defined(__PCD__)
#include <24EP256GP206.h>
#use delay(internal=7.37MHz)

#define PIC_TX_PIN      PIN_B4
#define PIC_RX_PIN      PIN_A8

#define PC_TX_PIN       PIN_A4
#define PC_RX_PIN       PIN_A9

#define PUSH_BUTTON     PIN_C3

#pin_select U1TX=PC_TX_PIN
#pin_select U1RX=PC_RX_PIN

#pin_select U2TX=PIC_TX_PIN
#pin_select U2RX=PIC_RX_PIN

#endif

#use rs232(baud=9600, xmit=PIC_TX_PIN, rcv=PIC_RX_PIN, stream=PIC_STREAM)
#use rs232(baud=9600, xmit=PC_TX_PIN, rcv=PC_RX_PIN, stream=PC_STREAM)

// CONSTANTS
#define MACHINE_ADDRESS 0x01
#define SEND_ADDRESS    0x02
#define ACK             0x01
#define NACK            0xFF
#define BUFFER_SIZE     64

// GLOBAL VARIABLES
unsigned int8 packet_buffer[BUFFER_SIZE];
unsigned int8 ext_buffer[BUFFER_SIZE];

#if BUFFER_SIZE > 255
 unsigned int16 ext_buffer_next_in;
 unsigned int16 ext_buffer_next_out;
#else
 unsigned int8 ext_buffer_next_in;
 unsigned int8 ext_buffer_next_out;
#endif

#define MESSAGE_SEND    (!input(PUSH_BUTTON))
#define DATA_IN         (ext_buffer_next_in != ext_buffer_next_out)

// EXT/RDA2 INTERRUPT
// function for reading in bytes from other PIC, PCM chips only have 1 UART
// so using EXT interrupt for receiving data, PCH and PCD chip have more then
// 1 UART so using RDA2 interrupt for receiving data.
#ifdef __PCM__
#INT_EXT
#else
#INT_RDA2
#endif
void receive_isr()
{
   ext_buffer[ext_buffer_next_in] = fgetc(PIC_STREAM);     // get a byte, put it in buffer

   if(++ext_buffer_next_in == BUFFER_SIZE)      // increment counter
      ext_buffer_next_in = 0;
}

// GET_BUFF_INT
// function to extract bytes from the buffer
unsigned int8 get_buff_int()
{
   unsigned int8 retval;

   while(!DATA_IN);                             // wait until data available

   retval = ext_buffer[ext_buffer_next_out];    // get the byte
   if(++ext_buffer_next_out == BUFFER_SIZE)     // increment counter
      ext_buffer_next_out = 0;

   return retval;
}

// SEND_PACKET
// function to send a packet of data to another PIC
void send_packet(unsigned int8 *packet_ptr, unsigned int16 packet_length)
{
   unsigned int8 *ptr;
   unsigned int16 CRC,i;

   ptr = packet_ptr;                            // set pointer
   
   crc_init(0);
   CRC = crc_calc16(ptr, packet_length, 8);     // make CRC

   for(i=0; i<packet_length; i++)               // send packet
      fputc(packet_ptr[i], PIC_STREAM);

   fputc(make8(CRC, 1), PIC_STREAM);           // send CRC
   fputc(make8(CRC, 0), PIC_STREAM);
}

typedef enum
{
   PACKET_ERR_NONE,
   PACKET_ERR_NACK,
   PACKET_ERR_WRONG_ADDR,
   PACKET_ERR_CRC,
} PACKET_ERR;

// GET_PACKET
// function to get a packet from the buffer and read the data
PACKET_ERR get_packet(unsigned int8 *packet_ptr)
{
   PACKET_ERR retval;
   unsigned int16 length;
   unsigned int16 rCRC,cCRC;
   unsigned int16 i;

   retval = PACKET_ERR_NONE;

   packet_ptr[0] = get_buff_int();              // get the address of send to
   packet_ptr[1] = get_buff_int();              // get the address of send from

   if(packet_ptr[0] != MACHINE_ADDRESS)
      retval = PACKET_ERR_WRONG_ADDR;

   packet_ptr[2] = get_buff_int();              // get the control byte
   
   if(packet_ptr[2] == NACK)
      retval = PACKET_ERR_NACK;
   
   packet_ptr[3] = get_buff_int();              // get the length of the data
   packet_ptr[4] = get_buff_int();

   length = make16(packet_ptr[3], packet_ptr[4]);
   
   for(i=5; i<(length+5); i++)                  // get the data
      packet_ptr[i] = get_buff_int();

   packet_ptr[length+5] = get_buff_int();       // get the CRC
   packet_ptr[length+6] = get_buff_int();
   
   rCRC = make16(packet_ptr[length+5], packet_ptr[length+6]);
   
   crc_init(0);
   cCRC = crc_calc16(packet_ptr, length+5, 8);
   
   if((cCRC != rCRC) && (retval == PACKET_ERR_NONE))
      retval = PACKET_ERR_CRC;

   return(retval);
}

void main()
{
   PACKET_ERR Error;
   ext_buffer_next_in = 0;                      // init variables
   ext_buffer_next_out = 0;
   
   setup_crc(16, 12, 5, 0);                     //0x1021 CRC CCITT 

  #ifdef __PCM__
   ext_int_edge(H_TO_L);                        // init interrupts
   enable_interrupts(INT_EXT);
  #else
   enable_interrupts(INT_RDA2);
  #endif
   enable_interrupts(GLOBAL);

   while(TRUE)                                  // loop always
   {
      if(MESSAGE_SEND)                          // if button pushed
      {
         packet_buffer[0] = SEND_ADDRESS;
         packet_buffer[1] = MACHINE_ADDRESS;
         packet_buffer[2] = 0;
         packet_buffer[3] = 0;
         packet_buffer[4] = 9;
         packet_buffer[5] = 'H';
         packet_buffer[6] = 'i';
         packet_buffer[7] = ' ';
         packet_buffer[8] = 't';
         packet_buffer[9] = 'h';
         packet_buffer[10] = 'e';
         packet_buffer[11] = 'r';
         packet_buffer[12] = 'e';
         packet_buffer[13] = '!';
         send_packet(packet_buffer, 14);        // send message
         
         delay_ms(100);
      }

      if(DATA_IN)                               // if data in
      {                                         
         Error = get_packet(packet_buffer);     // get packet
         
         if(Error == PACKET_ERR_NONE)           // if valid packet
         {
            unsigned int16 length,i;
   
            fprintf(PC_STREAM, "Message from unit# %U\r\n", packet_buffer[1]);
   
            length = ((int16)(packet_buffer[3]<<8)) + packet_buffer[4];
            
            if(packet_buffer[2] == ACK)
               fprintf(PC_STREAM, "Previous message sent was received by unit.\r\n");
               
            if(Length)  // display message
            {
               fprintf(PC_STREAM, "Message is:\r\n\n");
               for(i=0; i<length; ++i)
                  fputc(packet_buffer[i+5], PC_STREAM);
            }
            
            fprintf(PC_STREAM, "\r\n\n... Message End ...\r\n\n\n");
   
            if(length)
            {
               packet_buffer[0] = packet_buffer[1];   //send an ACK
               packet_buffer[1] = MACHINE_ADDRESS;
               packet_buffer[2] = ACK;
               packet_buffer[3] = 0;
               packet_buffer[4] = 0;
               send_packet(packet_buffer, 5);
            }
         }
         else if(Error == PACKET_ERR_NACK)
            fprintf(PC_STREAM, "Previous message sent was not received by unit.\r\n");
         else if(Error == PACKET_ERR_CRC)     // if not valid packet
         {
            fprintf(PC_STREAM, "Message received was corrupted.\r\n");

            packet_buffer[0] = packet_buffer[1];   //send a NACK
            packet_buffer[1] = MACHINE_ADDRESS;
            packet_buffer[2] = NACK;
            packet_buffer[3] = 0;
            packet_buffer[4] = 0;
            send_packet(packet_buffer, 5);
         }
      }
   }
}
