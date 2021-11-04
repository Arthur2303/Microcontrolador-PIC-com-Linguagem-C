/////////////////////////////////////////////////////////////////////////
////                    ex_sk_keypad_extracredit.C                   ////
////                                                                 ////
//// Read and write from an EEPROM.  Output messages are sent over   ////
//// serial (can be viewed in SIOW), input characters are read from  ////
//// the keypad (NOT USB).                                           ////
////                                                                 ////
//// This example also shows how to override the input method used   ////
//// by the input.c library.                                         ////
////                                                                 ////
//// Keypad connects to the E3 as so:                                ////
////   pin1 of keypad - PIN_C1                                       ////
////   pin2 of keypad - PIN_C2                                       ////
////   pin3 of keypad - PIN_B4                                       ////
////   pin4 of keypad - PIN_B5                                       ////
////   pin5 of keypad - PIN_B6                                       ////
////   pin6 of keypad - PIN_B7                                       ////
////   pin7 of keypad - PIN_C5                                       ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services         ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <e3.h>

#include <tick.c>

#define KBD_USES_PULLUPS

#define KBD_COL1  PIN_B6
#define KBD_COL2  PIN_B7
#define KBD_COL3  PIN_C5

#define KBD_ROW1  PIN_C1
#define KBD_ROW2  PIN_C2
#define KBD_ROW3  PIN_B4
#define KBD_ROW4  PIN_B5

#define KBD_NUM_ROWS 4
#define KBD_NUM_COLS 3
#define KBD_USE_EXTERNAL_KEYS
// Keypad layout:
const char KEYS[KBD_NUM_ROWS][KBD_NUM_COLS] =
  {{'1','2','3'},
   {'4','5','6'},
   {'7','8','9'},
   {'\r','0','\r'}};

#include <kbd4.c>

#define InputKbhit() kbd_kbhit()
#define InputGetc() kbd_getc()
#include <input.c>

typedef int8 INTEE;

void main() {

   unsigned int8 i, j, address;
   INTEE value;

   do {
      printf("\r\n\nEEPROM:\r\n");              // Display contents of the first 64
      for(i=0; i<=3; ++i) {                     // bytes of the data EEPROM in hex
         for(j=0; j<=15; ++j) {
            printf( "%2x ", read_eeprom( i*16+j ) );
         }
         printf("\n\r");
      }
      printf("\r\nLocation to change: ");
      address = get_Int8();
      printf("\r\nNew value: ");
      value = get_Int8();

      write_eeprom( address, value );

   } while (TRUE);
}
