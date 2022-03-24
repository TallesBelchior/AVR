/*
 * File:   Interrupcoes_exercicio3.c
 * Author: Talles Belchior
 *
 * Created on 10 de Mar�o de 2022, 19:13
 * 
 * 3. Crie um contador de 0 � F autom�tico, que incremente a cada 1 segundo e, utilizando as
 * interrup��es externas, crie um bot�o de START/PAUSE (PC2) e outro de RESET (PC3).
 * Obs: O bot�o de RESET s� pode funcionar caso a contagem esteja parada.
 * 
 * 
 */


#define F_CPU 16000000L    //Define a frequencia do microcontrolador - 16MHz
#include <avr/io.h>        //Defini��es do componente especifico
#include <util/delay.h>    //biblioteca para utiliza��o do delay
#include <avr/interrupt.h> //define algumas macros para as interrup��es


ISR(PCINT1_vect);

volatile int contador = 0;
volatile char pause = 1;
volatile char reset = 0;
volatile int tempo = 0;

int main(void) {
    
    
    DDRB  = 0b00000011;     //PB1 como saida do (transistros do display)& PC2 como entrada
    PORTB = 0x01;           //Liga o display de 7 segmentos
    DDRC  = 0x00;           //PORTC como entrada
    PORTC = 0x0C;           //habilita pull-ups apenas nos bot�es
    DDRD  = 0xFF;           //PORTD como saida
    PORTD = 0x00;           //Apaga todos os leds
    PCICR  = 0b00000010;    //Hab. interrup��o por mudan�a de sinal no PORTC
    PCMSK1 = 0b00001100;    //Hab. os pinos PCINT10 e 11 para gerar interrup��o
    sei();                  // Habilita a chave geral das interrup��es
    
    unsigned char vetor_disp[16] = {0xE7, 0x84, 0xD3, 0xD6, 0xB4, 0x76, 0x77, 0xC4, 
                                    0xF7, 0xF6, 0xF5, 0x37, 0x63, 0x97, 0x73, 0x71};
    
    /* Replace with your application code */
    while (1) {
        
        
        
        while(pause == 1){    
            PORTD = vetor_disp[contador];
            while(tempo != 10 && pause == 1){
               _delay_ms(100);
                tempo++;
            }
            
            if(pause == 0)break;
            tempo = 0;
            contador++;
        
            if(contador == 0x0f){
                contador = 0;
            }
        }
        
        if(reset == 1 && pause != 1){
            contador = 0;
            reset = 0;
            
        }
        
    }   
}


ISR(PCINT1_vect){
    //START/PAUSE
    if(!(PINC & (1<<2))){
        pause ^= (1<<0);
    }//RESET
    else if(!(PINC & (1<<3))){
        reset ^= (1<<0);
    }
    
}