// ADC, DMA, USART
//------------------------------------------------------------------
// Includes
#include "stm32f10x.h"
#include "stm32F10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "MT_STM32F10x_USART.h"
//------------------------------------------------------------------
//------------------------------------------------------------------
uint8_t ADC_buffer[8];
int readyToTransmitFlag = 0;
uint8_t TXi=0;
uint8_t TXc;

//------------------------------------------------------------------
//------------------------------------------------------------------
void SendStrtoPC(uint8_t* ADC_buffer_p, uint8_t count)
{
               uint8_t i=0;
        
        while(i < count)
        {
            while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
            USART_SendData(USART2, ADC_buffer_p[i]);
            i++;
        }
}
//------------------------------------------------------------------

//------------------------------------------------------------------
void Timer_ADC (void)
{
// If HCLK = 100 MHz, then frequency of APB1 timer clocks = 100 MHz, 
// Prescaler = 50000, Period = 1000, T = 500 msec
    TIM_TimeBaseInitTypeDef TIM_Time_user;
        
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    TIM_Time_user.TIM_Prescaler = 50000-1;
    TIM_Time_user.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_Time_user.TIM_Period = 1000-1;
    TIM_Time_user.TIM_ClockDivision = TIM_CKD_DIV1;
    
    TIM_TimeBaseInit(TIM3, &TIM_Time_user);
            
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
    TIM_Cmd(TIM3, ENABLE);
}
//------------------------------------------------------------------
//------------------------------------------------------------------
void GPIO_ini (void)
{
}
//------------------------------------------------------------------
 
//------------------------------------------------------------------
void ADC_ini (void)
{
    
   GPIO_InitTypeDef PORT_init_struct;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

   PORT_init_struct.GPIO_Pin = GPIO_Pin_0;
   PORT_init_struct.GPIO_Speed = GPIO_Speed_10MHz;
   PORT_init_struct.GPIO_Mode = GPIO_Mode_AIN;

   GPIO_Init(GPIOC, &PORT_init_struct);
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
        ADC_InitTypeDef ADC_Init_user;   
        ADC_Init_user.ADC_ScanConvMode = ENABLE;
        ADC_Init_user.ADC_ContinuousConvMode = DISABLE;
        ADC_Init_user.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
        ADC_Init_user.ADC_DataAlign = ADC_DataAlign_Right;
        ADC_Init_user.ADC_NbrOfChannel = 1; 
        ADC_Init(ADC1, &ADC_Init_user);
    
        ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_41Cycles5);
        NVIC_EnableIRQ(ADC1_2_IRQn);   
    
        ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

    ADC_Cmd(ADC1, ENABLE);
}
//------------------------------------------------------------------
//------------------------------------------------------------------
void DMA_ADC (void)
{
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
     DMA_DeInit(DMA1_Channel1);
         DMA_InitTypeDef DMA_ini_ADC;
         DMA_ini_ADC.DMA_PeripheralBaseAddr = (uint8_t)&(ADC1->DR);
         DMA_ini_ADC.DMA_MemoryBaseAddr = (uint8_t)ADC_buffer;
         DMA_ini_ADC.DMA_DIR = DMA_DIR_PeripheralDST;
         DMA_ini_ADC.DMA_BufferSize = sizeof(ADC_buffer);
         DMA_ini_ADC.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
         DMA_ini_ADC.DMA_MemoryInc = DMA_MemoryInc_Enable;
         DMA_ini_ADC.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
         DMA_ini_ADC.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
         DMA_ini_ADC.DMA_Mode = DMA_Mode_Circular;
         DMA_ini_ADC.DMA_Priority = DMA_Priority_High; 
         DMA_Init(DMA1_Channel1, &DMA_ini_ADC);  
         ADC_DMACmd(ADC1, ENABLE);   
    
     NVIC_EnableIRQ (DMA1_Channel1_IRQn); 
     DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
     
     DMA_Cmd(DMA1_Channel1, ENABLE);
 }

 // Interrupt of DMA function

void DMA1_Channel1_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA1_IT_TC1);
    readyToTransmitFlag = 1;
}
//------------------------------------------------------------------
void USART2_ini(void)
{
    
    // Pins configyration:
    // TX - PA2
    // RX - PA3
    // GND - GND
    
    // Setting the pins 
    GPIO_InitTypeDef GPIO_Init_USART;
    USART_InitTypeDef USART_InitUser;
    
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_StructInit(&GPIO_Init_USART);
	GPIO_Init_USART.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init_USART.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init_USART.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_Init_USART);
	
	GPIO_Init_USART.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init_USART.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init_USART.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Init_USART);
    
    USART_InitUser.USART_BaudRate=9600;
    USART_InitUser.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitUser.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
    USART_InitUser.USART_Parity=USART_Parity_No;
    USART_InitUser.USART_StopBits=USART_StopBits_1;
    USART_InitUser.USART_WordLength=USART_WordLength_8b;
    
    USART_Init(USART2, &USART_InitUser);
    
    USART_Cmd(USART2, ENABLE);
    
}

//------------------------------------------------------------------
int main(void) 
{
//------------------------------------------------------------------
// Initialization
//  
    USART2_ini();
    Timer_ADC();
    GPIO_ini();
    ADC_ini();
    DMA_ADC();
//------------------------------------------------------------------
    
    
//------------------------------------------------------------------
// Delay definition
//
 /*   DWT_DelayUpdate();
    DWT_Init();
    uint32_t DWT_GetTick(void);
    void DWT_Delay_sec(uint32_t sec);
    void DWT_Delay_ms(uint32_t ms);
    void DWT_Delay_us(uint32_t us);
    uint8_t DWT_Test_sec(uint32_t start, uint32_t time);
    uint8_t DWT_Test_ms(uint32_t start, uint32_t time);
    uint8_t DWT_Test_us(uint32_t start, uint32_t time);
    uint32_t DWT_Time_sec(uint32_t start, uint32_t current);
    uint32_t DWT_Time_ms(uint32_t start, uint32_t current);
    uint32_t DWT_Time_us(uint32_t start, uint32_t current); */
//------------------------------------------------------------------
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_EnableIRQ(TIM3_IRQn);
    USART_ITConfig(USART2, USART_IT_TC, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    MT_USART_SendData(ADC_buffer, 8);
    while(1)
    {
//------------------------------------------------------------------        
         
        if(TXi<10) 
        {
         TXc = ADC1->DR >>4;
         ADC_buffer[TXi] = TXc;
         TXi++;
        }
         else
        {
         TXi=0;
        }
         if (readyToTransmitFlag)
        {
            //SendStrtoPC(ADC_buffer, 8);
            MT_USART_SendData(ADC_buffer, 8);
            readyToTransmitFlag = 0;
        }
        
//------------------------------------------------------------------                
    }
 
}

void TIM3_IRQHandler()
{
    
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
     if(ADC1->SR & ADC_SR_EOC)
	{
	   
    if(TXi<10) 
    {
    TXc = ADC1->DR >>4;
        ADC_buffer[TXi] = TXc;
        TXi++;
    }
    else
    {
    TXi=0;
    }
	}
    ADC1->SR=0; 
}

