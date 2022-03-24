/*
 * File:   UART_exercicio3.c
 * Author: Talles Belchior
 *
 * Created on 21 de Março de 2022, 17:11
 * 
 * 3.Complemente o programa acima para que, ao pressionar qualquer um dos botões ou chaves,
 * seja enviada uma mensagem para o computador de qual pino foi acionado;
 * 
 */


#define F_CPU 16000000L    //Define a frequencia do microcontrolador - 16MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>    //biblioteca para utilização do delay
#include "UART.h"
#include "ADC.h"

#define set_bit(y,bit) (y|=(1<<bit))        //coloca em 1 o bit x da variável Y
#define clr_bit(y,bit) (y&=~(1<<bit))       //coloca em 0 o bit x da variável Y
#define cpl_bit(y,bit) (y^=(1<<bit))        //troca o estado lógico do bit x da variável Y
#define tst_bit(y,bit) (y&(1<<bit))         //retorna 0 ou 1 conforme leitura do bit

char s[] = "Comunicacao SERIAL\n";
char adc_0[] = "ADC 0:\n";
char adc_1[] = "ADC 1:\n";
char bot_2[] = "PC2 pressionado!\n";
char bot_3[] = "PC3 pressionado!\n";
char chave_2[] = "PB2 pressionado!\n";
char chave_4[] = "PB4 pressionado!\n";
char buffer[5];
volatile char ch;                               //armazena o caractere lido
volatile char flag_com = 0;                     //flag que indica se houve recepção de dado
volatile char flag_ADC = 0;


// Interrupção da UART
//======================================================

ISR(USART_RX_vect) {
    ch = UDR0; //Faz a leitura do buffer da serial
    UART_enviaCaractere(ch); //Envia o caractere lido para o computador
    flag_com = 1; //Aciona o flag de comunicação
}

//interrupção do TC1
//=====================================================
ISR(TIMER1_OVF_vect) 
{
    TCNT1 = 49911; //Recarrega o registrador para gerar 1s novamente
    flag_ADC = 1;
    
} 

//Interrupção extrena PORTC
//=====================================================
ISR(PCINT1_vect){
    if(tst_bit(PINC, PC2)){
       UART_enviaString(bot_2);                       //Envia para o computador 
    }
    if(tst_bit(PINC, PC3)){
       UART_enviaString(bot_3);                       //Envia para o computador 
    }
}

//Interrupção extrena PORTC
//=====================================================
ISR(PCINT0_vect){
    if(tst_bit(PINB, PB2)){
       UART_enviaString(chave_2);                       //Envia para o computador 
    }
    if(tst_bit(PINB, PB4)){
       UART_enviaString(chave_4);                       //Envia para o computador 
    }
}

int main() {
    
    int valor_adc = 0;
    DDRB = 0b00000000;
    PORTB = 0b00000000;
    
    DDRC = 0b00000000;
    PORTC = 0b00000000;
    
    DDRD = 0b11111111;
    PORTD = 0b00000000;
    
    PCICR  = 0b00000010;                //Hab. interrupção por mudança de sinal no PORTC
    PCMSK1 = 0b00001100;                //Hab. os pinos PCINT10 e 11 para gerar interrupção
    
    PCICR = 0x01;                       //Hab. interrupção por mudança de sinal no PORTB
    PCMSK0 = 0b00010100;                //Hab. os pinos PCINT2 e 4 para gerar interrupção
    
    TCCR1B = 0b00000101;                //TC1 com prescaler de 1024, a 16 MHz gera uma interrupção a cada 1s
    TCNT1 = 49911;                      //Inicia a contagem em 49911 para, no final, gerar 1s
    TIMSK1 = 0b00000001;                //habilita a interrupção do TC1
    
    UART_config();                      //Inicializa a comunicação UART
    inicializa_ADC();                   //Configura o ADC
    UART_enviaString(s);                //Envia um texto para o computador
    sei();                              //Habilita as interrupções
    
    while (1) {
        //Se houve comunicação, testa o caractere recebido
        if (flag_ADC == 1) {
            
            //ADC
            UART_enviaString(adc_0);                        //Envia um texto para o computador
            valor_adc = le_ADC(0);                          //Lê o canal 0 do ADC
            sprintf(buffer, "%4d\n", valor_adc);            //Converte para string
            UART_enviaString(buffer);                       //Envia para o computador
          //  _delay_ms(250);      
            UART_enviaString(adc_1);                        //Envia um texto para o computador
            valor_adc = le_ADC(1);                          //Lê o canal 0 do ADC
            sprintf(buffer, "%4d\n", valor_adc);            //Converte para string
            UART_enviaString(buffer);                       //Envia para o computador
        //    _delay_ms(250);   
            }
            //Apaga o flag de comunicação
            flag_ADC = 0;
        
    }
}

