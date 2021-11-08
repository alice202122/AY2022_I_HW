/**
* This file contains all the source code required
* for the interrupt service routines.
*/

// Include required header files
#include "InterruptRoutines.h" 
#include "project.h"


// Our global variables

extern uint8_t slaveBuffer[];
    

CY_ISR(Custom_Timer_Count_ISR)
{
    // Read timer status register to pull interrupt line low
    Timer_ADC_ReadStatusRegister();
    // Increment counter in slave buffer
    slaveBuffer[1]++;
}

/**
*   This function is called when exiting the EZI2C_ISR. Here we
*   perform all the tasks based on the requests.
*/
void EZI2C_ISR_ExitCallback(void)
{
    if ((slaveBuffer[0] & 0b11) == 0b11)
    {
        Pin_LED_Write(1); 
    }
    
}
/* [] END OF FILE */
