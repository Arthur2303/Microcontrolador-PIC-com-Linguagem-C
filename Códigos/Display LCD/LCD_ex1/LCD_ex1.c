#include <18F4550.h>
#fuses HS, NOMCLR, NOWDT
#use delay(clock=20MHz)

#include <lcd.c>

void main () {
   
   lcd_init();//Inicializa a biblioteca do display
   
   printf(LCD_PUTC, "Meu texto no LCD");//LCD_PUTC é uma referência a uma função do lcd.c
   delay_ms(1000);
   
   printf(LCD_PUTC, "\nTemperatura = %f", 28.6); 
   
}

