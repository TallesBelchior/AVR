/*
 * File:   AD_exercicio1.c
 * Author: Talles Belchior
 *
 * Created on 19 de Março de 2022, 12:16
 * 
 * 1. Implemente, no exemplo 3, a leitura do canal 1 do ADC e apresente o valor lido na segunda linha do
 * display LCD;
 * 
 */


#define F_CPU 16000000UL            //define a frequência do microcontrolador - 16MHz
#include <avr/io.h>                 //definições do componente especificado
#include <util/delay.h>             //biblioteca para o uso das rotinas de _delay_ms e _delay_us()
#include <stdio.h>                  //biblioteca para o uso da função sprintf
#include "LCD.h"                    //biblioteca com as funções do LCD
#include "ADC.h"                    //biblioteca com as funções do ADC

char msg1[] = "CANAL 0 = ";
char msg2[] = "CANAL 1 = ";
char buffer1[5] = " ";
char buffer2[5] = " ";

int main() {
    
    unsigned int valor = 0;
    DDRD = 0b11111111;                      //PORTD inteiro como saída
    DDRB = 0b00101000;                      //PB3 e PB5 como saída (pinos do LCD)
    inicializa_ADC();                       //inicializa o ADC
    inic_LCD_4bits();                       //inicializa o LCD
    
    escreve_LCD(msg1);                      //Escreve a mensagem na linha 1 do LCD
    cmd_LCD(0xC0, 0);                     //desloca cursor para a segunda linha
    escreve_LCD(msg2);                      //Escreve a mensagem na linha 2 do LCD
    
    while (1) {
        
        valor = le_ADC(0);                   //Faz a leitura do canal 0 do ADC
        _delay_ms(250);
        sprintf(buffer1, "%.4d", valor);     //Converte a variável valor em uma string
        cmd_LCD(0x8A, 0);                   //Retorna o cursor para o início da linha 1
        escreve_LCD(buffer1);                 //Escreve a variável no display
        
        
        valor = le_ADC(1);                   //Faz a leitura do canal 0 do ADC
        _delay_ms(250);
        sprintf(buffer2, "%.4d", valor);     //Converte a variável valor em uma string
        cmd_LCD(0xCA, 0);                     //desloca cursor para a segunda linha
        escreve_LCD(buffer2);                 //Escreve a variável no display
        
    }
}