/*
   An example of using the timeouts library to post an event to be executed
   later.
   
   LED1 blinks every 250ms
   LED2 blinks every 667ms
   'a'-'z' character is transmitted ever 500ms to USB CDC, character 
      incremented on each call.
   '!' is posted to be transmitted every 333ms after a '.' is sent, but the 
      20ms timer ISR clears this event from happening.
   Button is debounced every 16ms, and on a debounced press a '@' is sent
      and LED3 is enabled for 500ms.  The way LED3 is enabled is by posting
      a blank timeout for 500ms and checking to see if that timeout exists.
*/
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2019 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

//#define _TIMEOUTS_STATIC_MEMORY_ENTRIES 16

/*
   Based on the CCS C Compiler used, the example will use one of CCS's development kits.
*/
#if defined(__PCM__)
   // CCS Rapid-USB development kit, with a PIC16F1459
   // this development kit only has 1 LED, so LED2 and LED3 are ignored.
   
   #include <rapidusb.h>
   
   #define LED1 PIN_C6
   #define LEDS_OFF() output_low(PIN_A5); output_low(PIN_C6)
   
   #define ALT_TIMER_ISR 2
   #define ALT_TIMER_INIT() setup_timer_2(T2_DIV_BY_64,255,16); enable_interrupts(INT_TIMER2)     //1.3 ms overflow, 21.8 ms interrupt
   
   #define BUTTON_PRESSED() !input(PIN_C1)

#elif defined(__PCH__)
   // CCS PIC18 USB development kit, with a PIC18F45K50
   
   #include <devkit_pic18usb.h>
   
   #include <usb_cdc.h>
   void usb_cdc_putc_safe(char c) {usb_cdc_putc_fast(c);}
   
   #define ALT_TIMER_ISR 1
   #define ALT_TIMER_INIT()   setup_timer_1(T1_INTERNAL|T1_DIV_BY_4); enable_interrupts(INT_TIMER1)      //21.8 ms overflow

#elif defined(__PCD__)
   // CCS PIC24 USB development kit, with a PIC24FJ256GB206
   
   #include <devkit_pic24usb.h>

   #include <usb_cdc.h>
   void usb_cdc_putc_safe(char c) {usb_cdc_putc_fast(c);}
   
   #define ALT_TIMER_ISR 2
   #define ALT_TIMER_INIT() setup_timer2(TMR_INTERNAL | TMR_DIV_BY_256, 1362); enable_interrupts(INT_TIMER2) //21.808 ms overflow

#else
   #error compiler not supported
#endif

#define LED1_TOGGLE()   output_toggle(LED1)

#ifdef LED2
   #define LED2_TOGGLE()   output_toggle(LED2)
#else
   #define LED2_TOGGLE()
#endif

#ifdef LED3
   #define LED3_ON()   LED_ON(LED3)
   #define LED3_OFF()   LED_OFF(LED3)
#else
   #define LED3_ON()
   #define LED3_OFF()
#endif

#include <timeouts.c>

// posted to the timeouts library to be called later.
// must match 'timeouts_pf_t' prototype.
// 'pArgs' will either be 1 or 2 to signify which LED to toggle.
// will then re-add this at the appropiate rate.
void OnLEDTimeout(void* pArgs)
{
   if (pArgs == 1)
   {
      LED1_TOGGLE();
      
      TimeoutsAdd(OnLEDTimeout, 1, 250);
   }
   else if (pArgs == 2)
   {
      LED2_TOGGLE();
      
      TimeoutsAdd(OnLEDTimeout, 2, 667);
   }
}

// posted to the timeouts library to be called later.
// must match 'timeouts_pf_t' prototype.
// 'pArgs' is ignored.
// send '!' over USB CDC.
void OnExclaimTimeout(void* pArgs)
{   
   usb_cdc_putc_safe('!');
}

// posted to the timeouts library to be called later.
// must match 'timeouts_pf_t' prototype.
// 'pArgs' points to the character we want to send.
// will send this character over USB and then queue up the next character
// 500ms later.
// also queues up a '!' to be sent 333ms from now (will be erased on timer
// ISR).
void OnPutcTimeout(void* pArgs)
{
   char c;
   
   c = *(char*)pArgs;
   
   usb_cdc_putc_safe(c);
   
   if (c == 'z')
      c = 'a';
   else
      c += 1;
   
   *(char*)pArgs = c;
   
   TimeoutsAdd(OnPutcTimeout, pArgs, 500);
   
   TimeoutsAdd(OnExclaimTimeout, NULL, 333);
}

// posted to the timeouts library to be called later.
// must match 'timeouts_pf_t' prototype.
// send '@' to USB CDC.
// this event is posted once a button pressed has been debounced.
void OnButtonDebouncedTimeout(void *pArgs)
{
   if (pArgs == 1)
   {
      usb_cdc_putc_safe('@');
   }
}

// posted to the timeouts library to be called later.
// must match 'timeouts_pf_t' prototype.
// this event is posted to be called 500ms after a button press has been 
// debounced.
void OnButtonDelayedTimeout(void *pArgs)
{
}

// posted to the timeouts library to be called later.
// must match 'timeouts_pf_t' prototype.
// this is called every 16ms.
// pArgs contains a statemachine of debouncing the button.
//    bit7 - the debounced state of the button (1=pressed, 0=not pressed).
//    bits0-2 - number of consecutive times button has been the opposite of debounced state.
// when bit7 goes from 0->1, 
//    it pushes OnButtonDebouncedTimeout to be called right away
//    AND it pushes OnButtonDelayedTimeout to be called 500ms from now.
void OnButtonDebouncingTimeout(void *pArgs)
{
   unsigned int8 count;
   
   count = pArgs & 0x7;   //bits 0-2
   pArgs &= 0xF8; //mask bits 0-2
   
   if (bit_test(pArgs, 7) != BUTTON_PRESSED())
   {
      if (++count >= 4)
      {
         pArgs ^= 0x80; //toggle bit7
         count = 0;
         
         if (bit_test(pArgs, 7))
         {
            // button was press was debounced.
            // send an event indicating it was debounced.
            TimeoutsImmediate(OnButtonDebouncedTimeout, 1);
            
            // add a second event 500ms later.
            // this is not normal and is only used as a demonstration in the demo.
            TimeoutsRemove(OnButtonDelayedTimeout, NULL);
            TimeoutsAdd(OnButtonDelayedTimeout, 1, 500);
         }
      }
   }
   
   pArgs |= count;
   
   // have debounce algorithm run again 16ms from now, passing our
   // current state in pArgs.
   TimeoutsAdd(OnButtonDebouncingTimeout, pArgs, 16);
}

#if ALT_TIMER_ISR==0
#int_timer0
#elif ALT_TIMER_ISR==1
#int_timer1
#elif ALT_TIMER_ISR==2
#int_timer2
#else
#error not supported in this example
#endif
void isr_alt_timer(void)
{
   TimeoutsRemove(OnExclaimTimeout, NULL);
}

void main(void)
{
   char tx = 'a';

   enable_interrupts(GLOBAL);
  
   ALT_TIMER_INIT();
   
   usb_init_cs();
   
   LEDS_OFF();
   
   OnLEDTimeout(1);
   OnLEDTimeout(2);
   OnPutcTimeout((void*)&tx);
   OnButtonDebouncingTimeout(0);
   
   for(;;)
   {
      usb_task();
      
      TimeoutsTask();
      
      if (TimeoutsIsAdded(OnButtonDelayedTimeout, NULL))
      {
         LED3_ON();
      }
      else
      {
         LED3_OFF();
      }
      
      #if 0
      uint16_t ms = TimeoutsNext();
      delay_ms(ms);
      #endif
   }
}

