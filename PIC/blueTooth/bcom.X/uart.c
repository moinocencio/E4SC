// #include "config_bits.h"
#include "uart.h"
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>


void UART_Init(uint32_t br, uint8_t stob, char parity, uint32_t pb_f)
{ 
    U1MODE = 0x0000;
    U1MODEbits.BRGH = 0;
    U1BRG = ((pb_f + 8 * br) / (16 * br)) - 1;
    
    switch(parity){
        case 'N': U1MODEbits.PDSEL = 0b00; break;
        case 'E': U1MODEbits.PDSEL = 0b01; break;
        case 'O': U1MODEbits.PDSEL = 0b11; break;
        default : U1MODEbits.PDSEL = 0b00; break;
    }
    U1MODEbits.STSEL = (stob == 1 ? 0 : 1);

    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    TRISFbits.TRISF2 = 1;			// Rx - 52
    TRISFbits.TRISF8 = 0;    		// Tx - 53	

    U1MODEbits.ON = 1;
    
    __XC_UART = 1;
}

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
    
    TRISFbits.TRISF2 = 1;			// Rx - 52
    TRISFbits.TRISF8 = 0;    		// Tx - 53	    
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
    for(i = 0; i < length; i++)
    {
       aux = ReadChar();
       if(aux=='\n') break;
       string[i] = aux;
    }  
}

void config_U1_int()
{
    // IPC6bits.U1IS = ;    // Sub-priority
    IPC6bits.U1IP = 3;      // Priority
    IEC0bits.U1TXIE = 0;    // Enable 
    IEC0bits.U1RXIE = 1;    // Enable   
    IFS0bits.U1TXIF = 0;
    IFS0bits.U1RXIF = 0;
}


