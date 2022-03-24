;Author: Talles Belchior
    
;Created on 03 de Março de 2022

;exercicio 2: Elaborar um programa que troque o estado do LED a cada "PULSO" 
;dado no botão ligado em PC3 (Acionamento Flip-Flop).

.ORG 0x0000
    
INICIO:
    ;Configura PORTD como saida
    LDI R16, 0XFF
    OUT DDRD, R16
    
    ;Configura PORTC como entrada
    LDI R16, 0b00000000
    OUT DDRC, R16
    
    ;Ativa o resistor de Pull-Up interno
    LDI R16, 0b11111111
    OUT PORTC, R16

LOOP:
    SBIS PINC, PC3
    RJMP ALTERA
    RJMP LOOP

ALTERA:
    SBIS PIND, PD0
    RJMP LIGA_LED
    RJMP DESLIGA_LED
    
    

DESLIGA_LED:
    ;PD0 = "0", APAGANDO O LED, CBI = CLEAR BIT
    CBI PORTD, PD0
    RJMP LOOP
        
    
LIGA_LED:
    ;PD0 = "1", lIGANDO O LED, SBI = SET BIT
    SBI PORTD, PD0
    RJMP LOOP



