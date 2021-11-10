#include <18F4550.h>
#fuses INTRC, NOWDT, NOMCLR // NOWDT reinicia o código a cada travamento.
#use delay(clock=4MHz)

#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#INT_RDA
void serial_int() {

   char temp;
   /* A função kbhit() retorna 1 se tiver dado disponível e 
      0 se não tiver nenhum disponível.*/
   while( kbhit() ) {
      temp = getc();//Faz a leitura do dado disponível no buffer serial.
      
      if (temp == 'L') {
         output_bit(PIN_D0, 1);
         printf("-> LIGOU | ");
      }
      if (temp == 'D') {
         output_bit(PIN_D0, 0);
         printf("-> DESLIGOU | ");
      }
   }
   
}

void main () {

   enable_interrupts(GLOBAL);//Interrupção Global.
   enable_interrupts(INT_RDA);//Interrupção Serial.

   while(true) {
   
   }

}

