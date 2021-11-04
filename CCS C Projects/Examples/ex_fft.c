///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                             ex_fft.c                              ////
////                                                                   ////
//// An example of using the CCS C Compiler's FFT library for dsPICs.  ////
//// This example was written to run on the CCS DSP Audio development  ////
//// kit.                                                              ////
////                                                                   ////
//// Run the program on the dsPIC and use SIOW to read/send serial     ////
//// data.  The program will prompt the user to enter one or more      ////
//// sine waves (amplification and frequency), and when complete       ////
//// the example will create a sample array of size FFT_LENGTH         ////
//// comprising the user specified sine waves - and then perform an    ////
//// FFT of that array and return the frequency domain of those        ////
//// samples.  After performing the FFT, it will display the           ////
//// real, imaginary and magnitude of each FFT bin.                    ////
////                                                                   ////
//// If you comment out the #include <sine_window.h>, then it will not ////
//// perform any windowing.                                            ////
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

#if !defined(__PCD__)
#error this example requires the PCD compiler
#endif

// define this to have the example printf the input samples generated
// by the sin() function and the users's parameters.  comment this out
// to not display the samples.
//#define EXAMPLE_DISPLAY_INPUT_SAMPLES

// define this to have the example printf the result of the windowing
// function.  comment this out to not display the result.
//#define EXAMPLE_DISPLAY_WINDOW_SAMPLES

#include <33EP256MU806.h>

#use delay(crystal=12M, clock=140M, pll_wait)

#pin_select U1TX = PIN_E7
#pin_select U1RX = PIN_D11
#use rs232(UART1, baud=9600, errors)

#define FFT_LENGTH 32
#include <fft.h>
//#include <sine_window.h>
#include <dsp_data_util.c>

#include <stdlib.h>
#include <input.c>

void main()
{
   #bankx
   Complex samples[FFT_LENGTH];
   Complex *fft_result;
   float amp, f;
   unsigned int16 freq;
   int i;
   
   fft_init();
   
   printf("\r\n\r\nex_fft.c example, starting\r\n\r\n");
   
   while(TRUE)
   {
      memset(samples, 0, sizeof(samples));
      
      while(TRUE)
      {
         printf("Frequency of the sine wave (1 to %u): ", FFT_LENGTH/2);
         freq = get_int16();
         printf("\r\n");
         if ((freq < 1) || (freq > FFT_LENGTH/2))
            break;   //stop asking for inputs from the user
         
         printf("Amplitude of the sine wave: ");
         amp = get_float();
         printf("\r\n");

         for(i=0; i<FFT_LENGTH; i++)
         {
            f = sin(2.0 * PI * (float)i*(float)freq/(float)FFT_LENGTH);
            f *= amp;
            samples[i].re += (signed int16)f;
         }
      }

     #if defined(EXAMPLE_DISPLAY_INPUT_SAMPLES)
      for(i=0; i<FFT_LENGTH; i++)
      {
         printf("in,%u,%ld\r\n", i, samples[i].re);
      }
     #endif
     
      // if you don't perform window the inputs, then you need to make
      // sure the imaginary values of the complex samples are set to 0
      // because the windowwc() would clear those values for you.s
     #if defined(SINE_WINDOW_H)
      windowwc(sine_window, samples, samples, FFT_LENGTH);
     #endif
      
     #if defined(EXAMPLE_DISPLAY_WINDOW_SAMPLES)
      for(i=0;i<FFT_LENGTH;i++)
      {
         printf("windowed,%u,%ld\r\n", i, samples[i].re);
      }
     #endif
      
      fft_result = fft(samples, FFT_LENGTH);
      
      for(i=0; i<=FFT_LENGTH/2; i++)
      {
         printf("fft,%u,%ld,%ld,%ld\r\n", i, fft_result[i].re, fft_result[i].im, cplx_magnitude(&fft_result[i]));
      }
      printf("\r\n");
   }
}
