/////////////////////////////////////////////////////////////////////////
////                        EX_HUMIDITY.C                            ////
////                                                                 ////
//// This program displays the relative humity, in percent (RH%),    ////
//// read by a Humirel HT3223/HTF3223 Humidity Module.               ////
////                                                                 ////
////  This example will work with the PCM and PCH compilers.  The    ////
////  following conditional compilation lines are used to include a  ////
////  valid device for each compiler.  Change the device, clock and  ////
////  RS232 pins for your hardware if needed.                        ////
/////////////////////////////////////////////////////////////////////////
////     (C) Copyright 1996,2005,2018 Custom Computer Services       ////
//// This source code may only be used by licensed users of the CCS  ////
//// C compiler.  This source code may only be distributed to other  ////
//// licensed users of the CCS C compiler.  No other use,            ////
//// reproduction or distribution is permitted without written       ////
//// permission.  Derivative programs created using this software    ////
//// in object code form are not restricted in any way.              ////
/////////////////////////////////////////////////////////////////////////

#if defined(__PCM__)
#include <16F887.h>
#elif defined(__PCH__)
#include <18F4520.h>
#endif

#use delay(crystal=20mhz)
#use rs232(icd)                                  //Text through the ICD
//#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //Text through the UART


#include <ht3223.c>

void main(void) {
    int8 rh;

    printf("\r\n\nRelative Humidity:\r\n");

    humidity_init();
    enable_interrupts(TRUE);

    while (!humidity_ready()) {}

    while(TRUE) {
        rh=humidity_read();
        printf("\r\n%U%%",rh);
        delay_ms(1000);
    }
}
