/*
 * File:   LCD_exercicio3.c
 * Author: Talles Belchior
 *
 * Created on 10 de Fevereiro de 2022, 20:15
 * 
 * 3. Modifique o programa anterior para que o incremento e decremento da vari�vel seja realizada
 * dentro das interrup��es externas.
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

//defini��o para acessar a mem�ria flash
//PROGMEM const char flash_msg[] = "FLASH - IFSP";    //menssagem armazenada na mem�ria flash
//char ram_msg[] = "RAM - OLA MUNDO";                 //menssagem armazenada na mem�ria RAM
char msg1[] = "CONTAGEM = ";
char buffer[4] = "      ";               //Buffer para armazenamento da vari�vel convertida
volatile unsigned char num = 0;

int main(void) {
    /* Replace with your application code */
    DDRD = 0b11111111;              //PORTD inteiro como sa�da
    DDRB = 0b00101000;              //PB3 e PB5 como sa�da
    
    DDRC = 0b00000000;              //PORTC inteiro como entrada
    PORTC = 0b11111111;             //Habilita resistores de pull up internos
    PCICR  = 0b00000010;            //Hab. interrup��o por mudan�a de sinal no PORTC
    PCMSK1 = 0b00001100;            //Hab. os pinos PCINT10 e 11 para gerar interrup��o
    sei();                          // Habilita a chave geral das interrup��es
    
    inic_LCD_4bits();               //inicializa o LCD
//   escreve_LCD(ram_msg);           //string armazenada na RAM
//    cmd_LCD(0xc0, 0);               //desloca cursor para a segunda linha
//    escreve_LCD_Flash(flash_msg);   //string armazenada na flash
    //defini�ao para acessar a mem�ria flash
    
    
    while (1) {
        
        cmd_LCD(0x80, 0);               //desloca cursor para a primeira linha
        escreve_LCD(msg1);              //string armazenada na RAM
        cmd_LCD(0xc0, 0);               //desloca cursor para a segunda linha
        sprintf(buffer, "%.3d",num);    //Converte a vari�vel em uma string
        escreve_LCD(buffer);            //string armazenada na RAM
        
        if(num >= 100) num = 0;
        
    }
}

ISR(PCINT1_vect){
    if(!tst_bit(PINC,PC2)){
        num++;            
    }else if(!tst_bit(PINC,PC3)){
        num--;
    }
    
}