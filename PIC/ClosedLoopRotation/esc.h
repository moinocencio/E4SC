#ifndef _ESC_H    
#define _ESC_H

#define successiveZC 10

extern volatile unsigned int driverMask;           // Mask of drivers to put to HIGH, on each state
extern volatile unsigned int NdriverMask;          // Mask of drivers to clear
extern volatile unsigned int state;                // state of the system

void updateMask();
void updateInterrupts();
void updateAllInterrupts();
void switchInterruptEdge();

#endif 

/* *****************************************************************************
 End of File
 */
