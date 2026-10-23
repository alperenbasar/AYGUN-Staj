#include <xc.h>

// --- KONFIGÜRASYON ---
#pragma config FOSC = INTOSC  
#pragma config WDTE = OFF     
#pragma config PWRTE = ON     
#pragma config MCLRE = OFF    
#pragma config CP = OFF       
#pragma config CPD = OFF      
#pragma config BOREN = OFF    // Voltaj dalgalanmasinda reset atmayi kapatma
#pragma config CLKOUTEN = OFF 
#pragma config IESO = OFF     
#pragma config FCMEN = OFF    
#pragma config LVP = OFF      

#define _XTAL_FREQ 4000000    

// --- PIN TANIMLAMALARI ---
#define LED1 LATA5   
#define LED2 LATA4   
#define LED3 LATA2   
#define STAT_PIN RA3 

void main(void) {
    OSCCON = 0b01101010; 
    TRISA = 0b00001000; 
    ANSELA = 0x00; 

    // Dahili Pull-Up 
    OPTION_REGbits.nWPUEN = 0; 
    WPUA = 0b00001000;         

    LATA = 0x00; 

    while(1) {
        // YAZILIMSAL FILTRE
        int okuma1 = STAT_PIN;
        __delay_ms(50); 
        int okuma2 = STAT_PIN;
        
        if(okuma1 == okuma2) {
            
            if(okuma1 == 1) { 
                // --- SARJ OLUYOR ---
                LED1 = 1; LED2 = 0; LED3 = 0;
                __delay_ms(300);
                
                LED1 = 1; LED2 = 1; LED3 = 0;
                __delay_ms(300);
                
                LED1 = 1; LED2 = 1; LED3 = 1;
                __delay_ms(300);
                
                LED1 = 0; LED2 = 0; LED3 = 0;
                __delay_ms(300);
                
            } else {
                // --- SARJ BITTI ---
                LED1 = 1;
                LED2 = 1;
                LED3 = 1;
            }
        }
    }
}