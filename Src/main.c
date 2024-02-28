/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
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
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim15;
TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
volatile uint16_t adc_data[5];//массив переменных для АЦП
char error;//переменная для ошибки
char adc_number;//переменная для номера переменной АЦП
char tim6_start=0;//переменная для контроля таймера 6
char tim3_start=0;//переменная для контроля таймера 3
char tim7_start=0;//переменная для контроля таймера 7
char tim16_start=0;//переменная для контроля таймера 16
char tim17_start=0;//переменная для контроля таймера 16
char adc_4_obriv=0;//переменная для обрыва на пс
char adc_5_obriv=0;//переменная для обрыва на пс
char vse_v_nach=0;//переменная для контроля начального состояния
char vse_v_kon=0;//переменная для контроля конечного состояния
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM16_Init(void);
static void MX_TIM17_Init(void);
static void MX_TIM15_Init(void);
                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

void neispravnost(void)//функция неисправности
{
    if (error!=0)//если есть ошибка
    {
HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);//норму гасим
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);//неисправность зажигаем
if((tim6_start==0)&(tim16_start==0)) //если флаг таймера 6 и 16  снят заходим в блок
{	
HAL_TIM_Base_Start(&htim6);//запускаем таймер 6
HAL_TIM_Base_Start_IT(&htim6);//запускаем прерывания от него
tim6_start=1;//флаг запуска таймера 6 устнавливаем
}
    }
    else//если ошибок нет,
{
HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);//зажигаем норму
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);//гасим неисправность
HAL_TIM_Base_Stop(&htim6);//останавливаем таймер 6
HAL_TIM_Base_Stop_IT(&htim6);//останавливаем прерывания от таймера 6
tim6_start=0;//снимаем флаг таймера 6
if((tim7_start==0)&(tim6_start==0)&(tim16_start==0))//если таймеры 16,6,7 не запущены
{
tim3_start=0;//присваиваем переменной таймера 3 значение 0
HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_2);//останавливаем таймер 3	
}
}
if(((adc_data[0]>500)&(adc_data[0]<2850)|(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_SET))&//все ацп в норме и автоматика вкл.
((adc_data[1]>500)&(adc_data[1]<2850)|(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_SET))&
((adc_data[2]>500)&(adc_data[2]<2850)|(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_SET))&
((adc_data[3]>500)&(adc_data[3]<2850)|(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_SET))&
((adc_data[4]>500)&(adc_data[4]<2850))&
((adc_data[5]>500)&(adc_data[5]<2850))&
(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8)==GPIO_PIN_RESET)&(tim17_start==0)&
(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6)==GPIO_PIN_RESET))

	
{error=0;}// ошибки обнуляем
}


void kz_obriv(void)//функция кз и обрыва на концевиках клапанов и пусковых шлейфах
{
for(adc_number=0;adc_number<6;adc_number++)//для всех
{
if (adc_data[adc_number]<500)//если переменная в обрыве
{
switch(adc_number)//переключатель
{
case 0://для №1
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_RESET)//если дип 1 в положении вкл.
error=1;//выдаем ошибку
break;
case 1://для №2
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_RESET)//если дип 2 в положении вкл.
error=1;//выдаем ошибку
break;
case 2://для №3
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_RESET)//если дип 3 в положении вкл.
error=1;//выдаем ошибку
else//если дип 3 в положении выкл.
break;
case 3://для №4
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_RESET)//если дип 4 в положении вкл.
error=1;//выдаем ошибку
break;
case 4://для №4(ПС)
error=1;//выдаем ошибку
break;
case 5://для №5(ПС)
error=1;//выдаем ошибку
break;
}
}
if (adc_data[adc_number]>2850)//если переменная в кз
{   
switch(adc_number)//переключатель
{
case 0://для №1
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_RESET)//если дип 1 в положении вкл.
error=1;//выдаем ошибку
break;
case 1://для №2
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_RESET)//если дип 2 в положении вкл.
error=1;//выдаем ошибку
break;
case 2://для №3
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_RESET)//если дип 3 в положении вкл.
error=1;//выдаем ошибку
else//если дип 3 в положении выкл.
break;
case 3://для №4
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_RESET)//если дип 4 в положении вкл.
error=1;//выдаем ошибку
break;
case 4://для №4(ПС)
error=1;//выдаем ошибку
break;
case 5://для №5(ПС)
error=1;//выдаем ошибку
break;
}
}
}
}

void ps(void)//функция для пс
{
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_SET)//если дип1 выкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);}//реле без напр
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_SET)//если дип2 выкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);}//реле без напр
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_SET)//если дип3 выкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);}//реле без напр
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_SET)//если дип4 выкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);}//реле без напр

if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8)==GPIO_PIN_SET)//если авт выкл ключем
{
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);//зажигаем авт откл
error=1;//выдаем ошибку
}
else//если авт вкл 
{
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);//гасим авт откл
}
if((adc_data[5]<2850)&(adc_data[5]>2600))//если на шлейфе пс1 пуск
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);//гасим зеленый
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);//зажигаем красный 
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_SET);//реле без напряжением
}
if((adc_data[4]<2850)&(adc_data[4]>2600))//если на шлейфе пс2 пуск
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);//гасим зеленый
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);//зажигаем красный 
if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8)==GPIO_PIN_RESET)//если автоматика втключена 	
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_SET);}//реле без напряжения			
}	

if((tim6_start==0)&(tim6_start==0))//если таймер 6 и 16 не запущен
{
if(((adc_data[4]<2850)&(adc_data[4]>2600))|((adc_data[5]<2850)&(adc_data[5]>2600)))//и на каком либо пс сигнал пуск
{
HAL_TIM_Base_Start(&htim7);//запускаем таймер 7
HAL_TIM_Base_Start_IT(&htim7);//прерывания от него
tim7_start=1;//флаг запуска таймера 7
}
else//если пуска нет
{
HAL_TIM_Base_Stop(&htim7);//стоп таймер 7
HAL_TIM_Base_Stop_IT(&htim7);//прерывания от него
tim7_start=0;//флаг остановки таймера 7
}
}
else//если таймер 6 запущен
{
HAL_TIM_Base_Stop(&htim7);//стоп таймер 7
HAL_TIM_Base_Stop_IT(&htim7);//прерывания от него
tim7_start=0;//флаг остановки таймера 7
}

if((adc_data[5]<2600)&(adc_data[5]>500))//если на шлейфе пс1 норма
{	
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);//зажигаем зеленый
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);//гасим красный
if(!((adc_data[4]<2850)&(adc_data[4]>2600)))//если на другой пс нет пуска
{
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_RESET)//дип 1 вкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);}//реле под напряжением
else//дип1 выкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);}//реле без напряжения
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_RESET)//дип 2 вкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);}//реле под напряжением
else//дип2 выкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);}//реле без напряжения
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_RESET)//дип 3 вкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);}//реле под напряжением
else//дип3 выкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);}//реле без напряжения
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_RESET)//дип 4 вкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);}//реле под напряжением
else//дип4 выкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);}//реле без напряжения
}
else//если на другой пс пуск
{
if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8)==GPIO_PIN_RESET)//если автоматика вкл.
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_SET);//все реле без напряжения
}//реле без напряжения		
}
if((adc_data[4]<2600)&(adc_data[4]>500))//если на шлейфе пс2 норма
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);//зажигаем зеленый
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);//гасим красный 
if(!((adc_data[5]<2850)&(adc_data[5]>2600)))//если на другой пс нет пуска
{
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_RESET)//дип 1 вкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);}//реле под напряжением
else//дип1 выкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);}//реле без напряжения
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_RESET)//дип 2 вкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);}//реле под напряжением
else//дип2 выкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);}//реле без напряжения
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_RESET)//дип 3 вкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);}//реле под напряжением
else//дип3 выкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);}//реле без напряжения
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_RESET)//дип 4 вкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);}//реле под напряжением
else//дип4 выкл
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);}//реле без напряжения
}
else//если на другой пс пуск
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,GPIO_PIN_SET);}//реле без напряжения		
}
}


void konc_ind(void)//функция индикации положения клапана
{
for(adc_number=0;adc_number<4;adc_number++)//для переменных концевиков клапанов
{
if ((adc_data[adc_number]<2850)&(adc_data[adc_number]>2700))//если переменная входит в диапазон начального положения клапана
{
switch(adc_number) //переключатель   
{
case 0://для №1
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_RESET)//если дип 1 в положении вкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);}//зажигаем индикацию начального для №1
else//если дип 1 в положении выкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);}//гасим индикацию начального для №1
break;
case 1://для №2
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_RESET)//если дип 2 в положении вкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);}//зажигаем индикацию начального для №2
else//если дип 2 в положении выкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);}//гасим индикацию начального для №2
break;
case 2://для №3
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_RESET)//если дип 3 в положении вкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);}//зажигаем индикацию начального для №3
else//если дип 3 в положении выкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);}//гасим индикацию начального для №3
break;
case 3://для №4
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_RESET)//если дип 4 в положении вкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);}//зажигаем индикацию начального для №4
else//если дип 4 в положении выкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);}//гасим индикацию начального для №4
break;
}
}
else//если переменная НЕ входит в диапазон начального положения клапана
{
switch(adc_number)   //переключатель   
{
case 0://для №1
if(adc_data[0]<2850)//если переменная меньше переменной кз
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);}//гасим индикацию начального для №1
break;
case 1://для №2
if(adc_data[1]<2850)//если переменная меньше переменной кз
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);}//гасим индикацию начального для №2
break;
case 2://для №3
if(adc_data[2]<2850) //если переменная меньше переменной кз   
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);}//гасим индикацию начального для №3
break;
case 3://для №4
if(adc_data[3]<2850)//если переменная меньше переменной кз
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);}//гасим индикацию начального для №4
break;
}
}

if ((adc_data[adc_number]<2700)&(adc_data[adc_number]>2600))//если переменная входит в диапазон промежуточного положения клапана
{
switch(adc_number)   //переключатель    
{
case 0://для №1
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_RESET)//если дип 1 в положении вкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);}//зажигаем индикацию промежуточного для №1
else//если дип 1 в положении выкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);}//гасим индикацию промежуточного для №1
break;
case 1://для №2
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_RESET)//если дип 2 в положении вкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_SET);}//зажигаем индикацию промежуточного для №2
else//если дип 2 в положении выкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);}//гасим индикацию промежуточного для №2
break;
case 2://для №3
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_RESET)//если дип 3 в положении вкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);}//зажигаем индикацию промежуточного для №3
else//если дип 3 в положении выкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);}//гасим индикацию промежуточного для №3
break;
case 3://для №4
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_RESET)//если дип 4 в положении вкл.
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);}//зажигаем индикацию промежуточного для №4
else//если дип 4 в положении выкл.
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);}//гасим индикацию промежуточного для №4
break;
}
}
else//если переменная НЕ входит в диапазон промежуточного положения клапана
{
switch(adc_number)      //переключатель   
{
case 3://для №1
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);//гасим индикацию промежуточного для №1
break;
case 2://для №2
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,GPIO_PIN_RESET);//гасим индикацию промежуточного для №2
break;
case 1://для №3
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);//гасим индикацию промежуточного для №3
break;
case 0://для №4
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);//гасим индикацию промежуточного для №4
break;
}
}

if ((adc_data[adc_number]<2600)&(adc_data[adc_number]>500))//если переменная входит в диапазон конечного положения клапана
{
switch(adc_number)       //переключатель    
{
case 0://для №1
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_RESET)//если дип 1 в положении вкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);}//зажигаем индикацию конечного для №1
else//если дип 1 в положении выкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);}//гасим индикацию конечного для №1
break;
case 1://для №2
if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_RESET)//если дип 2 в положении вкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);}//зажигаем индикацию конечного для №2
else//если дип 2 в положении выкл.
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);}//гасим индикацию конечного для №2
break;
case 2://для №3
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_RESET)//если дип 3 в положении вкл.
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);}//зажигаем индикацию конечного для №3
else//если дип 3 в положении выкл.
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);}//гасим индикацию конечного для №3
break;
case 3://для №4
if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_RESET)//если дип 4 в положении вкл.
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);}//зажигаем индикацию конечного для №4
else//если дип 4 в положении выкл.
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);}//гасим индикацию конечного для №4
break;
}
}
else//если переменная НЕ входит в диапазон конечного положения клапана
{
switch(adc_number)      //переключатель
{
case 0://для №1
if(adc_data[0]>500)//если переменная больше переменной обрыва
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);}//гасим индикацию конечного для №1
break;
case 1://для №2
if(adc_data[1]>500)//если переменная больше переменной обрыва
{HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);}//гасим индикацию конечного для №2
break;
case 2://для №3
if(adc_data[2]>500)//если переменная больше переменной обрыва
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);}//гасим индикацию конечного для №3
break;
case 3://для №4
if(adc_data[3]>500)//если переменная больше переменной обрыва
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);}//гасим индикацию конечного для №4
break;
}
}
}
}

void koncevik(void)//функция концевика крышки
{
if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6)==GPIO_PIN_SET)//если концевик замкнут
{
error=1;//выдаем ошибку
}
}
void optron(void)//оптопары
{
if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)==GPIO_PIN_RESET)//если дип выключен(нормально разомкнутое состояние)
{
if(error==0)//норма
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);}//оптопара сет

else
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);}//если нет ресет

if ((((adc_data[0]<2850)&(adc_data[0]>2700))|(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_SET))&//если все в начальном сет
(((adc_data[1]<2850)&(adc_data[1]>2700))|(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_SET))&
(((adc_data[2]<2850)&(adc_data[2]>2700))|(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_SET))&
(((adc_data[3]<2850)&(adc_data[3]>2700))|(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_SET)))
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_14,GPIO_PIN_SET);
if (vse_v_nach==1)
{
tim16_start=1;
HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);	
HAL_TIM_Base_Start(&htim16);//запускаем таймер 6
HAL_TIM_Base_Start_IT(&htim16);//запускаем прерывания от него
vse_v_nach=0;
}
}
else
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_14,GPIO_PIN_RESET);
vse_v_nach=1;	
}	//если нет ресет

if(((adc_data[5]<2850)&(adc_data[5]>2600))|((adc_data[4]<2850)&(adc_data[4]>2600)))//если пуск сет
{HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13,GPIO_PIN_SET);}
else
{HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13,GPIO_PIN_RESET);}//если нет ресет

if(error==1)
{
HAL_TIM_Base_Start(&htim15);//запускаем таймер 6
HAL_TIM_Base_Start_IT(&htim15);//запускаем прерывания от него
}
else
{
HAL_TIM_Base_Stop(&htim15);//запускаем таймер 6
HAL_TIM_Base_Stop_IT(&htim15);//запускаем прерывания от него
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
}
if ((((adc_data[0]<2600)&(adc_data[0]>500))|(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_SET))&//если все в конечном сет
(((adc_data[1]<2600)&(adc_data[1]>500))|(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_SET))&
(((adc_data[2]<2600)&(adc_data[2]>500))|(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_SET))&
(((adc_data[3]<2600)&(adc_data[3]>500))|(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_SET)))
{
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
if (vse_v_kon==1)
{
tim16_start=1;
HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);	
HAL_TIM_Base_Start(&htim16);//запускаем таймер 6
HAL_TIM_Base_Start_IT(&htim16);//запускаем прерывания от него
vse_v_kon=0;
}
}
else
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
	vse_v_kon=1;
}//если нет ресет
}
else//то же только инверсия
{	
if (error==0)//норма
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_RESET);}
else
{HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);}

if ((((adc_data[0]<2850)&(adc_data[0]>2700))|(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_SET))&
(((adc_data[1]<2850)&(adc_data[1]>2700))|(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_SET))&
(((adc_data[2]<2850)&(adc_data[2]>2700))|(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_SET))&
(((adc_data[3]<2850)&(adc_data[3]>2700))|(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_SET)))
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_14,GPIO_PIN_RESET);
if (vse_v_nach==1)
{
tim16_start=1;
HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);	
HAL_TIM_Base_Start(&htim16);//запускаем таймер 6
HAL_TIM_Base_Start_IT(&htim16);//запускаем прерывания от него
vse_v_nach=0;
}
}
else
{
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_14,GPIO_PIN_SET);
vse_v_nach=1;
}	

if(((adc_data[5]<2850)&(adc_data[5]>2600))|((adc_data[4]<2850)&(adc_data[4]>2600)))
{HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13,GPIO_PIN_RESET);}
else
{HAL_GPIO_WritePin(GPIOA,GPIO_PIN_13,GPIO_PIN_SET);}

if(error==1)
{
HAL_TIM_Base_Start(&htim15);//запускаем таймер 6
HAL_TIM_Base_Start_IT(&htim15);//запускаем прерывания от него
}
else
{
HAL_TIM_Base_Stop(&htim15);//запускаем таймер 6
HAL_TIM_Base_Stop_IT(&htim15);//запускаем прерывания от него
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
}

if ((((adc_data[0]<2600)&(adc_data[0]>500))|(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_SET))&
(((adc_data[1]<2600)&(adc_data[1]>500))|(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_SET))&
(((adc_data[2]<2600)&(adc_data[2]>500))|(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_SET))&
(((adc_data[3]<2600)&(adc_data[3]>500))|(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_SET)))
{
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);
if (vse_v_kon==1)
{
tim16_start=1;
HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);	
HAL_TIM_Base_Start(&htim16);//запускаем таймер 6
HAL_TIM_Base_Start_IT(&htim16);//запускаем прерывания от него
vse_v_kon=0;
}
}
else
{
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
vse_v_kon=1;
}
}
}
void kontrol_220(void)
{
if((((adc_data[0]<2600)&(adc_data[0]>500))&(!((adc_data[5]<2850)&(adc_data[5]>2600)))&(!((adc_data[4]<2850)&(adc_data[4]>2600))))|
(((adc_data[1]<2600)&(adc_data[1]>500))&(!((adc_data[5]<2850)&(adc_data[5]>2600)))&(!((adc_data[4]<2850)&(adc_data[4]>2600))))|
(((adc_data[2]<2600)&(adc_data[2]>500))&(!((adc_data[5]<2850)&(adc_data[5]>2600)))&(!((adc_data[4]<2850)&(adc_data[4]>2600))))|
(((adc_data[3]<2600)&(adc_data[3]>500))&(!((adc_data[5]<2850)&(adc_data[5]>2600)))&(!((adc_data[4]<2850)&(adc_data[4]>2600)))))
{
HAL_TIM_Base_Start(&htim17);//запускаем таймер 6
HAL_TIM_Base_Start_IT(&htim17);//запускаем прерывания от него
tim17_start=1;
}
else
{tim17_start=0;}
	
}


/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_TIM15_Init();

  /* USER CODE BEGIN 2 */
HAL_ADC_Start_DMA(&hadc1,(uint32_t*) &adc_data,6);//запускаем DMA АЦП
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

//koncevik();//вызываем функцию контроля концевика крышки
kz_obriv();//вызываем функцию кз обрыв
ps();//пусковые шлейфы
konc_ind();//вызываем функцию положения клапана
neispravnost();//вызываем функцию неисправности
optron();
kontrol_220();
koncevik();
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 6;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 6;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim3);

}

/* TIM6 init function */
static void MX_TIM6_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 7999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 499;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM7 init function */
static void MX_TIM7_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 7999;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 1999;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM15 init function */
static void MX_TIM15_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 23999;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 1999;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM16 init function */
static void MX_TIM16_Init(void)
{

  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 23999;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 2999;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM17 init function */
static void MX_TIM17_Init(void)
{

  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 23999;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 59999;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10 
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 
                          |GPIO_PIN_15|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3 
                           PC6 PC7 PC8 PC9 
                           PC10 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC4 PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB10 
                           PB11 PB12 PB13 PB14 
                           PB15 PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10 
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 
                          |GPIO_PIN_15|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA11 PA12 PA13 
                           PA14 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
