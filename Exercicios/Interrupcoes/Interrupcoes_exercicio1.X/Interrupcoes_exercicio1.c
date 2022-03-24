/*
 * File:   Interrupcoes_exercicio1.c
 * Author: Talles Belchior
 *
 * Created on 9 de Março de 2022, 12:38
 * 
 * 1. Configure a interrupção das duas chaves ligadas na placa (PB2 e PB4). Cada uma delas
 * deve inverter o estado de dois LEDs;
 */


#define F_CPU 16000000L    //Define a frequencia do microcontrolador - 16MHz
#include <avr/io.h>        //Definições do componente especifico
#include <util/delay.h>    //biblioteca para utilização do delay
#include <avr/interrupt.h> //define algumas macros para as interrupções

ISR(PCINT0_vect);

int main(void) {
        
    DDRB = 0x00;            //PORTB como entrada    
    DDRD = 0Xff;            //PORTD como saida
    PORTD = 0x00;           //Apaga todos os leds
    PCICR = 0X01;           //Hab. interrupção por mudança de sinal no PORTB
    PCMSK0 = 0b00010100;    //Hab. os pinos PCINT2 e 4 para gerar interrupção
    sei();                  // Habilita a chave geral das interrupções
    
    /* Replace with your application code */
    while (1) {
        
    }
}

ISR(PCINT0_vect){
    if(PINB & (1<<2)){
        PORTD |= (1<<2);
    }else{
        PORTD &= ~(1<<2);
    }
    
    if(PINB & (1<<4)){
        PORTD |= (1<<4);
    }else{
        PORTD &= ~(1<<4);
    }
}