// #include "config_bits.h"
#include "uart.h"
#include "int.h"
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

void config_uart()
{
    U1MODEbits.BRGH = 0; // 9600 baudrate
    U1BRG = 256; //257; 
    U1MODEbits.SIDL = 0;
    U1MODEbits.IREN = 0;
    U1MODEbits.RTSMD = 0;
    U1MODEbits.UEN = 0b00;
    U1MODEbits.WAKE = 1;
    U1MODEbits.LPBACK = 0;
    U1MODEbits.RXINV = 0;
    U1MODEbits.PDSEL = 0b00;
    // U1MODEbits.STSEL = 0;
    U1MODEbits.STSEL = 1;   // Bluetooth
    
        // U1STAbits.UTXINV = 0;
    U1STAbits.URXEN = 1;
    U1STAbits.UTXEN = 1;
    
    TRISFbits.TRISF2 = 1;			
    TRISFbits.TRISF8 = 0;    			    
    U1MODEbits.ON = 1;
    __XC_UART = 1;
}

void SendChar(char c)
{
    U1STAbits.UTXEN = 1;                // Make sure transmitter is enabled
    while(U1STAbits.UTXBF);             // Wait while buffer is full
    U1TXREG = c;                        // Transmit character
}

void SendString(char *string)
{
   int i = 0;     
   U1STAbits.UTXEN = 1;                // Make sure transmitter is enabled   
   while(*string)
    {
        while(U1STAbits.UTXBF);         // Wait while buffer is full
        U1TXREG = *string;              // Transmit one character
        string++;                       // Go to next character in string
    }
}

char ReadChar(void)
{
    if(U1STAbits.OERR == 1){
        U1STAbits.OERR = 0;
    }
    while(U1STAbits.URXDA == 0);            // Wait for information to be received
    return U1RXREG;                         // Return received character
}

void ReadString(char *string, int length)
{  
    char aux;
    int i;
    disableInterrupts();
    for(i = 0; i <= length; i++)
    {
       aux = ReadChar();
       if(aux=='\n') break;
       string[i] = aux;
    }
    enableInterrupts();
}