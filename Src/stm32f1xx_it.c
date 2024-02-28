/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

/* USER CODE BEGIN 0 */
extern uint16_t adc_data[6];//массив переменных для АЦП
extern char adc_number;//переменная для номера переменной АЦП
extern char tim6_start;//переменная для контроля таймера 6
extern char tim3_start;//переменная для контроля таймера 3
extern char tim7_start;//переменная для контроля таймера 7
extern char tim16_start;
extern char tim17_start;
extern TIM_HandleTypeDef htim3;//пременная таймера хз но без нее не работает
extern char error;//
extern char adc_4_obriv;
extern char adc_5_obriv;
extern char vse_v_nach;
extern char vse_v_kon;
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles DMA1 channel1 global interrupt.
*/
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
* @brief This function handles TIM1 break interrupt and TIM15 global interrupt.
*/
void TIM1_BRK_TIM15_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_TIM15_IRQn 0 */

  /* USER CODE END TIM1_BRK_TIM15_IRQn 0 */
  HAL_TIM_IRQHandler(&htim15);
  /* USER CODE BEGIN TIM1_BRK_TIM15_IRQn 1 */

	
if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)==GPIO_PIN_RESET)
{
	if(error==1)
{HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);}
else
	{HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);}
}
else
	{
	if(error==1)
	{HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);}
else
	{HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);}
  }
	
  /* USER CODE END TIM1_BRK_TIM15_IRQn 1 */
}

/**
* @brief This function handles TIM1 update interrupt and TIM16 global interrupt.
*/
void TIM1_UP_TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim16);
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 1 */
tim16_start=0;
HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);	
HAL_TIM_Base_Stop(&htim16);//запускаем таймер 6
HAL_TIM_Base_Stop_IT(&htim16);//запускаем прерывания от него


  /* USER CODE END TIM1_UP_TIM16_IRQn 1 */
}

/**
* @brief This function handles TIM1 trigger and commutation interrupts and TIM17 global interrupt.
*/
void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_TRG_COM_TIM17_IRQn 0 */

  /* USER CODE END TIM1_TRG_COM_TIM17_IRQn 0 */
  HAL_TIM_IRQHandler(&htim17);
  /* USER CODE BEGIN TIM1_TRG_COM_TIM17_IRQn 1 */
if((((adc_data[0]<2600)&(adc_data[0]>500))&(!((adc_data[5]<2850)&(adc_data[5]>2600)))&(!((adc_data[4]<2850)&(adc_data[4]>2600))))|
(((adc_data[1]<2600)&(adc_data[1]>500))&(!((adc_data[5]<2850)&(adc_data[5]>2600)))&(!((adc_data[4]<2850)&(adc_data[4]>2600))))|
(((adc_data[2]<2600)&(adc_data[2]>500))&(!((adc_data[5]<2850)&(adc_data[5]>2600)))&(!((adc_data[4]<2850)&(adc_data[4]>2600))))|
(((adc_data[3]<2600)&(adc_data[3]>500))&(!((adc_data[5]<2850)&(adc_data[5]>2600)))&(!((adc_data[4]<2850)&(adc_data[4]>2600)))))
/*если кл№1 не в кз и не в обрыве и нет пуска или
остальные кл.тоже*/
{
error=1;//переменная ошибки в единице
}
else//иначе
{
HAL_TIM_Base_Stop(&htim17);
HAL_TIM_Base_Stop_IT(&htim17);
}

  /* USER CODE END TIM1_TRG_COM_TIM17_IRQn 1 */
}

/**
* @brief This function handles TIM6 global interrupt and DAC underrun error interrupts.
*/
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */
if (error!=0)
{
if((tim3_start)!=0)  //если переменная контроля запуска таймера 3(пищалка) не равна 0,т.е.таймер запущен ранее  
{
HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);//включаем ШИМ
tim3_start=0;//и приравниваем эту переменную к 0    
}
else//если эта переменная равна 0
{
HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);//выключаем ШИМ
tim3_start=1; //и приравниваем ее к единице
}
/*таким образом мы генерируем пики с частотой примерно 5 Гц и интервалы между ними*/

for(adc_number=0;adc_number<6;adc_number++)/*когда у нас вылез ерор находим его источник
проверяем наши АЦП*/
{
if((adc_data[adc_number]>2850)|(adc_data[adc_number]<500))//если любая переменная в кз или обрыве
switch(adc_number)//переключатель
{
case 0://для №1
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_RESET)//если дип 1 в положении вкл.
{
if(adc_data[0]>2850)//если переменная в кз
{HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);}//мигаем диодом кз
if(adc_data[0]<500)//если переменная в обрыве
{HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_2);}//мигаем диодом обрыва
}
else//если дип 1 в положении выкл.
{
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);//гасим диоды кз и обрыва.
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);
} 
break;
case 1:
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_RESET)//если дип 2 в положении вкл.
{
if(adc_data[1]>2850)//если переменная в кз
{HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_10);}//мигаем диодом кз
if(adc_data[1]<500)//если переменная в обрыве
{HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_12);}//мигаем диодом обрыва
}
else//если дип 2 в положении выкл.
{
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);//гасим диоды кз и обрыва.
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
} 
break;
case 2:
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_RESET) //если дип 3 в положении вкл.   
{
if(adc_data[2]>2850)//если переменная в кз
{HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);}//мигаем диодом кз,переменную инкрементируем
if(adc_data[2]<500)//если переменная в обрыве
{HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_6);}//мигаем диодом обрыва,переменную инкрементируем
}
else//если дип 3 в положении выкл.
{
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);//гасим диоды кз и обрыва.
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
} 
break;
case 3:
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_RESET)//если дип 4 в положении вкл.
{
if(adc_data[3]>2850)//если переменная в кз
{HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);}//мигаем диодом кз
if(adc_data[3]<500)//если переменная в обрыве
{HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_7);}//мигаем диодом обрыва
}
else//если дип 4 в положении выкл.
{
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);//гасим диоды кз и обрыва.
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
} 
break;
case 4:
if(adc_data[4]>2850)
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);	
}
if(adc_data[4]<500)
{
if(adc_4_obriv!=1)
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
adc_4_obriv=1;
}
else
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
adc_4_obriv=0;
}
}
break;
case 5:
if(adc_data[5]>2850)
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
}
if(adc_data[5]<500)
{
if(adc_5_obriv!=1)
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
adc_5_obriv=1;
}
else
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
adc_5_obriv=0;
}
}
break;
}
}

}

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
* @brief This function handles TIM7 global interrupt.
*/
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */
if((tim3_start)!=0)  //если переменная контроля запуска таймера 3(пищалка) не равна 0,т.е.таймер запущен ранее  
{
HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);//включаем ШИМ
tim3_start=0;//и приравниваем эту переменную к 0    
}
else//если эта переменная равна 0
{
HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);//выключаем ШИМ
tim3_start=1; //и приравниваем ее к единице
}
  /* USER CODE END TIM7_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
