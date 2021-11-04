///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                      ex_sk_gps_extracredit.c                      ////
////                                                                   ////
//// Example showing how to get the GPS NMEA data from the GPS module. ////
//// The GPS on this board is a u-Blox Neo-6M.  This example developed ////
//// to run on the E3mini development kit.  Connect GND of module to   ////
//// GND of E3, connect VCC of module to +5 of E3, connect TXD of      ////
//// module to B5 of E3mini.                                           ////
////                                                                   ////
//// This example filters out all messages except for GPGGA messages,  ////
//// and then only displays certain values from that message.          ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2016 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

#include <e3.h>

// create a new serial port using the UART peripheral of the PIC18F14K50.
// on the PIC18F14K50, PIN_B5 is receive and PIN_B7 is transmit.
#use rs232(UART1, baud=9600, errors, receive_buffer=128, stream=STREAM_GPS)

#include <input.c>

void main(void)
{
   char str[128];
   char gpgga[] = "$GPGGA,";
   char tokens[] = ",";
   int i;
   char *p, *lat, *latDir, *lon, *lonDir, *fix, *num, *time;
   
   fprintf(STREAM_USB, "\r\n\r\nex_sk_gps.c - GPS sensor board example\r\n\r\n");
   
   only_led_on(GREEN_LED);
   
   // interrupts need to be enabled in order for the #use rs232()'s 
   // receive buffering to work.
   enable_interrupts(GLOBAL);
 
   for(;;)
   {
      fgets(str, STREAM_GPS);
      
      //fprintf(STREAM_USB, "%s", str);
      
      if (strstr(str, gpgga) != 0)
      {
         //fprintf(STREAM_USB, "%s", str);
         
         output_toggle(YELLOW_LED);
         
         p = strtok(str, tokens);
         i = 0;
         while(p != NULL)
         {
            switch(i++)
            {
               case 1:  time = p;      break;
               case 2:  lat = p;       break;
               case 3:  latDir = p;    break;
               case 4:  lon = p;       break;
               case 5:  lonDir = p;    break;
               case 6:  fix = p;       break;
               case 7:  num = p;       break;
               default: break;
            }            
            p = strtok(NULL, tokens);
         }
         
         fprintf(STREAM_USB, "fix=%s, num=%s, time=%s, lat=%s %s, lon=%s %s\r\n",
               fix,
               num,
               time,
               lat,
               latDir,
               lon,
               lonDir
            );
      }
   }
}

