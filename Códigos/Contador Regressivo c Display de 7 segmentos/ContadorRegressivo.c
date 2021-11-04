/*Contador com Display de 7 segmentos, onde se poder incrementar ou decrementar a partir de
bot�es, sendo esses no c�digo o B0 para decrementar e B1 para incrementar.*/
#include <18F4550.h>
#fuses XT, NOMCLR, NOWDT
#use delay(clock = 4MHz)

int8 segmentos[] = {//Array definidor de segmentos
//Valores codificados.
   0b00111111, 0b00000110, 0b01011011, 0b01001111,
   0b01100110, 0b01101101, 0b01111100, 0b00000111,
   0b01111111, 0b01100111
};

int16 numero = 0;
int modo = 0; // 0: configura��o e 1:contagem regressivo
void ativarDisplay(int pino, int numero){
   output_bit(PIN_A2,0);
   output_bit(PIN_A3,0);
   output_bit(PIN_A4,0);
   output_bit(PIN_A5,0);
   
   output_bit(pino,1);//bit que vier como par�merto pino bai pra n�vel l�gico 1.
   
   output_d(segmentos[numero]);
   delay_ms(5);//Poss�vel de visualizar todos os displays ativados ao mesmo tempo
   //delay_ms(1000);//Altern�ncia entre os displays
}

void mostrarNumero(int16 valor){//Fun��o respons�vel por mostrar o n�mero no display, indo de 0 � 9999
   int unidade = ((valor%1000) %100)%10, dezena = ((valor%1000) %100) / 10, centena = (valor%1000)/100, milhar = valor/1000;
   
   ativarDisplay(PIN_A2,milhar);
   ativarDisplay(PIN_A3,centena);
   ativarDisplay(PIN_A4,dezena);
   ativarDisplay(PIN_A5,unidade);
}
void main(){
   int contador = 0;
   
   while(true){
      if (modo == 0){
         if(input(PIN_B0) == 0){
            if(numero > 0){
               numero--;
            }
            while(input(PIN_B0) == 0);//Impede que ocorra quando o bot�o est� pressionado
          }
          if(input(PIN_B1)==0){
            if(numero < 9999){
               numero++;
            }
            while(input(PIN_B1) == 0);
          }
          if(input(PIN_B2) == 0){
            modo = 1;                     
            while(input(PIN_B2) == 0);
          }
      }
      if(modo == 1){
         if(numero > 0){
            contador++;
            if(contador == 40){/*Contador est� servindo para desacelerar o decremento, tendo em vista que cada vez que o n�mero � mostrado h� um delay de 5ms, 
            vezes 4 displays d� 20 ms, vezes 40, que resulta 800ms, quase 1s*/
               numero--;
               contador = 0;
            }
         }else{
            modo = 0;
         }
      }
       mostrarNumero(numero);
   }
}
