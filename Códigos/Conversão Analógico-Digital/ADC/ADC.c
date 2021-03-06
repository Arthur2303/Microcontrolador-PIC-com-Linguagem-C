/*Lendo uma tens?o com Conversor AD 
NOTA: Os pinos do Modulo Conversor Anal?gico Digital(ADC) s?o utilizados por padr?o como bits de entrada e sa?da(E/S).
No entanto, ? poss?vel escolher se cada pino ser? utilizado como E/S ou leitura anal?gica.*/
#include <18F4550.h>
#device adc=10 // Defini??o de quantos bits o conversor ir? utilizar. OBS: 18F4550 pode 8 ou 10 bit
#fuses HS, NOMCLR, NOWDT
#use delay(clock=20MHz)

#include <lcd.c>

void main () {

   unsigned int16 valor;
   float tensao;
   
   lcd_init();
   
   setup_adc( ADC_CLOCK_INTERNAL );//Configuando o m?dulo com a origem do clock interno.
   setup_adc_ports( AN0 );//Espec?fica a porta anal?gica que ser? utilizada comco inputs.
   set_adc_channel(0);//Determinando o canal ativo.
   
   while(true) {
      valor = read_adc();//L?-se a porta ativada pela fun??o set_adc_channel e guarda em valor
      tensao = valor / 1023.0 * 5.0;
      printf(LCD_PUTC, "\fValor: %lu\n", valor);// %lu: long unsigned int ou unsigned int 16
      printf(LCD_PUTC, "Tensao: %f", tensao);
      delay_ms(100);
   }
   
   
}





