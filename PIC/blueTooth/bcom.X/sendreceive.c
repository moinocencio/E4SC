#include <stdio.h>
#include <string.h>
#include <xc.h>
#include <stdbool.h>
#define _XTAL_FREQ 20000000


void uart_init();
void uart_transmit(unsigned char *toTxfr, unsigned int size);
void uart_receive(unsigned char *toRcv, unsigned int size);

static bool txCompletion = 0;
static bool rxCompletion = 0;
static const unsigned char atEo[]={"ATE0\r\n"};
static const unsigned char at[]={"AT\r\n"};
unsigned char rcvBufffer[10] = {0};
unsigned int count = 0;
unsigned char *dataToTxfr = NULL;
unsigned int numOfDatTxfr = 0;

void main()
{
 uart_init();
 uart_transmit(&atEo[0], 6); // send the AT command
    while(txCompletion == 0);
    uart_receive(&rcvBufffer[0],2); // Receive the response for the send command
    while(rxCompletion == 0);
 //uart_transmit(at, 4);
 //uart_receive(rcvBufffer,2);
    while(1)
    {
        
    }
}

void uart_init()
{
 //ANSELD = 0x00; // change the analog pins to digital for uart2
    ANSELC = 0x00; // change the analog pins to digital for uart1
 TXSTA1 = 0x04; 
 RCSTA1 = 0x80;
    BAUDCON1 = 0x00;
 SPBRG1 = 129; 
 GIE = 1; // assumption is IPEN = 0
 PEIE = 1; // assumption is IPEN = 0
}

void uart_transmit(unsigned char *toTxfr, unsigned int size)
{
    TXSTA1 |= 0x20; 
 TX1IE = 1;
 dataToTxfr = toTxfr;
 numOfDatTxfr = size;
}

void uart_receive(unsigned char *toRcv, unsigned int size)
{
    //TXSTA1 &= ~(0x10); 
    SYNC1 = 0;
    RCSTA1 |= 0x10;
    RC1IE = 1;
    dataToTxfr = toRcv;
    numOfDatTxfr = size;
}

void interrupt uart()
{
 //Check the transmit interrupt flag is ready for the next byte transmission
    if(TX1IF)
    {
        TXREG1 = dataToTxfr[count];
        count++;
    }
    // If the size of the the data to be transferred is reached then disable the
    // transmit interrupt
    if(count == numOfDatTxfr)
    {
           //count = 0;
     TX1IE = 0;
           TXSTA1 &= ~(0x20); 
           txCompletion = 1;
    }
 
 //Check the receive interrupt flag is ready for the next byte receive
    if(RC1IF)
    {
               if(FERR1 != 1)
               {
                       dataToTxfr[count] = RCREG1;
                       if(OERR1 == 1)
                       {
                           CREN1 = 0;
                           CREN1 = 1;
                       }
                       count++;
               }
               else
               {
                   count = 0;
               }
   }
    // If the size of the the data to be received is reached then disable the
    // receive interrupt
    if(count == numOfDatTxfr)
    {
        count = 0;
        RC1IE = 0;
        RCSTA1 &= ~(0x10);
        rxCompletion = 1;
    }
}