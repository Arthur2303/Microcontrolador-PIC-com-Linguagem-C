#include <18F4550.h>
#fuses XT, NOWDT, MCLR
#use delay(clock=4MHz)

#INT_TIMER0
void interrupcao_t0() {
   clear_interrupt(INT_TIMER0);
   set_timer0(3036);
   output_toggle(PIN_B1);
}

void main () {
   //Habilitando interrupções (Global e TIMER0)
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_TIMER0);
   
   setup_timer_0(T0_INTERNAL | T0_DIV_16);
   set_timer0(3036);
   
   while(true) {
      output_toggle(PIN_B0);
      delay_ms(1000);
   }
   
   
}
