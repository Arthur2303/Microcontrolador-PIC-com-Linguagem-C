///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                       ex_ezapp_common.h                           ////
////                                                                   ////
//// Hardware definitions used by the EZ App examples.                 ////
////                                                                   ////
//// For full EZ App Lynx documentation, EZApp.PDF contains an         ////
//// overview of the entire system while EZApp.h documents the full    ////
//// API that can be used with this library.                           ////
////                                                                   ////
////                                                                   ////
//// OPTIONS FOR CONFIGURING YOUR OWN HARDWARE:                        ////
//// -------------------------------------------------------------     ////
////                                                                   ////
//// This file was meant to make the examples compatible with several  ////
//// different hardware platforms.  The examples do this by using      ////
//// macros defined in this file, and if a macro is missing in this    ////
//// file it assumed that the hardware platform doesn't support        ////
//// that feature.  For example, not having a DAC0_INIT() defined      ////
//// means there is no DAC 0 channel on this hardware.                 ////
//// The macros that can be added:                                     ////
////                                                                   ////
//// LEDx_PIN                                                          ////
////     The GPIO pin connected to this led.  The examples use x       ////
////     from 0 to 2.  #define this to PIN_D0, PIN_B5, etc.            ////
////     Example: #define LED0_PIN PIN_B5                              ////
////                                                                   ////
//// LEDx_STR                                                          ////
////     Descriptive string for this LEDx_PIN.  This is used in        ////
////     the EZApp Lnyx app GUI.                                       ////
////     Example: #define LED0_PIN "LED1"                              ////
////                                                                   ////
//// LED_OFF(pin)                                                      ////
//// LED_ON(pin)                                                       ////
//// LED_SET(pin,value)                                                ////
////     How to turn a LEDx_PIN off or on.                             ////
////     Example: #define LED_OFF(x) output_low(x)                     ////
////                                                                   ////
//// DACx_INIT()                                                       ////
////     Initialize DAC for channel x.  The examples use x from 0      ////
////     to 1.                                                         ////
////     Example: #define DAC0_INIT()  Dac0Init()                      ////
////                                                                   ////
//// DACx_SET(y)                                                       ////
////     Set DAC channel x to value y.  The examples use x from 0      ////
////     to 1.                                                         ////
////     Example: #define DAC0_SET(y)  Dac0Set(y)                      ////
////                                                                   ////
//// DACx_MAX                                                          ////
////     The maximum value that can be passed to DACx_SET().           ////
////     Example: #define DAC0_MAX  1023                               ////
////                                                                   ////
//// DACx_STR                                                          ////
////     Descriptive string for this DAC.  This is displayed in the    ////
////     EZ App Lynx app GUI.                                          ////
////     Example: #define DAC0_STR "DAC0"                              ////
////                                                                   ////
//// ADC_INIT()                                                        ////
////     The ADC used in the examples uses the PIC's ADC peripheral.   ////
////     This function is called to initiliaze that peripheral.        ////
////     Example: #define ADC_INIT() setup_adc(ADC_CLOCK_INTERNAL); \  ////
////                                 setup_adc_ports(sAN0);            ////
////                                                                   ////
//// ADCx_CHANNEL()                                                    ////
////     Set the ADC peripheral to use this channel.  The examples     ////
////     only use a value of 0 for x.                                  ////
////     Example: #define ADC_CHANNEL0() set_adc_channel(0); \         ////
////                                     delay_us(10);                 ////
////                                                                   ////
//// ADCx_STR                                                          ////
////     The descriptive string for this channel.  The examples        ////
////     only use a value of 0 for x.                                  ////
////     Example: #define ADC_STR0 "POT"                               ////
////                                                                   ////
//// ADC_MAX                                                           ////
////     The maximum value that can be read from the ADC.  If you      ////
////     don't define this then the example will automatically         ////
////     define one based on getenv("ADC_RESOLUTION").                 ////
////     Example: #define ADC_MAX 1023                                 ////
////                                                                   ////
//// ADC_READ()                                                        ////
////     Returns an ADC conversion of the previously selected          ////
////     channel (from ADCx_CHANNEL()).                                ////
////                                                                   ////
//// DIGOUTx_PIN                                                       ////
//// DIGOUTx_STR                                                       ////
//// DIGOUT_SET(pin, value)                                            ////
////     Define a digital output pin and string.  The examples         ////
////     use a value of x from 0 to 1.  Example:                       ////
////        #define DIGOUT0_PIN  PIN_D2                                ////
////        #define DIGOUT0_STR  "RELAY0"                              ////
////                                                                   ////
//// BUTTONx_PRESSED()                                                 ////
////     Returns TRUE if a button or digital input signal is active.   ////
////     Teh examples use a value of x from 0 to 1.  Example:          ////
////        #define BUTTON0_PRESSED()  (!input(PIN_D3))                ////
////                                                                   ////
//// HW_INIT()                                                         ////
////     Initialize anything special about the hardware, this is       ////
////     called at the start of main().                                ////
////     Example: #define HW_INIT() output_high(PIN_D0)                ////
////                                                                   ////
//// HW_TASK()                                                         ////
////     Perform any routine tasks that are related to the hardware.   ////
////     This is called on each loop through main.                     ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2014 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
////                     http://www.ccsinfo.com                        ////
///////////////////////////////////////////////////////////////////////////

////-- DEFINE ONE OF THESE --//////

#define EZAPP_HW_IS_CCS_RN4020            //CCS EZ App Lynx development kit with RN4020 daughter-board
//#define EZAPP_HW_IS_CCS_ROBOT             //CCS PIC Robot Development Kit
//#define EZAPP_HW_IS_CCS_RN42         //CCS EZ App Lynx development kit with RN42 daughter-board
//#define EZAPP_HW_IS_RN4020_PICTAIL        //Microchip's RN4020 PICtail development kit
//#define EZAPP_HW_IS_CCS_DSPAUDIO_RN4020 //CCS DSP Audio development kit with RN4020 daughter-board connected to I/O header
//#define EZAPP_HW_IS_CCS_SERVO //CCS Servo devkit board with RN4020 daughter-board connected
//#define EZAPP_HW_IS_CCS_E3_HC06  //CCS E3 Mini devkit with an HC06 connected to it's UART

#ifndef __EX_EZAPP_COMMON_H__
#define __EX_EZAPP_COMMON_H__

#if defined(EZAPP_HW_IS_CCS_RN4020) || defined(EZAPP_HW_IS_CCS_RN42)
   #include <devkit_ezapp.h>

   // STREAM_BT_SPP_UART defined in devkit_ezapp.h
   #define STREAM_BT_SPP_BUFFER  32

   #if defined(EZAPP_HW_IS_CCS_RN4020)
      #include <RN4020_MLDP_Bluetooth.h>
      #define HW_INIT_BLUETOOTH_FLOWCONTROL()   output_low(PIN_BT_RTS)
   #elif defined(EZAPP_HW_IS_CCS_RN42)
      #include <RN42_Bluetooth.h>
      #define HW_INIT_BLUETOOTH_FLOWCONTROL()
   #else
      #error something wrong
   #endif
   
   #define ADC_READ()      read_adc()
   #define ADC_INIT()      setup_adc(ADC_CLOCK_INTERNAL)
   #define ADC0_CHANNEL()  set_adc_channel(0)
   #define ADC0_STR        "POT_A0"
   #define ADC1_CHANNEL()  set_adc_channel(1)
   #define ADC1_STR        "POT_A1"
   #define ADC2_CHANNEL()  set_adc_channel(2)
   #define ADC2_STR        "PHOTO"
   #define ADC3_CHANNEL()  set_adc_channel(3)
   #define ADC3_STR        "THERMISTOR"

   #define ADC_MAX   1023

   #define DAC_MAX_VAL  1024
   
   // PIN_C1 / CCP2 - used as a DAC.  This pin is tied to the big red LED.
   // these definitions used by several of the ex_ezapp_***.c example programs.
   unsigned int16 g_DAC_CCP2;
   void DAC_CCP2_Set(unsigned int16 newVal)
   {
      if (g_DAC_CCP2 != newVal)
      {
         if (newVal == 0)
         {
            setup_ccp2(CCP_OFF);
            output_low(PIN_C1);
         }
         else if (newVal >= 1024)
         {
            setup_ccp2(CCP_OFF);
            output_high(PIN_C1);
         }
         else
         {
            if ((g_DAC_CCP2 == 0) || (g_DAC_CCP2 >= 1024))
            {
               setup_ccp2(CCP_PWM);
            }
            set_pwm2_duty(newVal);
         }
         g_DAC_CCP2 = newVal;
      }
   }

   unsigned int16 DAC_CCP2_Get(void)
   {
      return(g_DAC_CCP2);
   }

   void DAC_CCP2_Init()
   {
      g_DAC_CCP2 = -1;

      //10 bit DAC resolution, max frequency
      setup_timer_2(T2_DIV_BY_1, 0xff, 16);

      DAC_CCP2_Set(0);
   }   

   // PIN_C1 - big red LED
   #define DAC0_INIT()  DAC_CCP2_Init()
   #define DAC0_SET(y)  DAC_CCP2_Set(y)
   #define DAC0_MAX     1023
   #define DAC0_STR     "BIG LED"

   #define LED0_PIN        PIN_E0
   #define LED0_STR        "RED LED"

   #define LED1_PIN        PIN_E1
   #define LED1_STR        "YELLOW LED"

   #define LED2_PIN        PIN_E2
   #define LED2_STR        "GREEN LED"

   #define LED_ON(x)       output_high(x)
   #define LED_OFF(x)      output_low(x)
   #define LED_SET(pin, val)  (val ? output_high(pin) : output_low(pin))

   #define DIGOUT0_PIN  PIN_D2
   #define DIGOUT0_STR  "RELAY0"
   #define DIGOUT1_PIN  PIN_D5
   #define DIGOUT1_STR  "RELAY1"
   #define DIGOUT_SET(pin, val)  (val ? output_high(pin) : output_low(pin))
   
   #define BUTTON0_PRESSED()  (!input(PIN_D3))
   #define BUTTON1_PRESSED()  (!input(PIN_B4))   
   
   //144 ms is to wait for the oscillator/pll to get stable
   #define HW_INIT() \
      setup_adc_ports(sAN0 | sAN1 | sAN2 | sAN3);  \
      setup_comparator(NC_NC_NC_NC);   \
      HW_INIT_BLUETOOTH_FLOWCONTROL(); \
      delay_ms(144); \
      enable_interrupts(GLOBAL)
   
   #if 1
      #define _UART_CONSOLE_APIS_PROVIDED
      #include <UART2.c>
      #define UARTConsoleInit(baud)    UART2Init(baud)
      #define UARTConsolePuts(s)       UART2Puts(s)
      #define UARTConsolePutsROM(s)    UART2PutsROM(s)
      #define UARTConsolePutc(c)       UART2Putc(c)
      #define UARTConsoleKbhit()       UART2Kbhit()
      #define UARTConsoleGetc()        UART2Getc()
      #define UARTConsoleTask()
      #define STREAM_SERIAL_INPUT STREAM_UART_UART2
   #endif
#endif

#if defined(EZAPP_HW_IS_RN4020_PICTAIL)
   #include <18LF25K50.h>
   #device ADC=10

   #use delay(int=8MHz, clock=48MHz, USB_FULL, act=USB)
   
   #fuses NOWDT
   
   // comment this line out to disable the USB CDC connection, which if enabled
   // will act as a debug console.
   #include <usb_cdc.h>
   
   #define PIN_USB_SENSE         PIN_C0
   
   #define PIN_BT_MLDP_CONNECTED  PIN_B5
   #define PIN_BT_MLDP_CMD_MODE  PIN_A2
   #define PIN_BT_SWAKE          PIN_B3
   #define PIN_BT_CTS            PIN_A4
   #define PIN_BT_RTS            PIN_B2
   #define STREAM_BT_SPP_UART 1
   #define STREAM_BT_SPP_BUFFER  32
   #include <RN4020_MLDP_Bluetooth.h>
   
   #define ADC_READ()      FakeADCRead()
   #define ADC_INIT()      
   #define ADC0_CHANNEL()  
   #define ADC0_STR        "FAKEADC"
   #define ADC_MAX   1024
   unsigned int16 g_FakeADCRead = 0;
   unsigned int16 FakeADCRead(void)
   {
      return(g_FakeADCRead);
   }

   #define DAC0_INIT()  
   #define DAC0_SET(y)  
   #define DAC0_MAX     1023
   #define DAC0_STR     "NODAC0"

   #define DAC1_INIT()  
   #define DAC1_SET(y)  
   #define DAC1_MAX     1023
   #define DAC1_STR     "NODAC1"

   #define LED0_PIN        0
   #define LED0_STR        "NOLED0"

   #define LED1_PIN        0
   #define LED1_STR        "NOLED1"

   #define LED2_PIN        0
   #define LED2_STR        "NOLED2"

   #define LED3_PIN        0
   #define LED3_STR        "NOLED3"

   #define LED_ON(x)
   #define LED_OFF(x)   
   #define LED_SET(pin, val)

   #define DIGOUT0_PIN  0
   #define DIGOUT0_STR  "EMPTY"
   #define DIGOUT1_PIN  0
   #define DIGOUT1_STR  "EMPTY"
   #define DIGOUT_SET(pin, val)

  #if defined(__PIC18_USB_H__)
   // usb_cdc.h was included, use the USB CDC for the debug console
   #define _UART_CONSOLE_APIS_PROVIDED
   #define UARTConsoleInit(baud)    usb_init()
   void MyCDCConsolePutc(char c)    {if (usb_enumerated()) usb_cdc_putc(c);}
   #define UARTConsolePuts(s)       printf(MyCDCConsolePutc, s)
   #define UARTConsolePutsROM(s)    printf(MyCDCConsolePutc, s)   
   #define UARTConsolePutc(c)       MyCDCConsolePutc(c)
   #define UARTConsoleKbhit()       usb_cdc_kbhit()
   #define UARTConsoleGetc()        usb_cdc_getc()
   #define UARTConsoleTask()        usb_task()
  #endif

   #define HW_INIT()                   \
      setup_adc_ports(NO_ANALOGS);     \
      setup_comparator(NC_NC_NC_NC);   \
      output_low(PIN_BT_RTS);          \
      enable_interrupts(GLOBAL)
  
   unsigned int32 g_FakeADCTick;
   #define HW_TASK()                                           \
      if ((TickGet() - g_FakeADCTick) >= TICKS_PER_SECOND) \
      {                                                        \
         g_FakeADCTick = get_ticks();                          \
         g_FakeADCRead += ADC_MAX/8;                                 \
         if (g_FakeADCRead > ADC_MAX)                             \
            g_FakeADCRead = 0;                              \
      }                                                        \
      delay_cycles(1)

#endif

#if defined(EZAPP_HW_IS_CCS_DSPAUDIO_RN4020)
   #include <33EP256MU806.h>
   #device ADC=10

   #use delay(crystal=12MHz, clock=120MHz) //60MIPS
   
   #fuses NOWDT
   
 /* 
   RN4020 daughterboard pin# -> Development Kit Header I/O pin#
   #1 (GND) -> #33 (GND)                  #14 (3.3V) -> #1 (3.3V)
   #2 (TX) -> #9 (PIN_F5/RX)              #13 (SWAKE) -> #15 (PIN_F3)
   #3 (RX) -> #12 (PIN_F4/TX)             #12 (RTS) -> #19 (PIN_G8)
   #4 (Conn) -> #10 (PIN_D0)              #11 (CTS) -> NC
   #5 (CMD) -> #14 (PIN_B1)               #10 (WAKE-10) -> NC
   #6 (NC) -> NC                          #9 (NC) -> NC
   #7 (NC) -> NC                          #8 (NC) -> NC
 */
   #define PIN_BT_MLDP_CONNECTED  PIN_D0
   #define PIN_BT_MLDP_CMD_MODE   PIN_B1
   #define PIN_BT_SWAKE           PIN_F3  //wake_sw, pin#7
   #define PIN_BT_RTS             PIN_G8  //rts, pin#18
   #pin_select U1TX=PIN_F4
   #pin_select U1RX=PIN_F5
   #define STREAM_BT_SPP_UART 1
   #define STREAM_BT_SPP_BUFFER  32
   #include <RN4020_MLDP_Bluetooth.h>

   #define ADC_READ()      read_adc()
   #define ADC_INIT()      setup_adc(ADC_CLOCK_INTERNAL)
   //PIN_B8
   #define ADC0_CHANNEL()  set_adc_channel(8)
   #define ADC0_STR        "POT_AN8"
   //PIN_B9
   #define ADC1_CHANNEL()  set_adc_channel(9)
   #define ADC1_STR        "POT_AN9"
   #define ADC_MAX   1023

   #define DAC0_INIT()  
   #define DAC0_SET(y)  
   #define DAC0_MAX     1023
   #define DAC0_STR     "NODAC0"

   #define DAC1_INIT()  
   #define DAC1_SET(y)  
   #define DAC1_MAX     1023
   #define DAC1_STR     "NODAC1"

   #define LED0_PIN        PIN_D10
   #define LED0_STR        "RED LED"

   #define LED1_PIN        PIN_G9
   #define LED1_STR        "YELLOW LED"

   #define LED2_PIN        PIN_F1
   #define LED2_STR        "GREEN LED"

   #define LED3_STR        "NOLED3"

   #define LED_ON(x)          output_high(x)
   #define LED_OFF(x)         output_low(x)
   #define LED_SET(pin, val)  (val ? LED_ON(pin) : LED_OFF(pin))

   #define DIGOUT0_STR  "EMPTY"
   #define DIGOUT1_STR  "EMPTY"
   #define DIGOUT_SET(pin, val)

   #define BUTTON0_PRESSED()  (!input(PIN_B2))
   #define BUTTON1_PRESSED()  (!input(PIN_B11))    

   #define HW_INIT()                   \
      setup_comparator(0, NC_NC);   \
      setup_comparator(1, NC_NC);   \
      setup_comparator(2, NC_NC);   \
      setup_adc_ports(sAN8 | sAN9);     \
      output_low(PIN_BT_RTS); \
      delay_ms(200); \
      enable_interrupts(GLOBAL)
   
   #define _UART_CONSOLE_APIS_PROVIDED
   #pin_select U2TX = PIN_E7
   #pin_select U2RX = PIN_D11
   #include <UART2.c>
   #define UARTConsoleInit(baud)    UART2Init(baud)
   #define UARTConsolePuts(s)       UART2Puts(s)
   #define UARTConsolePutsROM(s)    UART2PutsROM(s)
   #define UARTConsolePutc(c)       UART2Putc(c)
   #define UARTConsoleKbhit()       UART2Kbhit()
   #define UARTConsoleGetc()        UART2Getc()
   #define UARTConsoleTask()
   #include <pcd_traps.c>
   #define STREAM_SERIAL_INPUT STREAM_UART_UART2
#endif

#if defined(EZAPP_HW_IS_CCS_E3_HC06)
   #include <e3.h>
   
   #define __BLUETOOTH_DRIVER_OVERRIDE__
   #include <UART1.c>
   #define BtSppIsConnected() (TRUE)
   void BtSppInit(void) {UART1Init(9600);}
   inline bool BtSppKbhit(void)
   {
      return(UART1Kbhit());
   }
   inline char BtSppGetc(void)
   {
      return(UART1Getc());
   }
   inline void BtSppPutc(char c)
   {
      UART1Putc(c);
   }
   #define BtSppFlush()
   #define BtSppIsPutReady() (1)

   #if 0
      #define _UART_CONSOLE_APIS_PROVIDED
      #define UARTConsoleInit(baud)
      #define UARTConsolePuts(s) fprintf(STREAM_USB_UART, s)
      #define UARTConsolePutsROM(s) fprintf(STREAM_USB_UART, s)
      #define UARTConsolePutc(c) fputc(c, STREAM_USB_UART)
      #define UARTConsoleKbhit() kbhit(STREAM_USB_UART)
      #define UARTConsoleGetc() fgetc(STREAM_USB_UART)
      #define UARTConsoleTask()
   #endif
   
   #define ADC_READ()      read_adc()
   #define ADC_INIT()      setup_adc_ports(sAN4); setup_adc(ADC_CLOCK_INTERNAL)
   #define ADC0_CHANNEL()  set_adc_channel(4);
   #define ADC0_STR        "POT"
   #define ADC_MAX   256

   #define DAC0_INIT()  
   #define DAC0_SET(y)  
   #define DAC0_MAX     1023
   #define DAC0_STR     "NODAC0"

   #define DAC1_INIT()  
   #define DAC1_SET(y)  
   #define DAC1_MAX     1023
   #define DAC1_STR     "NODAC1"

   #define LED0_PIN        PIN_C6
   #define LED0_STR        "GREEN"

   #define LED1_PIN        PIN_C4
   #define LED1_STR        "YELLOW"

   #define LED2_PIN        PIN_C3
   #define LED2_STR        "RED"

   #define LED3_PIN        0
   #define LED3_STR        "NOLED3"

   #define LED_ON(x)
   #define LED_OFF(x)   
   #define LED_SET(pin, val)

   #define DIGOUT0_PIN  0
   #define DIGOUT0_STR  "EMPTY"
   #define DIGOUT1_PIN  0
   #define DIGOUT1_STR  "EMPTY"
   #define DIGOUT_SET(pin, val)
   
   #define HW_INIT() enable_interrupts(GLOBAL)
  
   #define HW_TASK()
#endif

#if defined(EZAPP_HW_IS_CCS_SERVO)
   #include <24FJ256GB206.h>

   #device ICSP=2 //PIN_B5, PIN_B6 and PIN_B7 used for ICD
   
   #device ADC=10

   #use delay(crystal=20MHz)
   
   #fuses NOWDT
   
 /* 
   RN4020 daughterboard pin# -> Development Kit Header I/O pin#
   #1 (GND) -> #33 (GND)                  #14 (3.3V) -> #1 (3.3V)
   #2 (TX) -> #9 (PIN_D8/RX)              #13 (SWAKE) -> #15 (PIN_F1)
   #3 (RX) -> #12 (PIN_D10/TX)            #12 (RTS) -> #19 (PIN_E2)
   #4 (Conn) -> #10 (PIN_E1)              #11 (CTS) -> NC (PIN_E3)
   #5 (CMD) -> #14 (PIN_E0)               #10 (WAKE-10) -> NC (PIN_G8)
   #6 (NC) -> NC (PIN_C6)                 #9 (NC) -> NC (PIN_C14)
   #7 (NC) -> NC (PIN_B8)                 #8 (NC) -> NC (PIN_B13)
 */
   #define PIN_BT_MLDP_CONNECTED  PIN_E1
   #define PIN_BT_MLDP_CMD_MODE   PIN_E0
   #define PIN_BT_SWAKE           PIN_F1  //wake_sw, pin#7
   #define PIN_BT_RTS             PIN_E2  //rts, pin#18
   #pin_select U1TX=PIN_D10
   #pin_select U1RX=PIN_D8
   #define STREAM_BT_SPP_UART 1
   #define STREAM_BT_SPP_BUFFER  32
   #include <RN4020_MLDP_Bluetooth.h>

   #define ADC_READ()      read_adc()
   #define ADC_INIT()      setup_adc(ADC_CLOCK_INTERNAL)
   //PIN_B8
   #define ADC0_CHANNEL()  set_adc_channel(0)
   #define ADC0_STR        "POT_AN8"

   #define ADC1_CHANNEL()  ADC0_CHANNEL()
   #define ADC1_STR        ADC0_STR
   #define ADC_MAX   1023

   #define DAC0_INIT()  
   #define DAC0_SET(y)  
   #define DAC0_MAX     1023
   #define DAC0_STR     "NODAC0"

   #define DAC1_INIT()  
   #define DAC1_SET(y)  
   #define DAC1_MAX     1023
   #define DAC1_STR     "NODAC1"

   #define LED0_PIN        PIN_B9
   #define LED0_STR        "RED LED"

   #define LED1_PIN        PIN_B10
   #define LED1_STR        "YELLOW LED"

   #define LED2_PIN        PIN_B11
   #define LED2_STR        "GREEN LED"

   #define LED3_STR        "NOLED3"

   #define LED_ON(x)          output_high(x)
   #define LED_OFF(x)         output_low(x)
   #define LED_SET(pin, val)  (val ? LED_ON(pin) : LED_OFF(pin))

   #define DIGOUT0_STR  "EMPTY"
   #define DIGOUT1_STR  "EMPTY"
   #define DIGOUT_SET(pin, val)

   #define BUTTON0_PRESSED()  (!input(PIN_F5))
   #define BUTTON1_PRESSED()  (FALSE)

   #define HW_INIT()                   \
      setup_comparator(1, NC_NC);   \
      setup_comparator(2, NC_NC);   \
      setup_comparator(3, NC_NC);   \
      setup_adc_ports(sAN0);  \
      output_low(PIN_BT_RTS); \
      delay_ms(200); \
      enable_interrupts(GLOBAL)
   
   #define _UART_CONSOLE_APIS_PROVIDED
   #pin_select U2TX = PIN_D5
   #pin_select U2RX = PIN_D4
   #include <UART2.c>
   #define UARTConsoleInit(baud)    UART2Init(baud)
   #define UARTConsolePuts(s)       UART2Puts(s)
   #define UARTConsolePutsROM(s)    UART2PutsROM(s)
   #define UARTConsolePutc          UART2Putc
   #define UARTConsoleKbhit()       UART2Kbhit()
   #define UARTConsoleGetc()        UART2Getc()
   #define UARTConsoleTask()
   #include <pcd_traps.c>
   #define STREAM_SERIAL_INPUT STREAM_UART_UART2
#endif

#if defined(EZAPP_HW_IS_CCS_ROBOT)
   //#define CONFIG_UART_CONSOLE_FROM_USB_CDC

   #include <devkit_picrobot.h>
   
   #include <RN4020_MLDP_Bluetooth.h>
   
   #define ADC_READ()      read_adc()
   #define ADC_INIT()      setup_adc(ADC_CLOCK_INTERNAL | ADC_TAD_MUL_2)
   #define ADC0_CHANNEL()  set_adc_channel(AN_CHANNEL_LEFT_LINE); delay_us(20)
   #define ADC0_STR        "L LINE"
   #define ADC1_CHANNEL()  set_adc_channel(AN_CHANNEL_RIGHT_LINE); delay_us(20)
   #define ADC1_STR        "R LINE"
   #define ADC2_CHANNEL()  set_adc_channel(AN_CHANNEL_LEFT_PROXIMITY); delay_us(20)
   #define ADC2_STR        "L PROX"
   #define ADC3_CHANNEL()  set_adc_channel(AN_CHANNEL_RIGHT_PROXIMITY); delay_us(20)
   #define ADC3_STR        "R PROX"

   #define ADC_MAX   1023

   // PIN_C1 - big red LED
   #define DAC0_INIT()
   #define DAC0_SET(y)
   #define DAC0_MAX     1023
   #define DAC0_STR     "FAKE"

   #define DIGOUT0_PIN  WHEEL_MOTOR_IDX_LEFT
   #define DIGOUT0_STR  "LEFT"
   #define DIGOUT1_PIN  WHEEL_MOTOR_IDX_RIGHT
   #define DIGOUT1_STR  "RIGHT"
   #define DIGOUT_SET(_pin, _val)  _MY_DIGOUT_SET(_pin, _val)
   void _MY_DIGOUT_SET(unsigned int8 pin, unsigned int8 val)  //funky function to replace DIGOUT_SET on Robot boards
   {
      static int1 init;
      signed int8 speed;
      
      if (val)
         speed = 100;
      else
         speed = 0;
         
      if (!init)
      {
         WheelMotorInit();
         init = 1;
      }
      
      WheelMotorSetSpeed(pin, speed);
   }
   
   #define BUTTON0_PRESSED()  (BUTTON_PRESSED())
   #define BUTTON1_PRESSED()  (FALSE)
   
   #if defined(CONFIG_UART_CONSOLE_FROM_USB_CDC)
      #define _UART_CONSOLE_APIS_PROVIDED
   #endif
   
   #if !defined(DEVICE_USE_ICD) && 0
      // map ICD ICSP connector to UART2 and Console
      #define _UART_CONSOLE_APIS_PROVIDED
      #pin_select U2RX=PIN_B1
      #pin_select U2TX=PIN_B0
      #include <UART2.c>
      #hexcomment\SETTINGS MONITOR=19200
      #define UARTConsoleInit(baud)    UART2Init(baud); delay_ms(3000); restart_wdt()
      #define UARTConsolePuts(s)       UART2Puts(s)
      #define UARTConsolePutsROM(s)    UART2PutsROM(s)
      #define UARTConsolePutc(c)       UART2Putc(c)
      #define UARTConsoleKbhit()       UART2Kbhit()
      #define UARTConsoleGetc()        UART2Getc()
      #define UARTConsoleTask()
      #define STREAM_SERIAL_INPUT STREAM_UART_UART2
      #include <pcd_traps.c>
      
      #define _DO_DEBUG_PRINTF_STREAM STREAM_SERIAL_INPUT
   #else
      #define TRAPS_HAS_NO_PUTC
      #include <pcd_traps.c>
   #endif   
#endif

#include <stdint.h>
#include <stdbool.h>

#if !defined(_UART_CONSOLE_APIS_PROVIDED)
   #define UARTConsoleInit(baud)
   #define UARTConsolePuts(s)
   #define UARTConsolePutsROM(s)
   #define UARTConsolePutc(c)
   #define UARTConsoleKbhit() (false)
   #define UARTConsoleGetc() (0)
   #define UARTConsoleTask()
#endif

#ifdef _DO_DEBUG_PRINTF_STREAM
   #define debug_printf printf
   void debug_putc(char c) {fputc(c, _DO_DEBUG_PRINTF_STREAM);}
   #define debug_printf0(a)   fprintf(_DO_DEBUG_PRINTF_STREAM, a)
   #define debug_printf1(a,b)   fprintf(_DO_DEBUG_PRINTF_STREAM, a, b)
   #define debug_delay(x)  restart_wdt(); delay_ms(x); restart_wdt()
   void debug_array(void *p, int n) {while(n--) {debug_printf1("%02X ", *p++);}}   
#else
   #define debug_printf(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)
   #define debug_putc(c)
   #define debug_printf0(a)
   #define debug_printf1(a,b)
   #define debug_delay(x) 
   #define debug_array(p, n)
#endif

#include <Tick.c>

#if defined(__BT_GET_MODULE_VERSION__) && defined(__RN4020_MLDP_Bluetooth_H__)
   char rn4020Version[32] = {0};
   #define BT_GET_VERSION_INFO rn4020Version
   #define BT_MAX_VERSION_INFO sizeof(rn4020Version)
#endif

#include <Bluetooth_Serial.c>

#if 1
   // giving the EZ App library a tick library will enable some extra timeout
   // protections in the library.  these are optional
   
   #define EZ_APP_TICK_TYPE   TICK
   #define EZ_APP_TICKGET()   TickGet()
   #define EZ_APP_TICKS_PER_SECOND TICKS_PER_SECOND
#endif

#if defined(EXAMPLE_EZAPP_SECURITY)
   // this is located in ex_ezapp_security.c
   bool MyEZAppAuthCheck(char *str);

   #define EZAPP_CHECK_AUTHENTICATION(str) MyEZAppAuthCheck(str)
#endif

#include <EZApp.c>

#ifndef HW_INIT
#define HW_INIT()
#endif

#ifndef HW_TASK
#define HW_TASK()
#endif

#endif
