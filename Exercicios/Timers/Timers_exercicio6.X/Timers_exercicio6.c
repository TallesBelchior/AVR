/*
 * File:   Timers_exercicio6.c
 * Author: Talles Belchior
 *
 * Created on 18 de Março de 2022, 16:28
 * 
 * 6.Por fim, adicione um alarme fixo neste relógio. Quando as horas e os minutos atingirem um
 * valor pré-determinado, o LED 3 deve ser ligado. Ele será desligado após passar 1 minuto do
 * horário definido. Os testes condicionais para acionamento do alarme deverão ser feitos no
 * loop infinito do programa principal.
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

//#define LED PD5
#define PWM1 PC5            //escolha do pino para o sinal PWM1
#define Resol_PWM1 100      //PWM1 com 100 passos temporais de resolução
#define fade 5000           //5s 

char msg1[] = "Time:      ";
char buffer[] = "        ";          //Buffer para armazenamento da variável convertida
unsigned char seg = 0;
unsigned char min = 0;
unsigned char hr  = 0;

volatile unsigned int Passo_PWM1 = 0;
volatile unsigned int Ciclo_Ativo_PWM1 = 0;
volatile unsigned int fade_PWM1 = 0;

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

ISR(TIMER1_OVF_vect) //interrupção do TC1
{
    TCNT1 = 49911; //Recarrega o registrador para gerar 1s novamente
    seg++;
    
}   


ISR(TIMER2_OVF_vect) {
    
    /*
    * 
    * Valor inicial de contagem = TC2(8 bits) - (TempoDesejado * Fosc)/ Prescaler = 
    * 
    * Valor inicial de contagem = 256 - ( 0,00001 * 16000000)/1 = 96
    *
    */
    
    
    TCNT2 = 96;                             //Recarrega o timer para garantir a próxima interrupção com 10us
    Passo_PWM1++;                           //incremento do passo de tempo
    if (Passo_PWM1 == Resol_PWM1) {         //a cada 1 ms
        Passo_PWM1 = 0;                     //inicializa o contador
        set_bit(PORTC, PWM1);               //na igualdade de comparação coloca o pino PWM1 em 1
        
        fade_PWM1++;
        if(fade_PWM1 == 50){                //A cada 0,05 seg aumenta 1%
            Ciclo_Ativo_PWM1++;
            fade_PWM1 = 0;
            if(Ciclo_Ativo_PWM1 >= Resol_PWM1) Ciclo_Ativo_PWM1 = 0;
        }
    } else if (Passo_PWM1 == Ciclo_Ativo_PWM1){  
        clr_bit(PORTC, PWM1);               /*quando o contador atinge o valor do ciclo ativo do PWM1, o pino vai a zero*/
        
    }   
}




int main(void) {
    /* Replace with your application code */
    DDRD = 0b11111111;              //somente pino do LED como saída
    
    
    DDRC  = 0b11100000;             //PORTC como entrada /PC5 como saida
    PORTC = 0b00011111;             //Habilita resistores de pull up internos
    PCICR  = 0b00000010;            //Hab. interrupção por mudança de sinal no PORTC
    PCMSK1 = 0b00001100;            //Hab. os pinos PCINT10 e 11 para gerar interrupção
    sei();                          // Habilita a chave geral das interrupções
    
    TCCR1B = 0b00000101;            //TC1 com prescaler de 1024, a 16 MHz gera uma interrupção a cada 1s
    TCNT1 = 49911;                  //Inicia a contagem em 49911 para, no final, gerar 1s
    TIMSK1 = 0b00000001;            //habilita a interrupção do TC1
    
    TCCR2B = 0b00000001;            //TC2 com prescaler de 1
    TCNT2 = 96;                     //Inicia a contagem em 96 para, no final, gerar 10 us
    TIMSK2 = 0b00000001;            //habilita a interrupção do TC1
    
    sei();                          //habilita a chave de interrupção global                     
  
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
        sprintf(buffer, "%.2d:%.2d:%.2d",hr,min,seg);    //Converte a variável em uma string
        escreve_LCD(buffer);            //string armazenada na RAM
      
    }
}

