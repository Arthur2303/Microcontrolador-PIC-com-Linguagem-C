#include <18F4550.h>
#fuses XT, MCLR, NOWDT
#use delay(clock=4MHz)

void main(){

   unsigned int16 P = 50, PR2 = 124, duty;
   
   setup_timer_2(T2_DIV_BY_16,PR2,1);//Determina o valor da frequência no PWM
   setup_ccp1(CCP_PWM);//Configura o CCP1 como PWM
   
   duty = (unsigned int16)((PR2+1)*4*(P/100.0));
   set_pwm1_duty(duty);//Define o duty cycle do pwm1 para variável duty
   
   while(true){
      
   }
   
}


