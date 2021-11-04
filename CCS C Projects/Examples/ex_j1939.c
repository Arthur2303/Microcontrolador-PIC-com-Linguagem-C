///////////////////////////////////////////////////////////////////////////////
////                               EX_J1939.c                              ////
////                                                                       ////
//// Example of CCS's J1939 driver.  This example was tested using the CCS ////
//// CAN Bus, CAN Bus 24 and CAN FD Development kits.                      ////
////                                                                       ////
//// This example will send a message once every second commanding Node B  ////
//// to toggle it's LED once every second.  Requires that EX_J1939B.c be   ////
//// programmed onto Node B of the development kit.  Also pressing the     ////
//// push button will cause Node A to send out a Global Address Request,   ////
//// causing all device to respond with their claimed address.  Which will ////
//// be displayed over RS232.                                              ////
////                                                                       ////
//// Select either the ICD or the UART1 peripheral for the RS232 to PC     ////
//// communication by commenting out our uncommenting the define           ////
//// USE_ICD_FOR_RS232.                                                    ////
////                                                                       ////
//// This example will work with the PCM, PCH and PCD compilers.  Comment  ////
//// out and uncomment the defines CANBUS_DEVELOPMENT_KIT,                 ////
//// CANBUS24_DEVELOPMENT_KIT and CANFD_DEVELOPMENT_KIT to select which    ////
//// hardware the example if build for.  Or comment out all three defines  ////
//// and add your own chip, clock, rs232, led and push button pins to the  ////
//// #else section of the first #if, #elif, #else, #endif to specify your  ////
//// own hardware configuration.                                           ////
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

//#define USE_ICD_FOR_RS232

#if defined(CANBUS_DEVELOPMENT_KIT) //CAN Bus development kit
 #include <18F45K80.h>
 #fuses NOWDT
 #use delay(crystal=16MHz)
 
 #ifndef USE_ICD_FOR_RS232
  #define RS232_RX_PIN  PIN_C7
  #define RS232_TX_PIN  PIN_C6
 #endif
 
 #define LED_PIN        PIN_B1
 #define PUSH_BUTTON    PIN_A3
 
#elif defined(CANBUS24_DEVELOPMENT_KIT)   //CAN Bus 24 development kit
 #include <24HJ256GP610.h>
 #fuses NOWDT
 #use delay(crystal=20MHz)
 
 #ifndef USE_ICD_FOR_RS232
  #define RS232_RX_PIN  PIN_F2
  #define RS232_TX_PIN  PIN_F3
 #endif
 
 #define LED_PIN        PIN_B1
 #define PUSH_BUTTON    PIN_A4
 
#elif defined(CANFD_DEVELOPMENT_KIT)   //CAN FD development kit
 #include <33CH128MP506.h>
 #fuses NOWDT
 #use delay(crystal=25MHz, AUX: clock=80MHz)

 #define CAN_TX_PIN     PIN_C14
 #define CAN_RX_PIN     PIN_C15

 #pin_select C1TX = CAN_TX_PIN
 #pin_select C1RX = CAN_RX_PIN
 
 #ifndef USE_ICD_FOR_RS232
  #define RS232_RX_PIN  PIN_C9
  #define RS232_TX_PIN  PIN_C8
  
  #pin_select U1RX=RS232_RX_PIN
  #pin_select U1TX=RS232_TX_PIN
 #endif
 
 #define LED_PIN        PIN_B14
 #define PUSH_BUTTON    PIN_C0
#else
 #error Added your hardware defines here
#endif

#ifdef USE_ICD_FOR_RS232
 #use rs232(ICD) //Text through the ICD
#else
 #use rs232(xmit=RS232_TX_PIN, rcv=RS232_RX_PIN,baud=38400) //Text through the UART
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

//Following defines what the CAN's baud rate it, not required defaults to 250 Kbit 
//only necessary if using non standard baud rate.  Is set to 125 Kbit to work
//on CCS CAN Bus and CAN Bus 24 development kit, and 250 Kbit for the CCS
//CANFD development kit.
#ifdef CANFD_DEVELOPMENT_KIT
 #define J1939_BAUD_RATE      250000
#else
 #define J1939_BAUD_RATE      125000
#endif

//Following defines/macros used to associate your tick timer to J1939 tick timer
// defines/macro's - Required
#define J1939GetTick()                 TickGet()
#define J1939GetTickDifference(a,b)    (a-b)
#define J1939_TICKS_PER_SECOND         TICKS_PER_SECOND
#define J1939_TICK_TYPE                TICK

//Include the J1939 driver
#include <j1939.c>

//Defines for J1939 Commands used in this example
#define LED_ON       50
#define LED_OFF      51
#define LED_TOGGLE   52

//Define for other Node's J1939 address used in this example
#define OTHER_NODE_ADDRESS    129

//Function used to initialize this unit's J1939 Address
void InitJ1939Address(void)
{
   g_MyJ1939Address = 128;
}

//Function used to initialize this unit's J1939 Name
void InitJ1939Name(void)
{
   g_J1939Name[0] = 0;
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
   uint8_t i;
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
      else if(Message.PDUFormat == J1939_PF_ADDR_CLAIMED)   //If J1939 PDU Format is Address Claim, print name and address being claimed
      {
         printf("\r\nName: 0x");
         for(i=0;i<Length;i++)
            printf("%02X",Data[7-i]);
         printf(", Address: %03u",Message.SourceAddress);
      }
   }      
   
   if(J1939GetTickDifference(CurrentTick, PreviousTick) >= (J1939_TICK_TYPE)TICKS_PER_SECOND)
   {
      //send message to other unit once every second to toggle pin
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

int1 CurrentPushButtonState,PreviousPushButtonState;

void InputTask(void)
{
   int1 state;
   
   state = input(PUSH_BUTTON);
   
   delay_us(50);
   
   if(state == input(PUSH_BUTTON))
     CurrentPushButtonState = state;     
}

void main()
{
   delay_ms(100);
   
   printf("\r\nex_j1939.c - %s\r\n", getenv("DEVICE"));

   enable_interrupts(GLOBAL);
  
   J1939Init();  //Initialize J1939 Driver must be called before any other J1939 function is used
   
   CurrentPushButtonState = PreviousPushButtonState = input(PUSH_BUTTON);
   
   while(TRUE)
   {
      J1939Task();
      
      InputTask();
      
      if(PreviousPushButtonState != CurrentPushButtonState)
      {
         if(CurrentPushButtonState == FALSE)
            J1939RequestAddress(J1939_GLOBAL_ADDRESS);   //Sends J1939 Address request, sending to global address
                                                         //causes all units on bus to respond with their address
         PreviousPushButtonState = CurrentPushButtonState;
      }
   }
}
