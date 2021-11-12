/**
* This file contains all the source code required
* for the interrupt service routines.
*/

#include "InterruptRoutines.h" 
#include "project.h"
int flagTEMP, flagLDR; //flags used to detect the status bits of each channel

int32 media_TEMP, media_LDR;

// Our global variables

extern int32 value_digit[]; // used to store the results of the A/D conversion
extern uint8_t slaveBuffer[];
extern uint8_t slaveCounter;

    

CY_ISR(Custom_Timer_Count_ISR)
{
    slaveCounter++;
    // Read timer status register to pull interrupt line low
    Timer_ADC_ReadStatusRegister(); // used  to read the status register value
    //1 case: Temperature and Light signals are sampled
    if (( flagLDR==1) & (flagTEMP==1)) // the status bits of the control resgister 0x00 are both set to 1
    {
        if (slaveCounter==0) AMux_Select(0); // to check that if the counter=0 the multiplexer is setted to the first channel
       
       
        value_digit[slaveCounter] = ADC_DelSig_Read32(); // The ADC is a 16-bit single ended mode so we use the ADC_Read32() function to make the conversion
        if (value_digit[slaveCounter] < 0) value_digit[slaveCounter] = 0; //check of the minimum and maximum positive value allowed for a 16 bit ADC
        if (value_digit[slaveCounter] > 65535) value_digit[slaveCounter] = 65535;
                
        if (slaveCounter == 4) // as required, when the first 5 samples are collected, we compute the mean
        {  
            for( int	i	=	0;	i < 5; i++ ) 
            {
                media_TEMP += (value_digit[i]);
            } 
            media_TEMP= media_TEMP/5;
            AMux_Select(1); // at this point we switch to the second channel of the multiplexer to acquire the light signal
            
        }
        
        if (slaveCounter== 9) //when the last 5 samples are collected, we compute the mean 
        {
            value_digit[slaveCounter] = ADC_DelSig_Read32();
            for (int i= 5;  i<10; i++)
                {
                    media_LDR += (value_digit[i]);
                }
                media_LDR= media_LDR/5;
                //we save the data relative to the sample acquisition in the dedicate SlaveBuffer register addresses
                slaveBuffer[3] = media_TEMP>>8; //We store the most significant Temperature related Bits in the 0x03 register 
                slaveBuffer[4] = media_TEMP & 0xFF;  //We store the least significant Temperature related Bits in the 0x04 register
                slaveBuffer[5] = media_LDR>>8; //We store the most significant Light related Bits in the 0x05 register 
                slaveBuffer[6] = media_LDR & 0xFF; //We store the least significant Light related Bits in the 0x06 register 
                AMux_Select(0); // at this point we switch to the first channel of the multiplexer in order to restart the data acquisition
                slaveCounter=-1; // we reset the counter    
        } 
    }
    
    //2 case: Only temperature signals are sampled
    
    if (( flagLDR==0) & (flagTEMP==1)) // status bit 1 is set to 0 and status bit 0 is set to 1. 
    {
        slaveBuffer[5]= 0;
        slaveBuffer[6] =0;
        value_digit[slaveCounter] = ADC_DelSig_Read32();
        if (slaveCounter == 4)
        {     
            for( int	i	=	0;	i < 5; i++ ) 
            {
                media_TEMP += (value_digit[i]);
            } 
            media_TEMP= media_TEMP/5;    
            slaveBuffer[3] = media_TEMP>>8;
            slaveBuffer[4] = media_TEMP & 0xFF;
            slaveCounter=-1;
            }
    }
    //3 case: Only light signals are sampled
    if (( flagLDR==1) & (flagTEMP==0)) // status bit 1 is set to 1 and status bit 0 is set to 0. 
    {
        slaveBuffer[3]= 0;
        slaveBuffer[4] =0;
        AMux_Select(1);
        value_digit[slaveCounter] = ADC_DelSig_Read32();
        if (slaveCounter == 4)
        {     
            for( int	i	=	0;	i < 5; i++ ) 
            {
                media_LDR += (value_digit[i]);
            }  
            media_LDR= media_LDR/5;
            slaveBuffer[5] = media_LDR>>8;
            slaveBuffer[6] = media_LDR & 0xFF;
            
            slaveCounter=-1;
        }
    }
    
 
}
        
            
/**
*   This function is called when exiting the EZI2C_ISR. Here we
*   perform all the tasks based on the requests.
*/
void EZI2C_ISR_ExitCallback(void){

    if ((slaveBuffer[0] & 0b11) == 0b11) //when the status bits of the ControlRegister0 are both 1 the Led is on

    {         
        Pin_LED_Write(1); // function that writes the value to the phisical port
        flagTEMP=1;
        flagLDR=1;        
    }
    if ((slaveBuffer[0] & 0b11)==0b00) //when the status bits of the ControlRegister0 are both 0, the Led is off
    {   
        Pin_LED_Write(0);
        flagTEMP= 0;
        flagLDR=0;
    }
    if ((slaveBuffer[0]& 0b11)==0b01) //when the status bits of the ControlRegister0 are  0 and 1  the Led is off
    {
        Pin_LED_Write(0);
        flagLDR=0;
        flagTEMP=1;
    }
     if ((slaveBuffer[0]& 0b11)==0b10) ///when the status bits of the ControlRegister0 are 1 and 0  the Led is off
    {
        Pin_LED_Write(0);
        flagLDR=1;
        flagTEMP=0;
    }
}

    
  
// [] END OF FILE */