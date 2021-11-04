#include <18F4550.h>
#fuses XT, NOMCLR, NOWDT
#use delay(clock=4MHz, crystal=4MHz)

#define IN1 PIN_C4
#define IN2 PIN_C5

#define IN3 PIN_C6
#define IN4 PIN_C7

void main () {

   unsigned int16 duty1;
   unsigned int16 duty2;

   setup_ccp1(CCP_PWM);
   setup_ccp2(CCP_PWM);
   
   setup_timer_2(T2_DIV_BY_4, 24, 1);
   duty1 = 100; //(unsigned int16)(25 * 4 * (100/100.0));
   duty2 = 100;
   set_pwm1_duty(duty1);
   set_pwm2_duty(duty2);

   while(true) {      
      output_bit(IN1, 1);
      output_bit(IN2, 0);   
      output_bit(IN3, 0);
      output_bit(IN4, 1);
      delay_ms(3000); 
      
      output_bit(IN1, 0);
      output_bit(IN2, 1);   
      output_bit(IN3, 1);
      output_bit(IN4, 0);
      delay_ms(3000);
   }

}







