;Author: Talles Belchior
    
;Created on 03 de Março de 2022

;exercicio 3: Elaborar um programa que execute 3 funções lógicas:
;A. LED0 = SW1 & SW2;
;B. LED1 = SW2 | SW3;
;C. LED2 = SW1 ^ SW3;


.ORG 0x0000
    
INICIO:
    ;Configura PORTB como entrada
    LDI R16, 0x00
    OUT DDRB, R16
        
    LDI R16, 0xFF
    OUT DDRD, R16
    
LOOP:
    SBIC PINB, PB4
    RJMP SW1
    SBIC PINB, PB2
    RJMP SW2
    RJMP LED2
    
SW1:
   SBIC PINB, PB2
   RJMP LED0
   RJMP LED1
   
SW2:
   SBIC PINB, PB4
   RJMP LED0
   RJMP LED1
   
;A. LED0 = SW1 & SW2;   
LED0:
    SBI PORTD, PD0
    CBI PORTD, PD1
    CBI PORTD, PD2
    RJMP LOOP

;B. LED1 = SW2 | SW3;
LED1:
    CBI PORTD, PD0
    SBI PORTD, PD1
    CBI PORTD, PD2
    RJMP LOOP
    
;C. LED2 = SW1 ^ SW3;    
LED2:
    CBI PORTD, PD0
    CBI PORTD, PD1
    SBI PORTD, PD2
    RJMP LOOP

