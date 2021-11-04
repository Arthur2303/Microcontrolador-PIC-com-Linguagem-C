#include <16F628A.h>
#fuses INTRC, NOWDT, NOMCLR
#use delay(clock=4MHz)

#use rs232(baud=9600, xmit=PIN_B2, rcv=PIN_B1)

void main() {
   
   int1 btn1;
   int1 btn2;
   int1 btn3;
   
   while(true) {
      btn1 = input(PIN_A0);
      btn2 = input(PIN_A1);
      btn3 = input(PIN_A2);
      
      if (btn1 == 1 && btn2 == 0 && btn3 == 0) {
         printf("#AUTO");//O # limpa o display a partir da condi��o feita no receptor.
      } else if (btn1 == 0 && btn2 == 1 && btn3 == 0) {
         printf("#ECONOMICO");
      } else if (btn1 == 0 && btn2 == 0 && btn3 == 1) {
         printf("#DESEMPENHO");
      }
      
      delay_ms(1000);
   }
   
}






