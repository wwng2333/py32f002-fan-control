/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by Puya under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef    TimHandle;
TIM_OC_InitTypeDef   sConfig;

ADC_HandleTypeDef             hadc;
volatile uint16_t             aADCxConvertedData;
uint16_t pwmOutputValue = 0;
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_TimConfig(void);
static void APP_TimPwmConfig(void);
static void APP_ADCConfig(void);
uint16_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max);


/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Systick. */ 
  HAL_Init();
  
  /* TIM Config */
  APP_TimConfig();

  /* TIM PWM Config */
  APP_TimPwmConfig();

  /* Configure ADC */
  APP_ADCConfig();
  
  /* Channel4 Start PWM */
  if (HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_4) != HAL_OK)                  
  {
    APP_ErrorHandler();
  }
  while (1)
  {
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1000);
    aADCxConvertedData = HAL_ADC_GetValue(&hadc);
    pwmOutputValue = map(aADCxConvertedData, 20, 2100, 96, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, pwmOutputValue);
    __HAL_TIM_SET_COMPARE(&TimHandle, TIM_CHANNEL_4, pwmOutputValue);
    HAL_Delay(100);
  }
}

/**
  * @brief  TIM Configure
  * @param  None
  * @retval None
  */
static void APP_TimConfig(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();                             /* Enable GPIO clock */

  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;                   /* Push-pull output */
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;                     /* Pull-down */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;             /* GPIO speed */
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;               /* TIM1_CH4 */
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);                   /* Initialize GPIO */
  
  /* TIM1 */
  TimHandle.Instance = TIM1;                                                  
  
  /* Period = 96 - 1  */
  TimHandle.Init.Period            = 96 - 1;                                     

  /* Prescaler = 10 - 1 */
  TimHandle.Init.Prescaler         = 10 - 1;                                 

  /* ClockDivision = 0  */
  TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;                  

  /* Counter direction = Up */
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;                      

  /* Repetition = 0 */
  TimHandle.Init.RepetitionCounter = 1 - 1;                                   

  /* Auto-reload register not buffered  */
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;          

  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)                                
  {
    APP_ErrorHandler();
  }

}
/**
  * @brief  TIM1 PWM Configure
  * @param  None
  * @retval None
  */
static void APP_TimPwmConfig(void)
{
  /* Set output compare mode: PWM1 */
  sConfig.OCMode       = TIM_OCMODE_PWM1;                                     

  /* OC channel output high level effective */
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;                                 

  /* Disable OC FastMode */
  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;                                  

  /* OCN channel output high level effective */
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;                                

  /* Idle state OC1N output low level */
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;                              

  /* Idle state OC1 output low level*/
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;                               

  sConfig.Pulse = pwmOutputValue;                                               

  /* Channel 4 configuration */
  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_4) != HAL_OK)
  {
    APP_ErrorHandler();
  }
}

/**
  * @brief  ADC configuration function
  * @param  None
  * @retval None
  */
static void APP_ADCConfig(void)
{
  ADC_ChannelConfTypeDef        sConfig;
  __HAL_RCC_ADC_CLK_ENABLE();                                                /* Enable ADC clock */

  hadc.Instance = ADC1;

  hadc.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;                /* Set ADC clock*/
  hadc.Init.Resolution            = ADC_RESOLUTION_12B;                      /* 12-bit resolution for converted data */
  hadc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;                     /* Right-alignment for converted data */
  hadc.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;              /* Scan sequence direction: forward */
  hadc.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;                     /* Single Conversion */
  hadc.Init.LowPowerAutoWait      = DISABLE;                                 /* Auto-delayed conversion feature disabled */
  hadc.Init.ContinuousConvMode    = DISABLE;                                 /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  hadc.Init.DiscontinuousConvMode = DISABLE;                                 /* Disable discontinuous mode */
  hadc.Init.ExternalTrigConv      = ADC_SOFTWARE_START;                      /* Software start to trig the 1st conversion manually, without external event */
  hadc.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;           /* Parameter discarded because software trigger chosen */
  hadc.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;                /* DR register is overwritten with the last conversion result in case of overrun */
  hadc.Init.SamplingTimeCommon    = ADC_SAMPLETIME_239CYCLES_5;              /* The channel sampling time is 239.5 ADC clock cycles */
  if (HAL_ADC_Init(&hadc) != HAL_OK)                                         /* ADC initialization */
  {
    APP_ErrorHandler();
  }

  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;                             
  sConfig.Channel      = ADC_CHANNEL_1;                                      
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)                       /* Configure ADC Channel */
  {
    APP_ErrorHandler();
  }
    
  if (HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)                           /* ADC Calibration */
  {
    APP_ErrorHandler();
  }
}

uint16_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  if (in_max == in_min) {
    return out_min;
  }
  
  int32_t input_low = (in_min < in_max) ? in_min : in_max;
  int32_t input_high = (in_min < in_max) ? in_max : in_min;

  if (x < input_low) {
    x = input_low;
  } else if (x > input_high) {
    x = input_high;
  }

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void APP_ErrorHandler(void)
{
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
