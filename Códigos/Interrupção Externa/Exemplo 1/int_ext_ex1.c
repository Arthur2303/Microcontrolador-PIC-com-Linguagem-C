#include <18F4550.h>
#fuses XT, NOWDT, NOMCLR
#use delay(clock=4MHz)

/* Para utilizar o pino INT1 seria necess?rio ter a diretiva como
   #INT_EXT1 para o pinto INT2 #INT_EXT2.*/
#INT_EXT//Diretiva da fun??o abaixo
void int_externa1(){
   output_toggle(PIN_D6);
}

void main (){

   enable_interrupts(GLOBAL);
   enable_interrupts(INT_EXT);
   
   while(true){
      
      output_toggle(PIN_D7);
      delay_ms(500);
      
   }
}






