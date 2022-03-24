/*
 * File:   UART_exrecicio4.c
 * Author: Talles Belchior
 *
 * Created on 21 de Março de 2022, 20:04
 * 
 * 4. Crie um programa onde o usuário altere o ciclo do PWM pela serial. O ciclo deverá ser divido
 * em 10 faixas, por exemplo: ?0? ? 10%, ?1? ? 20%, ?2? ? 30%, ?9? ? 100%, ?A? ? 0%. Utilize o LED
 * de PD4 para mostrar o funcionamento do PWM.
 * 
 * '0' -  0%
 * '1' - 10%
 * '2' - 20%
 * '3' - 30%
 * '4' - 40%
 * '5' - 50%
 * '6' - 60%
 * '7' - 70%
 * '8' - 80%
 * '9' - 90%
 * 'A' - 100%
 * 
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "UART.h"

#define set_bit(y,bit) (y|=(1<<bit))        //coloca em 1 o bit x da variável Y
#define clr_bit(y,bit) (y&=~(1<<bit))       //coloca em 0 o bit x da variável Y
#define cpl_bit(y,bit) (y^=(1<<bit))        //troca o estado lógico do bit x da variável Y
#define tst_bit(y,bit) (y&(1<<bit))         //retorna 0 ou 1 conforme leitura do bit

#define PWM1 PD4            //escolha do pino para o sinal PWM1
#define Resol_PWM1 200      //PWM1 com 200 passos temporais de resolução


char s[] = "Comunicacao SERIAL\n";
char buffer[5];
volatile char ch;                               //armazena o caractere lido
volatile char flag_com = 0;                     //flag que indica se houve recepção de dado
volatile unsigned int Passo_PWM1 = 0;
volatile unsigned int Ciclo_Ativo_PWM1;

/*
 * PeriodoEstouro = PeriodoPWM / Resolução 
 * 
 * PeriodoEstouro = 0,002 / 200 = 10us
 * 
 */

//Interrupção do Timer que controla o PWM

ISR(TIMER0_OVF_vect) {
    TCNT0 = 96;                 //Recarrega o timer para garantir a próxima interrupção com 10us
    Passo_PWM1++;               //incremento do passo de tempo
    if (Passo_PWM1 == Resol_PWM1) {
        Passo_PWM1 = 0;         //inicializa o contador
        set_bit(PORTD, PWM1);   //na igualdade de comparação coloca o pino PWM1 em 1
    } else if (Passo_PWM1 == Ciclo_Ativo_PWM1)
        clr_bit(PORTD, PWM1);   /*quando o contador atinge o valor do ciclo ativo do
                                  PWM1, o pino vai a zero*/
}

// Interrupção da UART
//======================================================

ISR(USART_RX_vect) {
    ch = UDR0; //Faz a leitura do buffer da serial
    UART_enviaCaractere(ch); //Envia o caractere lido para o computador
    flag_com = 1; //Aciona o flag de comunicação
}



int main() {
    DDRD = 0b11111111;          //PORTD como saída
    PORTD = 0b00000000;         //acende apenas o LED0
    
    TCCR0B = 0b00000001;        //TC0 com prescaler de 1
    TIMSK0 = 0b00000001;        //habilita a interrupção do TC0
    TCNT0 = 96;                 //Inicia a contagem em 96 para gerar a interrupção a cada 10us
    
    UART_config();                      //Inicializa a comunicação UART
    UART_enviaString(s);                //Envia um texto para o computador
    
    sei();                      //habilita as interrupções
  //  Ciclo_Ativo_PWM1 = 50;       //Determina o ciclo ativo para o PWM1 (0 - 100)
    while (1) {
        
        if (flag_com == 1) {
            switch (ch) {
                //LEDS
                case '0':
                    Ciclo_Ativo_PWM1 = 1;       //Determina o ciclo ativo para o PWM1 (0 - 100)
                break;

                case '1':
                    Ciclo_Ativo_PWM1 = 10;       //Determina o ciclo ativo para o PWM1 (0 - 100)
                break;
                
                case '2':
                    Ciclo_Ativo_PWM1 = 20;       //Determina o ciclo ativo para o PWM1 (0 - 100)
                break;
                
                case '3':
                    Ciclo_Ativo_PWM1 = 30;       //Determina o ciclo ativo para o PWM1 (0 - 100)
                break;
                
                case '4':
                    Ciclo_Ativo_PWM1 = 40;       //Determina o ciclo ativo para o PWM1 (0 - 100)
                break;
                
                case '5':
                    Ciclo_Ativo_PWM1 = 50;       //Determina o ciclo ativo para o PWM1 (0 - 100)
                break;
                
                case '6':
                    Ciclo_Ativo_PWM1 = 60;       //Determina o ciclo ativo para o PWM1 (0 - 100)
                break;
                
                case '7':
                    Ciclo_Ativo_PWM1 = 70;       //Determina o ciclo ativo para o PWM1 (0 - 100)
                break;
                
                case '8':
                    Ciclo_Ativo_PWM1 = 80;       //Determina o ciclo ativo para o PWM1 (0 - 100)
                break;
                
                case '9':
                    Ciclo_Ativo_PWM1 = 90;       //Determina o ciclo ativo para o PWM1 (0 - 100)
                break;
                
                case 'A':
                    Ciclo_Ativo_PWM1 = 00;       //Determina o ciclo ativo para o PWM1 (0 - 100)
                break;
            }
            //Apaga o flag de comunicação
            flag_com = 0;
            
            
        }
    }
}
