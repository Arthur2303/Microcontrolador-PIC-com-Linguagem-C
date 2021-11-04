#include <18F4550.h>
#fuses INTRC, NOWDT, NOMCLR
#use delay(clock=4MHz)

#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#include <lcd.c>

#INT_RDA
void serial_int() {
   
   char temp;
   
   while ( kbhit() ) {
      temp = getc();
      if (temp == '#') {
         printf(LCD_PUTC, "\f");//Limpa Display
      } else {
         printf(LCD_PUTC, "%c", temp);
      }
   }
   
}

void main() {
   
   lcd_init();
   
   enable_interrupts(GLOBAL);   
   enable_interrupts(INT_RDA);
   
   printf(LCD_PUTC, "\fINICIANDO...");
   delay_ms(1000);
   
   while(true) {
   
   }
}






