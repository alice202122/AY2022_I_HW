/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
/*BEFORE PROGRAMMING, IT IS NECESSARY TO ADD THE ANNOTATION LIBRARY*/
#include "project.h"
#include "InterruptRoutines.h"
#include "stdio.h"
#define slaveBuffer_size 7 //our EZI2C is composed by 7 registers
uint8_t slaveBuffer[slaveBuffer_size]; 
int32 value_digit[10]; 
uint8_t slaveCounter;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    /* Start the components */    
    isr_StartEx(Custom_Timer_Count_ISR); 
    AMux_Start();
    AMux_Select(0);
    ADC_DelSig_Start();
    Timer_ADC_Start();
    EZI2C_Start();
    slaveBuffer[2] = 0xBC; //WHO AM I register of the slave buffer set to 0xBC
    slaveBuffer[1]= 20; //Timer period considering 5 samples (computed as: clock_frequency/n_samples*transmission frequency)
    //in our case n_samples =5*2 (since we have two signals), clock_frequency = 10kHz, transmission_freq = 50Hz)
    slaveCounter=-1; 
    EZI2C_SetBuffer1(slaveBuffer_size, 2, slaveBuffer); //Sets up the data buffer to be exposed to the master on a primary slave address request. 
    //as arguments of this function we pass the slave buffer size (7), the number of register on which it is possible to both, read and write, and the slave buffer variable.

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
