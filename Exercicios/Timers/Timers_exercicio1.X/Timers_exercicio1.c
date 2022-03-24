/*
 * File:   Timers_exercicio1.c
 * Author: Talles Belchior
 *
 * Created on 10 de Fevereiro de 2022, 20:15
 * 
 * 1.Configure o Timer 1 (Exemplo 2) para que ele gere uma interrupção e inverta o estado de um
 * LED a cada 2 segundos;
 * 
 *  
 */

#define F_CPU 16000000L    //Define a frequencia do microcontrolador - 16MHz
#include <avr/io.h>        //Definições do componente especifico
#include <util/delay.h>    //biblioteca para utilização do delay
#include <avr/pgmspace.h>  //para uso do PROGMEM, gravação de dados na memoria flash
#include <avr/interrupt.h> //define algumas macros para as interrupções
#include <stdio.h> //biblioteca para o uso da função sprintf

//Definições de macros para o trabalho com bits
#define set_bit(y, bit) (y|=(1<<bit))  //coloca em 1 o bit x da variavel Y
#define clr_bit(y, bit) (y&=~(1<<bit)) //coloca em 0 o bit x da variável Y
#define cpl_bit(y, bit) (y^=(1<<bit))  //troca o estado lógico do bit x da variável Y
#define tst_bit(y, bit) (y&(1<<bit))   //retorna 0 ou 1 conforme leitura do bit

#define LED PD5

/*
 * Tempo = ( TimerBits * Prescaler ) / Fosc
 * 
 * Valor inicial de contagem = 65536 - (TempoDesejado * Fosc)/ Prescaler = 
 * 
 * Valor inicial de contagem = 65536 - ( 2 * 16000000)/1024 = 34286
 *
 */

ISR(TIMER1_OVF_vect)                //interrupção do TC1
{
    TCNT1 = 34286;                  //Recarrega o registrador para gerar 2s novamente
    cpl_bit(PORTD, LED);            //Inverte o estado do LED
}

int main(void) {
    /* Replace with your application code */
    DDRD = 0b00100000;              //somente pino do LED como saída
    PORTD = 0b00000000;             //apaga LED e habilita pull-ups nos pinos não utilizados
    
    DDRC = 0b00000000;              //PORTC inteiro como entrada
    PORTC = 0b11111111;             //Habilita resistores de pull up internos
    
    TCCR1B = 0b00000101;            //TC1 com prescaler de 1024, a 16 MHz gera uma interrupção a cada 2s
    TCNT1 = 34286;                  //Inicia a contagem em 49911 para, no final, gerar 2s
    TIMSK1 = 0b00000001;            //habilita a interrupção do TC1
    sei();                          //habilita a chave de interrupção global                     
  
    while (1) {
        
       
        
    }
}

