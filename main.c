#pragma config FOSC = INTIO67      
#pragma config PLLCFG = ON         
#pragma config WDTEN = OFF         

#include <xc.h>

#define _XTAL_FREQ 64000000UL      

volatile unsigned char duty = 0;   

// -------- Configuración PWM (CCP1 en RC2) --------
void setupPWM(void) {
    TRISC2 = 0;                    
    PR2 = 255;                     
    CCP1CON = 0b00001100;          
    CCPR1L = 0;                    
    T2CON = 0b00000111;            
}

// -------- Cambiar el duty cycle (0 a 255) --------
void setDuty(unsigned char val) {
    CCPR1L = val;
}

// -------- Configuración del Timer0 para 100 ms --------
void setupTimer0(void) {
    T0CON = 0b10000111;            
    TMR0 = 3036;                   
    INTCONbits.TMR0IE = 1;         
    INTCONbits.TMR0IF = 0;        
    INTCONbits.GIE = 1;            
}

// -------- Interrupción de Timer0 --------
void __interrupt() ISR(void) {
    if (INTCONbits.TMR0IF) {
        TMR0 = 3036;               
        duty += 20;                
        if (duty > 255) duty = 0;  
        setDuty(duty);             
        INTCONbits.TMR0IF = 0;     
    }
}

// -------- Main --------
void main(void) {
    OSCCON = 0b01110000;       
    OSCTUNEbits.PLLEN = 1;     
    
    setupPWM();                    
    setupTimer0();                 

    while (1) {
        
    }
}
