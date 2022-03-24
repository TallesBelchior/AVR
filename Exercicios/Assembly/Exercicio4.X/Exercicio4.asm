;Author: Talles Belchior
    
;Created on 03 de Março de 2022

;exercicio 4: Elaborar um programa que realize a soma entre dois registradores (R16 e R17)
;e apresente o resultado nos leds da placa.Utilize a função aritmética (ADD). 


.ORG 0x0000
    
INICIO:
    ;Configura PORTD como saida
    LDI R16, 0XFF
    OUT DDRD, R16
    
LOOP:
    ;
    LDI R16, 0x0F
    LDI R17, 0xF0
    ADD R16, R17
    OUT PORTD, R16

