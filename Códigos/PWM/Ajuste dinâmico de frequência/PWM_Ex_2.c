//Ajuste din?mico de frequ?ncia
#include <18F4550.h>
#device adc=8//ADC como 8 bits
#fuses XT, MCLR, NOWDT
#use delay(clock=4MHz)

#define LCD_DB0   PIN_D0
#define LCD_DB1   PIN_D1
#define LCD_DB2   PIN_D2
#define LCD_DB3   PIN_D3
#define LCD_DB4   PIN_D4
#define LCD_DB5   PIN_D5
#define LCD_DB6   PIN_D6
#define LCD_DB7   PIN_D7

#define LCD_E     PIN_E1
#define LCD_RS    PIN_E2

#include <flex_lcd.c>

void main() {

   lcd_init();
   
   unsigned int16 P;
   unsigned int16 PR2;
   unsigned int16 duty;
   
   float t_pwm;
   
   setup_adc_ports(AN0);//Configurando AN) como um input anal?gico
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(0);
   
   P = 50;   
   setup_ccp1(CCP_PWM);
      
   while(true) {
      PR2 = read_adc();//l? do AN0 e guarda em PR2
      setup_timer_2(T2_DIV_BY_16, PR2, 1);      
      duty = (unsigned int16)((PR2 + 1) * 4 * (P/100.0));
      set_pwm1_duty(duty);
      
      t_pwm = (float)((PR2 + 1) * 4.0 * (1/4000000.0) * 16);
      printf(LCD_PUTC, "\fT PWM: %.8f", t_pwm);// \f limpa display
      printf(LCD_PUTC, "\n%lu | %f", PR2, 1/t_pwm);
      
      delay_ms(1000);
   }
}




