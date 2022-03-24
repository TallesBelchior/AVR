/*
 * File:   UART_exercicio1.c
 * Author: Talles Belchior
 *
 * Created on 21 de Março de 2022, 16:19
 * 
 * 1.Inclua, no terceiro exemplo, mais comandos para: ligar e desligar os LEDs 4 à 7, ler o estado
 * do botão PC3 e das chaves PB2 e PB4 e solicitar o valor do canal 1 do ADC.
 * 
 * ?A? ? Acende um LED;
 * 'a' - Apagaga LED;
 * ?B? ? Apaga um LED;
 * 'b' - Apagaga LED;
 * ?C? ? Apaga um LED;
 * 'c' - Apagaga LED;
 * ?D? ? Retorna o status do botão PC2;
 * ?E? ? Retorna o status do botão PC3;
 * ?F? ? Retorna o valor do conversor AD.
 * ?G? ? Retorna o valor do conversor AD.
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
char bt_press[] = "BOTAO PRESSIONADO\n";
char bt_solto[] = "BOTAO SOLTO\n";
char buffer[5];
volatile char ch;                               //armazena o caractere lido
volatile char flag_com = 0;                     //flag que indica se houve recepção de dado


// Interrupção da UART
//======================================================

ISR(USART_RX_vect) {
    ch = UDR0; //Faz a leitura do buffer da serial
    UART_enviaCaractere(ch); //Envia o caractere lido para o computador
    flag_com = 1; //Aciona o flag de comunicação
}
//------------------------------------------------------

int main() {
    
    int valor_adc = 0;
    DDRB = 0b00000000;
    PORTB = 0b00000000;
    DDRC = 0b00000000;
    PORTC = 0b00001100;
    DDRD = 0b11111111;
    PORTD = 0b00000000;
    UART_config();                      //Inicializa a comunicação UART
    inicializa_ADC();                   //Configura o ADC
    UART_enviaString(s);                //Envia um texto para o computador
    sei();                              //Habilita as interrupções
    
    while (1) {
        //Se houve comunicação, testa o caractere recebido
        if (flag_com == 1) {
            switch (ch) {
                //LEDS
                case 'A':

                    set_bit(PORTD, PD4);                //Liga o LED4

                    break;

                case 'a':

                    clr_bit(PORTD, PD4);                //Desliga o LED4

                    break;
                
                case 'B':

                    set_bit(PORTD, PD5);                //Liga o LED5

                    break;

                case 'b':

                    clr_bit(PORTD, PD5);                //Desliga o LED5

                    break;
                    
                case 'C':

                    set_bit(PORTD, PD6);                //Liga o LED6

                    break;

                case 'c':

                    clr_bit(PORTD, PD6);                //Desliga o LED6

                    break;
                 
                //============================================================    
                //Chaves
                case 'D':

                    if (tst_bit(PINC, PINC2) == 0)      //Testa o botão PC2
                        UART_enviaString(bt_press);     //Envia "PRESSIONADO"
                    else
                        UART_enviaString(bt_solto);     //Envia "SOLTO"
                    break;
                
                case 'E':

                    if (tst_bit(PINC, PINC3) == 0)      //Testa o botão PC3
                        UART_enviaString(bt_press);     //Envia "PRESSIONADO"
                    else
                        UART_enviaString(bt_solto);     //Envia "SOLTO"
                    break;
                
                
                //=============================================================
                //ADC
                case 'F':

                    valor_adc = le_ADC(0);              //Lê o canal 0 do ADC
                    sprintf(buffer, "%4d\n", valor_adc); //Converte para string
                    UART_enviaString(buffer);           //Envia para o computador
                    break;
                    
                case 'G':

                    valor_adc = le_ADC(1);              //Lê o canal 0 do ADC
                    sprintf(buffer, "%4d\n", valor_adc); //Converte para string
                    UART_enviaString(buffer);           //Envia para o computador
                    break;
            }
            //Apaga o flag de comunicação
            flag_com = 0;
        }
    }
}