#include <16F628A.h>
#fuses XT
#use delay( clock=4MHz )

void main () {
      
   int16 intervalo = 250;
   int saidaB = 0;
      
   while(true) {
      output_b(saidaB);
      saidaB = saidaB + 1;
      delay_ms(intervalo);
   }
}
