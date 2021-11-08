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
#include "project.h"
#include "InterruptRoutines.h"
#define slaveBuffer_size 7
uint8_t slaveBuffer[slaveBuffer_size];

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_StartEx(Custom_Timer_Count_ISR);
    Timer_ADC_Start();
    EZI2C_Start();
    EZI2C_SetBuffer1(slaveBuffer_size, 2, slaveBuffer);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
