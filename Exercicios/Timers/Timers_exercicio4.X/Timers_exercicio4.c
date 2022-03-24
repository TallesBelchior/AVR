/*
 * File:   Timers_exercicio4.c
 * Author: Talles Belchior
 *
 * Created on 11 de Fevereiro de 2022, 20:15
 * 
 *4. Implemente, no programa anterior, um ajuste de horas e minutos. O incremento das horas
 * (0 ? 23) deve ser feito pela interrup��o do bot�o PC2 e o incremento dos minutos pela
 * interrup��o do bot�o PC3 (0 ? 59).
 * 
 *  
 */

#define F_CPU 16000000L    //Define a frequencia do microcontrolador - 16MHz
#include <avr/io.h>        //Defini��es do componente especifico
#include <util/delay.h>    //biblioteca para utiliza��o do delay
#include <avr/pgmspace.h>  //para uso do PROGMEM, grava��o de dados na memoria flash
#include <avr/interrupt.h> //define algumas macros para as interrup��es
#include "LCD.h"           //inclus�o da biblioteca LCD.h
#include <stdio.h> //biblioteca para o uso da fun��o sprintf

//Defini��es de macros para o trabalho com bits
#define set_bit(y, bit) (y|=(1<<bit))  //coloca em 1 o bit x da variavel Y
#define clr_bit(y, bit) (y&=~(1<<bit)) //coloca em 0 o bit x da vari�vel Y
#define cpl_bit(y, bit) (y^=(1<<bit))  //troca o estado l�gico do bit x da vari�vel Y
#define tst_bit(y, bit) (y&(1<<bit))   //retorna 0 ou 1 conforme leitura do bit

#define LED PD5

char msg1[] = "Time:      ";
char buffer[] = "        ";          //Buffer para armazenamento da vari�vel convertida
unsigned char seg = 0;
unsigned char min = 0;
unsigned char hr  = 0;

ISR(TIMER1_OVF_vect) //interrup��o do TC1
{
    TCNT1 = 49911; //Recarrega o registrador para gerar 1s novamente
    seg++;
    
}   

ISR(PCINT1_vect){
    if(!tst_bit(PINC, PC2)){
        hr++;
        if(hr >= 24) hr = 0;
    }
    if(!tst_bit(PINC, PC3)){
        min++;
        if(min >= 60) min = 0;
    }
}

int main(void) {
    /* Replace with your application code */
    DDRD = 0b11111111;              //somente pino do LED como sa�da
    
    DDRC = 0b00000000;              //PORTC inteiro como entrada
    PORTC = 0b11111111;             //Habilita resistores de pull up internos
    PCICR  = 0b00000010;            //Hab. interrup��o por mudan�a de sinal no PORTC
    PCMSK1 = 0b00001100;            //Hab. os pinos PCINT10 e 11 para gerar interrup��o
    
    TCCR1B = 0b00000101;            //TC1 com prescaler de 1024, a 16 MHz gera uma interrup��o a cada 1s
    TCNT1 = 49911;                  //Inicia a contagem em 49911 para, no final, gerar 1s
    TIMSK1 = 0b00000001;            //habilita a interrup��o do TC1
    sei();                          //habilita a chave de interrup��o global                     
  
    inic_LCD_4bits();               //inicializa o LCD
    
    while (1) {
        
        if(seg >= 60 ){
         
            min++;
            seg = 0;
        }
        if(min >= 60){
            hr++;
            min = 0;
                    
        }
        if(hr >= 24){
            hr = 0;
            
        }
        
        
        cmd_LCD(0x80, 0);               //desloca cursor para a primeira linha
        escreve_LCD(msg1);              //string armazenada na RAM
        cmd_LCD(0xc0, 0);               //desloca cursor para a segunda linha
        sprintf(buffer, "%.2d:%.2d:%.2d",hr,min,seg);    //Converte a vari�vel em uma string
        escreve_LCD(buffer);            //string armazenada na RAM
        
    }
}

