//Executa interrup??es nos led(atraso ou adiantamento de um em rela??o a um outro.)
#include <16F628A.h>
#fuses XT, MCLR, NOWDT
#use delay(clock=4MHz)

int contador = 0;

#INT_TIMER0 // M?dulo TIMER0 pode ser de 8 ou 16 bits, dependendo do Microcontrolador
void interrupcao_t0() {//Temporiza??o ? separada da execu??o da fun??o main.
   clear_interrupt(INT_TIMER0);
   if (contador == 10) {
      output_toggle(PIN_B1);//alterna o estado do bit(alto ou baixo)
      contador = 0;
   }
   contador++;
}

void main() {

   enable_interrupts(GLOBAL);
   enable_interrupts(INT_TIMER0);
   
   setup_timer_0(T0_INTERNAL | T0_DIV_256);
   
   while (true) {
      output_toggle(PIN_B0);
      delay_ms(1000);
   }   
}
