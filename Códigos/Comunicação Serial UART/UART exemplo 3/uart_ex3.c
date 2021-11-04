#include <18F4550.h>
#device adc=10
#fuses XT, NOWDT, NOMCLR
#use delay(crystal=4MHz)

#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#INT_RDA
void serial_int() {

   char temp;
   
   while( kbhit() ) {
      temp = getc();
      
      if (temp == 'L') {
         output_bit(PIN_D0, 1);
         printf("-> LIGOU \n\r");
      }
      if (temp == 'D') {
         output_bit(PIN_D0, 0);
         printf("-> DESLIGOU \n\r");
      }
   }
}

void main () {

   enable_interrupts(GLOBAL);
   enable_interrupts(INT_RDA);
   
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(AN0);
   set_adc_channel(0);
   
   unsigned int16 luminosidade; 
   
   while(true) {
      luminosidade = read_adc();
      printf("LUMINOSIDADE %lu \n\r", luminosidade);
      delay_ms(1000);
   }
}






