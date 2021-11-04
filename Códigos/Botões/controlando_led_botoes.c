#include <16F628A.h>
#fuses XT
#use delay( clock=4MHz )

void  main () {
   
   int contador = 0;
   int1 btn1;
   int1 btn2;
   
   while (true) {
      btn1 = input ( PIN_A1 );
      btn2 = input ( PIN_A2 );
      if ( btn1 == 0 ) {
         contador++; // contador = contador + 1;
         output_b ( contador );
         while ( input(PIN_A1) == 0);
      }
      if ( btn2 == 0 ) {
         contador--;
         output_b( contador );
         while ( input(PIN_A2) == 0);
      }
   }
   
}
