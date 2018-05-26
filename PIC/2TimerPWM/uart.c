// #include "config_bits.h"
#include "uart.h"
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
    U1MODEbits.STSEL = 0;
    U1MODEbits.ON = 1;
    
    U1STAbits.UTXINV = 0;
    U1STAbits.URXEN = 1;
    U1STAbits.UTXEN = 1;
    
    __XC_UART=1;
}

void SendChar(char c)
{
    U1STAbits.UTXEN = 1;                // Make sure transmitter is enabled
    // while(CTS)                       // Optional CTS use
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
    // RTS = 0                           // Optional RTS use
    while(!U1STAbits.URXDA)             // Wait for information to be received
    // RTS = 1
    return U1RXREG;                     // Return received character
}

void ReadString(char *string, int length)
{  
    int count = length;
     
    do
    {
        *string = ReadChar();               // Read in character
        SendChar(*string);                  // Echo character
         
        if(*string == 0x7F && count>length) // Backspace conditional
        {
            length++;
            string--;
            continue;
        }       
        if(*string == '\r')                 // End reading if enter is pressed
            break;
         
        string++;
        length--;        
    }while(length>1);    
    *string = '\0';                         // Add null terminator
}
