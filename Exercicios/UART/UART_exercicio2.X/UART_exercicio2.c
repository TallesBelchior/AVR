/*
 * File:   UART_exercicio2.c
 * Author: Talles Belchior
 *
 * Created on 21 de Março de 2022, 16:45
 * 
 * 2.Desenvolva um programa que, a cada 1 segundo, envie pela serial o valor dos canais 0 e 1 do
 * ADC;
 * 
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "UART.h"
#include "ADC.h"

#define set_bit(y,bit) (y|=(1<<bit))        //coloca em 1 o bit x da variável Y
#define clr_bit(y,bit) (y&=~(1<<bit))       //coloca em 0 o bit x da variável Y
#define cpl_bit(y,bit) (y^=(1<<bit))        //troca o estado lógico do bit x da variável Y
#define tst_bit(y,bit) (y&(1<<bit))         //retorna 0 ou 1 conforme leitura do bit

char s[] = "Comunicacao SERIAL\n";
char adc_0[] = "ADC 0:\n";
char adc_1[] = "ADC 1:\n";
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
//------------------------------------------------------

ISR(TIMER1_OVF_vect) //interrupção do TC1
{
    TCNT1 = 49911; //Recarrega o registrador para gerar 1s novamente
    flag_ADC = 1;
    
} 


int main() {
    
    int valor_adc = 0;
    DDRB = 0b00000000;
    PORTB = 0b00000000;
    DDRC = 0b00000000;
    PORTC = 0b00001100;
    DDRD = 0b11111111;
    PORTD = 0b00000000;
    
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
                  
            UART_enviaString(adc_1);                        //Envia um texto para o computador
            valor_adc = le_ADC(1);                          //Lê o canal 0 do ADC
            sprintf(buffer, "%4d\n", valor_adc);            //Converte para string
            UART_enviaString(buffer);                       //Envia para o computador
            
            }
            //Apaga o flag de comunicação
            flag_ADC = 0;
        
    }
}
