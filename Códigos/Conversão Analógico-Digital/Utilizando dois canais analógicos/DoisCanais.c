//Utilzando dois canais anal?gicos, sendo cada um deles respectivo a um ponteci?metro. 
#include <18F4550.h>
#device adc=10
#fuses HS, MCLR, NOWDT
#use delay(clock=20MHz)

#define LCD_DB0   PIN_D0
#define LCD_DB1   PIN_D1
#define LCD_DB2   PIN_D2
#define LCD_DB3   PIN_D3
#define LCD_DB4   PIN_D4
#define LCD_DB5   PIN_D5
#define LCD_DB6   PIN_D6
#define LCD_DB7   PIN_D7

#define LCD_E     PIN_E1
#define LCD_RS    PIN_E2

#include <flex_lcd.c>

void main () {

   unsigned int16 pot1;
   unsigned int16 pot2;
   
   lcd_init();  
   
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(AN0_TO_AN1);
   
   
   while(true) {
      set_adc_channel(0);
      delay_ms(10);
      pot1 = read_adc();
      delay_ms(10);
      
      set_adc_channel(1);
      delay_ms(10);
      pot2 = read_adc();
      delay_ms(10);
      
      printf(LCD_PUTC, "\fPOT 1: %lu\nPOT 2: %lu", pot1, pot2);
   }
}
















