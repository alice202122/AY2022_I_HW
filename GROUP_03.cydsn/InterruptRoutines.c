/**
* This file contains all the source code required
* for the interrupt service routines.
*/

// Include required header files
#include "InterruptRoutines.h" 

#include "project.h"
int flagTEMP, flagLDR;
extern int32 value_digit[];
int32 media_TEMP, media_LDR;



// Our global variables

extern uint8_t slaveBuffer[];
int8 slaveCounter;
    

CY_ISR(Custom_Timer_Count_ISR)
{
    // Read timer status register to pull interrupt line low
    Timer_ADC_ReadStatusRegister();
        if (( flagLDR==1) & (flagTEMP==1))
        {
            if (slaveCounter==0)
            {
                value_digit[slaveCounter] = ADC_DelSig_Read32();
                if (value_digit[slaveCounter] < 0) value_digit[slaveCounter] = 0;
                if (value_digit[slaveCounter] > 65535) value_digit[slaveCounter] = 65535;
            
            }
            if (slaveCounter == 4)
            {     
                
                for( int	i	=	0;	i < 5; i++ ) 
                {
                    media_TEMP += (value_digit[i])/5;
                    
                } 
                AMux_Select(1);
            }
            
            if (slaveCounter== 9)
            {
                
                for (int i= 5;  i<10; i++)
                {
                    media_LDR += (value_digit[i])/5;
                }
                slaveBuffer[3] = media_TEMP>>8;
                slaveBuffer[4] = media_TEMP & 0xFF;
                slaveBuffer[5] = media_LDR>>8;
                slaveBuffer[6] = media_LDR & 0xFF;
                
                AMux_Select(0);
                slaveCounter=0;     
                               
            }
        }
        
    
    // Increment counter 
    slaveCounter++;
}

/**
*   This function is called when exiting the EZI2C_ISR. Here we
*   perform all the tasks based on the requests.
*/
void EZI2C_ISR_ExitCallback(void){

    if ((slaveBuffer[0] & 0b11) == 0b11)
    
    {         
        Pin_LED_Write(1); 
        flagTEMP=1;
        flagLDR=1;        
    }
    if ((slaveBuffer[0] & 0b11)==0b00)
    {   
        Pin_LED_Write(0);
        flagTEMP= 0;
        flagLDR=0;
    }
    if ((slaveBuffer[0]& 0b11)==0b01)
    {
        Pin_LED_Write(0);
        flagLDR=0;
        flagTEMP=1;
    }
     if ((slaveBuffer[0]& 0b11)==0b10)
    {
        Pin_LED_Write(0);
        flagLDR=1;
        flagTEMP=0;
    }
}

    
  
// [] END OF FILE */
