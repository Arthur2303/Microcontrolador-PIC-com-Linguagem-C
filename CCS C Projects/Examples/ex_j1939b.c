///////////////////////////////////////////////////////////////////////////////
////                              EX_J1939B.c                              ////
////                                                                       ////
//// Example of CCS's J1939 driver.  This example was tested using the CCS ////
//// CAN Bus, CAN Bus 24 and CAN FD Development kits.                      ////
////                                                                       ////
//// This example will send a message once every 250 ms commanding Node A  ////
//// to toggle it's LED once 250 milli-seconds.  Requires that EX_J1939.c  ////
//// be programmed onto Node A of the development kit.                     ////
////                                                                       ////
//// This example will work with the PCM, PCH and PCD compilers.  Comment  ////
//// out and uncomment the defines CANBUS_DEVELOPMENT_KIT,                 ////
//// CANBUS24_DEVELOPMENT_KIT and CANFD_DEVELOPMENT_KIT to select which    ////
//// hardware the example if build for.  Or comment out all three defines  ////
//// and add your own chip, clock, led pin to the #else section of the     ////
//// first #if, #elif, #else, #endif to specify your own hardware          ////
//// configuration.                                                        ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////           (C) Copyright 1996,2019 Custom Computer Services            ////
////  This source code may only be used by licensed users of the CCS C     ////
////  compiler.  This source code may only be distributed to other         ////
////  licensed users of the CCS C compiler.  No other use, reproduction or ////
////  distribution is permitted without written permission.  Derivative    ////
////  programs created using this software in object code form are not     ////
////  restricted in any way.                                               ////
////                        http://www.ccsinfo.com                         ////
///////////////////////////////////////////////////////////////////////////////

#define CANBUS_DEVELOPMENT_KIT
//#define CANBUS24_DEVELOPMENT_KIT
//#define CANFD_DEVELOPMENT_KIT

#if defined(CANBUS_DEVELOPMENT_KIT)
 #include <16F1936.h>
 #fuses NOWDT
 #use delay(internal=16MHz)
 
 #define LED_PIN  PIN_A1
 
 #define MCP2515_SPI_CS_PIN   PIN_B1
 #define MCP2515_SPI_SCK_PIN  PIN_C3
 #define MCP2515_SPI_SDO_PIN  PIN_C4
 #define MCP2515_SPI_SDI_PIN  PIN_C5
 
#elif defined(CANBUS24_DEVELOPMENT_KIT)
 #include <30F4012.h>
 #fuses NOWDT
 #use delay(internal=117.92MHz)
 
 #define LED_PIN  PIN_E1
 
#elif defined(CANFD_DEVELOPMENT_KIT)
 #include <16F18346.h>
 #fuses NOWDT
 #use delay(internal=32MHz)
 
 #define LED_PIN  PIN_C3

 #define MCP2517_SPI_SCK_PIN   PIN_C0
 #define MCP2517_SPI_SDO_PIN   PIN_C2
 #define MCP2517_SPI_SDI_PIN   PIN_C1
 #define MCP2517_SPI_CS_PIN    PIN_B4

 #pin_select SCK1OUT=MCP2517_SPI_SCK_PIN
 #pin_select SDO1=MCP2517_SPI_SDI_PIN
 #pin_select SDI1=MCP2517_SPI_SDO_PIN
 
 #define J1939_USE_EXT_FD_CONTROLLER    TRUE

#else
 #error Added your hardware defines here
#endif
 
#include <stdint.h>

void InitJ1939Address(void);
void InitJ1939Name(void);

//////////////////////////////////////////////////////////////////////////////// Tick Timer

#define __CCS_USE_TIMER_PERIPH   1
#include <tick.c>

//////////////////////////////////////////////////////////////////////////////// J1939 Settings

//Following Macros used to initialize unit's J1939 Address and Name - Required
#define J1939InitAddress()    InitJ1939Address()
#define J1939InitName()       InitJ1939Name()

//Following define selects whether to use the internal CAN peripheral of PIC or 
//if your using an external CAN Control. Not required default to TRUE if not
//specified.
#define J1939_USE_INTERNAL_CAN   FALSE

//Following defines what the CAN's baud rate it, not required defaults to 250 Kbit 
//only necessary if using non standard baud rate.  Is set to 125 Kbit to work
//on CCS CAN Bus and CAN Bus 24 development kit, and 250 Kbit for the CCS
//CANFD development kit.
#ifdef CANFD_DEVELOPMENT_KIT
 #define J1939_BAUD_RATE   250000
#else
 #define J1939_BAUD_RATE   125000
#endif

//Following defines/macros used to associate your tick timer to J1939 tick timer
// defines/macro's - Required
#define J1939GetTick()                 TickGet()
#define J1939GetTickDifference(a,b)    (a-b)
#define J1939_TICKS_PER_SECOND         TICKS_PER_SECOND
#define J1939_TICK_TYPE                TICK

//Include the J1939 driver
#include "J1939.c"

//Defines for J1939 Commands used in this example
#define LED_ON       50
#define LED_OFF      51
#define LED_TOGGLE   52

//Define for other Node's J1939 address used in this example
#define OTHER_NODE_ADDRESS    128

//Function used to initialize this unit's J1939 Address
void InitJ1939Address(void)
{
   g_MyJ1939Address = 129;
}

//Function used to initialize this unit's J1939 Name
void InitJ1939Name(void)
{
   g_J1939Name[0] = 1;
   g_J1939Name[1] = 0;
   g_J1939Name[2] = 0;
   g_J1939Name[3] = 0;
   g_J1939Name[4] = 0;
   g_J1939Name[5] = 0;
   g_J1939Name[6] = 0;
   g_J1939Name[7] = 128;
} 

//J1939 Task function for this example
void J1939Task(void)
{
   uint8_t Data[8];
   uint8_t Length;
   J1939_PDU_STRUCT Message;
   
   J1939_TICK_TYPE CurrentTick;
   static J1939_TICK_TYPE PreviousTick;

   CurrentTick = J1939GetTick();

   J1939ReceiveTask();  //J1939ReceiveTask() needs to be called often
   J1939XmitTask();     //J1939XmitTask() needs to be called often
   
   if(J1939Kbhit())  //Checks for new message in J1939 Receive buffer
   {
      J1939GetMessage(Message,Data,Length);  //Gets J1939 Message from receive buffer
      
      if(Message.PDUFormat == LED_ON)                       //If J1939 PDU Format is LED_ON, turn LED on
         output_low(LED_PIN);
      else if(Message.PDUFormat == LED_OFF)                 //If J1939 PDU Format is LED_OFF, turn LED off
         output_high(LED_PIN);
      else if(Message.PDUFormat == LED_TOGGLE)              //If J1939 PDU Format is LED_TOGGLE, toggle LED
         output_toggle(LED_PIN);
   }      
   
   if(J1939GetTickDifference(CurrentTick, PreviousTick) >= (J1939_TICK_TYPE)TICKS_PER_SECOND/4)
   {
      //send message to other unit once every 250ms to toggle pin
      Message.SourceAddress = g_MyJ1939Address;          //set PDU Source Address, this units address (g_MyJ1939Address)
      Message.DestinationAddress = OTHER_NODE_ADDRESS;   //set PDU Destination Address, address of other unit
      Message.PDUFormat = LED_TOGGLE;                    //set PDU Formate, LED_TOGGLE command
      Message.DataPage = 0;                              //set PDU Data Page can be either 0 or 1, this message uses 0
      Message.ExtendedDataPage = 0;                      //set PDU Extended Data Page, must be zero for J1939 Messages
      Message.Priority = J1939_CONTROL_PRIORITY;         //set Priority, can be 0 to 7 (0 highest priority) Control default is 3
      
      //Load PGN of Message (refer to J1939 documentation for correct format)
      Data[0] = Message.SourceAddress;
      Data[1] = Message.PDUFormat;
      Data[2] = 0;
      
      J1939PutMessage(Message,Data,3);    //loads J1939 Message into Xmit buffer
      
      PreviousTick =  CurrentTick;
   }
}

void main()
{
   enable_interrupts(GLOBAL);
  
   J1939Init();  //Initialize J1939 Driver must be called before any other J1939 function is used
   
   while(TRUE)
   {
      J1939Task();
   }
}
