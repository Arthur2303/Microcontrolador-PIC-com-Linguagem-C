#include <18F4550.h>
#fuses XT, NOWDT, NOMCLR
#use delay(crystal=4MHz)

#INT_EXT1//INT1
void externa1() {
   output_toggle(PIN_D1);
}

#INT_EXT2//INT2
void externa2() {
   output_toggle(PIN_D2);
}

int1 modo = 0;

#INT_EXT//INT
void externa0() {
   output_toggle(PIN_D0);
   if (modo == 0) {
      ext_int_edge( 0, H_TO_L );
   } else {
      ext_int_edge( 0, L_TO_H );
   }
   modo = !modo;
}

void main () {
   
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_EXT1_H2L);//H2L = High to Low, representando uma borda de descida
   enable_interrupts(INT_EXT2_L2H);//L2H = Low to High, representando uma borda de subida
   
   ext_int_edge( 0, L_TO_H );//Configurando a borda de interrupção 0 de Low to High
         
   while(true) {
      
   }

}






