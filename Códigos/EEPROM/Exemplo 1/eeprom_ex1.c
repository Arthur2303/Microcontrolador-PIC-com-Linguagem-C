#include <16F628A.h>// EEPROM com 128 bytes
#fuses XT
#use delay( clock=4MHz )

void  main () {
   
   int addr = 0;
   unsigned int valor;
   
   valor = read_eeprom(addr);//Faz a leitura do EEPROM para vari�vel valor
   if (valor == 0) {
      write_eeprom(addr, 130);//Escreve na EEPROM interna do chip
   } else {
      write_eeprom(addr, valor - 1);
   }   
   output_b(valor);
     
   while (true) {
      
   }
   
}

