/////////////////////////////////////////////////////////////////////////
////                       EX_PCD_CHECKSUM.C                         ////
////                                                                 ////
////  This program demonstrates the use of #ROM address = checksum   ////
////  to generate a value to be stored at the end of program memory, ////
////  for this example, that will make the checksum of program       ////
////  memory equal to 0x1248.  The checksum is verified during       ////
////  run-time by reading the entire program memory and verifing     ////
////  that the sum is equal to the value 0x1248.                     ////
////                                                                 ////
////  Select either the ICD or your own RS232-to-PC connection       ////
////  for the text I/O.                                              ////
////                                                                 ////
////  This example will work with the PCD compiler.                  ////
////  The following conditional compilation lines are used to        ////
////  include a valid device for each compiler.  Change the device,  ////
////  clock and RS232 pins for your hardware if needed.              ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2012,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#include <24FJ128GA006.h>
#use delay(crystal=20MHz)
#use rs232(icd)               //Text through the ICD
//#use rs232(UART1,baud=9600) //Text through the UART

#ROM getenv("PROGRAM_MEMORY")-2 = checksum

/*
int1 perform_checksum()
This will return TURE if checksum is verified and FALSE if checksum fails.
PARAM : none
RETURNS : Boolean
*/
int1 perform_checksum()
{
   unsigned int16 checksum;
   unsigned int16 data[2];
   unsigned int32 address;
   
   checksum = 0;

   for(address=0;address<getenv("PROGRAM_MEMORY");address+=2)
   {
      read_program_memory(address,data,4);
      checksum += data[0];
      checksum += data[1];
   }
   
   if(checksum == 0x1248)
      return(TRUE);
   else
      return(FALSE);
}


void main()
{
   if(perform_checksum())
   {
      printf("\n\r Checksum Verified");
   }
   else
   {
      printf("\n\r Checksum FAILED");
   }

   while(TRUE)
   {
   }
}

