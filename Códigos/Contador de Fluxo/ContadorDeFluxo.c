/*Para isso foi solicitado o desenvolvimento um aparelho  "contador de fluxo". Esse aparelho deve ser operado por uma pessoa que deve selecionar o sem?foro (de 0 a 4) e 
pressionar um bot?o para incrementar em uma unidade quantos ve?culos passaram pelo sem?foro selecionado. A contagem deve ser armazenada na EEPROM para que n?o ocorram perdas 
por falta de bateria ou desligamento acidental.
Os bot?es RB0 e RB1 devem ser utilizados selecionar o sem?foro entre as op??es de 0 ? 4, onde o RB0 move a sele??o para o sem?foro anterior, caso seja maior que zero, e o RB1
move a sele??o para o sem?foro seguinte, caso seja menor que 4.
O sem?foro selecionado deve ser indicado pelos LEDs B3 ? B7 . Assim quando, por exemplo, sem?foro 0 for selecionado o LED B3 fica aceso e os outros 4 apagados.
O bot?o RB2 deve ser utilizado para incrementar o valor da contagem de fluxo de ve?culos do sem?foro selecionado, devendo a contagem sempre iniciar com 0.
O resultado da contagem deve ser salvo na mem?ria EEPROM do chip no mesmo endere?o indicado pela sele??o do sem?foro. Ent?o quando o sem?foro 0 for selecionado a contagem 
referente a ele deve ser salva na posi??o 0 da EEPROM; quando o sem?foro 1 for selecionado a contagem referente a ele deve ser salva na posi??o 1 da EEPROM, e assim por diante.
Sempre que um sem?foro for selecionado sua contagem deve ser apresentada no Display LCD. Considere que o valor m?ximo da contagem por sem?foro ser? 200.
*/
#include <18F4550.h>
#fuses HS, NOWDT, MCLR
#use delay(clock=20MHz)

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

int semaforoSelecionado = 0; 

void mostrarContagem(int semaforo) {
   unsigned int contagem = read_eeprom(semaforo);
   
   //O sem?foro selecionado e a contagem nele ser?o apresentados no Display LCD.
   printf(LCD_PUTC, "\fSemaforo %i", semaforo);
   printf(LCD_PUTC, "\nContagem %i", contagem);
}

/*Esta fun??o recebe como par?metro qual o sem?foro, e consequentemente qual a
   posi??o na EEPROM devemos incrementar.*/
void incrementar(int semaforo) {
   /*Primeiramente ? lido o valor existente na posi??o da EEPROM para uma 
      vari?vel auxilizar aqui definida como contagem.*/
   unsigned int contagem = read_eeprom(semaforo);
   
   contagem++;
   /* Por fim salvamos o novo valor da vari?vel auxiliar na mesma posi??o da
      EEPROM de obtemos o valor anterior caso o valor seja menor que 200.*/
   if (contagem < 200) {
      write_eeprom(semaforo, contagem);
      delay_ms(10);
   }
   mostrarContagem(semaforoSelecionado);
}

void main () {

   lcd_init();    
   
   /* Inicializa as 5 primeiras posi??es da EEPROM com o valor zero caso sejam 
      255.*/
   for (int i = 0; i < 5; i++) {
      unsigned int leitura = read_eeprom(i);
      if (leitura == 255) {
         write_eeprom(i, 0);
         delay_ms(10);
      }
   }
   /* Mostra sem?foro selecionado a partir do bit 3 da porta B. Os bits 
      iniciais, 0 1 e 2, est?o sendo utilizados pelos bot?es.*/
   output_bit(PIN_B3 + semaforoSelecionado, 1);
   
   mostrarContagem(semaforoSelecionado);
   
   while(true) {
   
      /* Ao pressionar o bot?o RB0 decrementa a vari?vel auxiliar 
         semaforoSelecionado case ela seja maior que zero.*/
      if ( input(PIN_B0)  == 0 ) {
         if (semaforoSelecionado > 0) {
            semaforoSelecionado--;
         }
         /*Ap?s decrementar a vari?vel semaforoSelecionado zera os bits
            da porta B de 3 ? 7.*/
         for (int i = 0; i < 5; i++) {
            output_bit(PIN_B3 + i, 0);
         }
                  
         /* Para acender apenas o LED correto de acordo com o sem?foro selecionado somamos o valor da posi??o do primeiro bit (PIN_B3) com
            o semaforoSelecionado. Isso funciona porque as constantes PIN_B3 ? PIN_B7 s?o valores inteiros linerares iniciando em PIN_B0 com o 
            valor 31755 e aumentando em uma unidade a para PIN_B4, PIN_B5 etc.*/
         output_bit(PIN_B3 + semaforoSelecionado, 1);
         
         mostrarContagem(semaforoSelecionado);
         
         while (input(PIN_B0)  == 0);
      }
      
      // Agora para o PIN_B1.
      if ( input(PIN_B1)  == 0 ) {
         if (semaforoSelecionado < 4) {
            semaforoSelecionado++;
         }
         
         for (int i = 0; i < 5; i++) {
            output_bit(PIN_B3 + i, 0);
         }
         
         output_bit(PIN_B3 + semaforoSelecionado, 1);
         
         mostrarContagem(semaforoSelecionado);
         
         while (input(PIN_B1)  == 0);
      }
      /* Ao pressionar o bot?o RB2 chamamos a fun??o incrementar passando como
         par?metro o semaforoSelecionado. Assim a fun??o se encarrega de 
         realizar o incremento do valor no devido endere?o da EEPROM.*/
      if (input(PIN_B2)  == 0 ) {         
         incrementar (semaforoSelecionado);
         while (input(PIN_B2)  == 0);
      }
   }
}
