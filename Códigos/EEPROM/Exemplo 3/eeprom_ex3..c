#include <18F4550.h>
#device adc=10
#fuses HS, NOWDT, MCLR
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

typedef unsigned int uint;

typedef union {
   float f;
   uint i[4];
} Conversor;

void main () {

   float tensao;
   Conversor dado;
   unsigned int16 leitura;
   
   lcd_init();
   
   setup_adc( ADC_CLOCK_INTERNAL );
   setup_adc_ports( AN0 );
   set_adc_channel (0);
   
   write_eeprom(0, 'v');
   
   while(true) {
      
      leitura = read_adc();
      tensao = (float)(leitura) / 1023.0 * 5.0;
            
      if ( input(PIN_B0) == 0 ) {
         
         dado.f = tensao;
         
         write_eeprom(1, dado.i[0]);
         delay_ms(10);
         
         write_eeprom(2, dado.i[1]);
         delay_ms(10);
         
         write_eeprom(3, dado.i[2]);
         delay_ms(10);
         
         write_eeprom(4, dado.i[3]);
         delay_ms(10);
         
      } else {
         
         char letra = read_eeprom(0);
         dado.i[0] = read_eeprom(1);
         dado.i[1] = read_eeprom(2);  
         dado.i[2] = read_eeprom(3);
         dado.i[3] = read_eeprom(4);
         
         float salvo = dado.f;
         printf(LCD_PUTC, "\fDado: %f", tensao);
         printf(LCD_PUTC, "\nSalvo: %c %f", letra, salvo);
         delay_ms(100);
         
      }
      
   }
   
}







