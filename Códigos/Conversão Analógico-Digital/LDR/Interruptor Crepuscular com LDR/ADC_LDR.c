//LDR(Light Dependent Resistor)
#include <12F675.h>
#device adc=10
#fuses NOMCLR, INTRC
#use delay(clock=4MHz)

void main () {
   
   unsigned int16 ldr;
   
   setup_adc( ADC_CLOCK_INTERNAL );
   setup_adc_ports(sAN0);
   set_adc_channel(0);
   
   while(true) {
      ldr = read_adc();
      delay_ms(10);
      
      if ( ldr > 512 ) {
         output_bit(PIN_A1, 1);
      } else {
         output_bit(PIN_A1, 0);
      }
   }
   
}  








