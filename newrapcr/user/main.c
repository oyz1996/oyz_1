/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "freehead.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart6.h"
#include "iic.h"
#include "adc.h"
#include "tim1.h"
#include "tim9.h"
#include "tim3.h"
#include "exti.h"
#include "control.h"
#include "elect.h"
#include "resolv.h"
#include "worksequ.h"
#include "commwind.h"
#include "inspection.h"
#include <tgmath.h>
#include <stdio.h>
#include <string.h>
/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
uint16_t oji = 0;
uint16_t oji1 = 0;
float oyziu ;
extern int oioi;

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

 		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
		SystemCoreClockUpdate();
	
		GPIO_Config();

		ADC_Config();
		IIC_Init();
		USART1_Init();
		USART2_Init();
		USART3_Init();
		USART6_Init();
		EXTI5_Init();
		Elec_Init();
		Init_Speed();
	
		TIM1_Config(TIM1_25KHZ_PSC,TIM1_25KHZ_ARR);
		TIM9_Config(TIM9_25KHZ_PSC,TIM9_25KHZ_ARR);
		

		Calcu_Config(&FrePeriStruct);	
		Calcu_Config(&FrefubenStruct);	
		TIM3_Config(TIM3_20KHZ_PSC,TIM3_20KHZ_ARR);
	
		Init_Control();
		Init_Inspection();
		Init_WorkSequ();
		Init_CommWind();							
		
  /* Infinite loop */
	 
		while (1)
		{		
			vTaskStartScheduler();
		}
}


/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
