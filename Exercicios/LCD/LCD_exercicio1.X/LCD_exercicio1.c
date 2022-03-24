/*
 * File:   LCD_exercicio1.c
 * Author: Talles Belchior
 *
 * Created on 10 de Fevereiro de 2022, 20:15
 * 
 * 1. Apresente no display o status dos botões PC2 e PC3 (Pressionado ou Solto). O status do botão
 * PC2 deve ser apresentado na primeira linha do display, enquanto do botão PC3, na segunda
 * linha.
 * 
 *  
 */

#define F_CPU 16000000L    //Define a frequencia do microcontrolador - 16MHz
#include <avr/io.h>        //Definições do componente especifico
#include <util/delay.h>    //biblioteca para utilização do delay
#include <avr/pgmspace.h>  //para uso do PROGMEM, gravação de dados na memoria flash
#include "LCD.h"           //inclusão da biblioteca LCD.h

//Definições de macros para o trabalho com bits
#define set_bit(y, bit) (y|=(1<<bit))  //coloca em 1 o bit x da variavel Y
#define clr_bit(y, bit) (y&=~(1<<bit)) //coloca em 0 o bit x da variável Y
#define cpl_bit(y, bit) (y^=(1<<bit))  //troca o estado lógico do bit x da variável Y
#define tst_bit(y, bit) (y&(1<<bit))   //retorna 0 ou 1 conforme leitura do bit

//definição para acessar a memória flash
//PROGMEM const char flash_msg[] = "FLASH - IFSP";    //menssagem armazenada na memória flash
//char ram_msg[] = "RAM - OLA MUNDO";                 //menssagem armazenada na memória RAM

char solto[]       = "Solto      ";
char pressionado[] = "Pressionado";

int main(void) {
    /* Replace with your application code */
    DDRD = 0b11111111;              //PORTD inteiro como saída
    DDRB = 0b00101000;              //PB3 e PB5 como saída
    
    DDRC = 0b00000000;              //PORTC inteiro como entrada
    PORTC = 0b11111111;             //Habilita resistores de pull up internos
    
    
    inic_LCD_4bits();               //inicializa o LCD
//   escreve_LCD(ram_msg);           //string armazenada na RAM
//    cmd_LCD(0xc0, 0);               //desloca cursor para a segunda linha
//    escreve_LCD_Flash(flash_msg);   //string armazenada na flash
    
    while (1) {        
         
      
        if(tst_bit(PINC,PINC2) == 0){
            cmd_LCD(0x80, 0);               //desloca cursor para a primeira linha
            escreve_LCD(pressionado);
        }else{
            cmd_LCD(0x80, 0);               //desloca cursor para a primeira linha
            escreve_LCD(solto);
        }
        
        if(!tst_bit(PINC,PC3)){
            cmd_LCD(0xC0, 0);               //desloca cursor para a segunda linha
            escreve_LCD(pressionado);
        }else{
            cmd_LCD(0xC0, 0);               //desloca cursor para a segunda linha
            escreve_LCD(solto);
        }
        
    }
}
