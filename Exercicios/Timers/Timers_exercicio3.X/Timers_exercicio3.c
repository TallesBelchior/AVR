/*
 * File:   Timers_exercicio3.c
 * Author: Talles Belchior
 *
 * Created on 11 de Fevereiro de 2022, 20:15
 * 
 * 3.Apresente no display LCD um relógio simples no formato HH:MM:SS. O incremento dos
 * segundos deve ser feito a partir da interrupção do Timer 1;
 * Dica: Pode ser utilizada a função sprintf(buffer, ?%d:%d:%d?, hora, minuto, segundo).
 * 
 *  
 */

#define F_CPU 16000000L    //Define a frequencia do microcontrolador - 16MHz
#include <avr/io.h>        //Definições do componente especifico
#include <util/delay.h>    //biblioteca para utilização do delay
#include <avr/pgmspace.h>  //para uso do PROGMEM, gravação de dados na memoria flash
#include <avr/interrupt.h> //define algumas macros para as interrupções
#include "LCD.h"           //inclusão da biblioteca LCD.h
#include <stdio.h> //biblioteca para o uso da função sprintf

//Definições de macros para o trabalho com bits
#define set_bit(y, bit) (y|=(1<<bit))  //coloca em 1 o bit x da variavel Y
#define clr_bit(y, bit) (y&=~(1<<bit)) //coloca em 0 o bit x da variável Y
#define cpl_bit(y, bit) (y^=(1<<bit))  //troca o estado lógico do bit x da variável Y
#define tst_bit(y, bit) (y&(1<<bit))   //retorna 0 ou 1 conforme leitura do bit

#define LED PD5

char msg1[] = "Time:";
char buffer[7] = "       ";               //Buffer para armazenamento da variável convertida
unsigned int seg = 0;
unsigned int min = 0;
unsigned int hr  = 0;

/*
 * Tempo = ( TimerBits * Prescaler ) / Fosc
 * 
 * Valor inicial de contagem = 65536 - (TempoDesejado * Fosc)/ Prescaler = 
 * 
 * Valor inicial de contagem = 65536 - ( 1 * 16000000)/1024 = 49911
 *
 */

ISR(TIMER1_OVF_vect) //interrupção do TC1
{
    TCNT1 = 49911; //Recarrega o registrador para gerar 1s novamente
    seg++;
}

int main(void) {
    /* Replace with your application code */
    DDRD = 0b11111111;              //somente pino do LED como saída
    
    TCCR1B = 0b00000101;            //TC1 com prescaler de 1024, a 16 MHz gera uma interrupção a cada 1s
    TCNT1 = 49911;                  //Inicia a contagem em 49911 para, no final, gerar 1s
    TIMSK1 = 0b00000001;            //habilita a interrupção do TC1
    sei();                          //habilita a chave de interrupção global                     
  
    inic_LCD_4bits();               //inicializa o LCD
    
    while (1) {
        
        if(seg == 60){
            min++;
            seg = 0;
        }
        if(min == 60){
            hr++;
            min = 0;
        }
        if(hr == 23){
            hr = 0;
        }
        
        
        cmd_LCD(0x80, 0);               //desloca cursor para a primeira linha
        escreve_LCD(msg1);              //string armazenada na RAM
        cmd_LCD(0xc0, 0);               //desloca cursor para a segunda linha
        sprintf(buffer, "%d:%d:%d",hr,min,seg);    //Converte a variável em uma string
        escreve_LCD(buffer);            //string armazenada na RAM
        
    }
}

