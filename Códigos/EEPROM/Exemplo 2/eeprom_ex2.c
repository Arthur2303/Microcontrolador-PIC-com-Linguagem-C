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

void main () {

   unsigned int16 leitura;
   unsigned int baixo;
   unsigned int alto;
   
   lcd_init();
   
   setup_adc( ADC_CLOCK_INTERNAL );
   setup_adc_ports( AN0 );
   set_adc_channel (0);
   
   write_eeprom(0, 'v');
   
   while(true) {
      
      leitura = read_adc();
      
      if ( input(PIN_B0) == 0 ) {
      //make8 ? uma fun??o que recebe uma vari?vel de at? 32 bits e separa em partes de 8 bits
         baixo = make8(leitura, 0);//Extraindo a parte menos significativa
         alto = make8(leitura, 1);//Extraindo a parte mais significativa
         
         write_eeprom(1, baixo);
         delay_ms(10);
         
         write_eeprom(2, alto);
         delay_ms(10);
         
      } else {
         
         char letra = read_eeprom(0);
         baixo = read_eeprom(1);
         alto = read_eeprom(2);

         unsigned int16 salvo = make16(alto, baixo);
         printf(LCD_PUTC, "\fLEITURA: %lu", leitura);
         printf(LCD_PUTC, "\nSALVO: %c %lu", letra, salvo); 
         delay_ms(100);
         
      }
      
   }
   
}







