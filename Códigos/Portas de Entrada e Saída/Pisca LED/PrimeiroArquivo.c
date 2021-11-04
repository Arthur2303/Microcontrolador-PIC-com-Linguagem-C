#include <16F628A.h> //Arquivo de definições do chip/microcontrolador a ser utilizado
#fuses XT //fuses/fusíveis são os bits de configuração
#use delay (clock = 4MHz)

void main(){

   while(true){
      output_high(PIN_B0);
      delay_ms(1000);
      output_low(PIN_B0);
      delay_ms(1000);
   }
}
