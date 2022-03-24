/*
 * File:   Interrupcoes_exercicio2.c
 * Author: Talles Belchior
 *
 * Created on 9 de Mar�o de 2022, 19:13
 * 
 * 2. Utilizando uma das duas chaves da placa, fa�a um programa que, pela interrup��o, mude
 * o display em que aparece o n�mero da contagem do exemplo 2;
 * 
 * 
 */


#define F_CPU 16000000L    //Define a frequencia do microcontrolador - 16MHz
#include <avr/io.h>        //Defini��es do componente especifico
#include <util/delay.h>    //biblioteca para utiliza��o do delay
#include <avr/interrupt.h> //define algumas macros para as interrup��es


ISR(PCINT0_vect);

volatile int contador = 0;
volatile int flag = 0;

int main(void) {
    char flag_2 = 0;        //Quando o bot�o de incremento foi apertado
    
    DDRB  = 0b00000011;     //PB1 como saida do (transistros do display)& PC2 como entrada
    PORTB = 0b00000001;     //Liga o display de 7 segmentos
    DDRC  = 0x00;           //PORTC como entrada
    PORTC = 0x0C;           //habilita pull-ups apenas nos bot�es
    DDRD  = 0xFF;           //PORTD como saida
    PORTD = 0x00;           //Apaga todos os leds
    PCICR = 0X01;           //Hab. interrup��o por mudan�a de sinal no PORTB
    PCMSK0 = 0b00010100;    //Hab. os pinos PCINT2 e 4 para gerar interrup��o
    sei();                  // Habilita a chave geral das interrup��es
    
    unsigned char vetor_disp[16] = {0xE7, 0x84, 0xD3, 0xD6, 0xB4, 0x76, 0x77, 0xC4, 
                                    0xF7, 0xF6, 0xF5, 0x37, 0x63, 0x97, 0x73, 0x71};
    
    /* Replace with your application code */
    while (1) {
         if(!(PINC & (1<<2))){
            _delay_ms(10);   //debouncing
            flag_2 = 1;
        }
        
        if((PINC & (1<<2)) && flag_2 == 1){
            
            _delay_ms(10);
            contador++;
            if(contador == 16){
                contador = 0;
            }
            PORTD = vetor_disp[contador];
            
            flag_2 = 0;
        }
        
    }
    
}

//Chave PB2 altera o display 
ISR(PCINT0_vect){
    if((PINB & (1<<2)) && flag == 0){
        PORTB = 0b00000010;
        flag = 1;
    }else if((PINB & (1<<2)) && flag == 1){
        PORTB = 0b00000001;
        flag = 0;
    }
    
}