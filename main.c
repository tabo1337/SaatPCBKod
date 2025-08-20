/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "ee.h"
#include "ds3231.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
// 0--63,  n--55, F--113,  A--119, b--124 
//              0,1, 2, 3, 4,  5,  6, 7, 8, 9,10,11,12,13,   14, 15, 16, 17
uint8_t T[18]={63,6,91,79,102,109,125,7,127,111,64,0,99,118,119,124,113,55};

uint8_t i,birler,onlar,yuzler,binler,sanon,sanbir;
uint8_t s1,s2,s3,s4;
bool menu;

uint8_t ds3231Addr = (uint8_t) 0xD0;
uint8_t gun,ay,yil,haftagun;
uint8_t saat,dakika,saniye;


uint8_t rakam;

uint8_t saat1,dakika1,saniye1,gun1,ay1,yil1,haftagun1,ayardeger,menudeger;
uint16_t deger,ab=0,ab1,ab2,ac1=0,ac2=0,ac3=0,ad=0,ae=0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	
 if(HAL_GPIO_ReadPin(GPIOB,AYAR_Pin)==1){HAL_Delay(300);deger++;if(deger>5)deger=1;}
			
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
// *********************** DISPLAY TARAMA BASLANGIÇ ******************************
// display ledlerin titrememesi için verilmesi gerekn timer kesme degerleri su sekilde olmali
// TIM_CLK=72MHZ TIM_PSC=23999 ARR=1 
// hesaplama sonucu frekans 1500 HZ çikmakta. Periyot 0,00066 saniye oldugu zaman ledler titremiyor. 	
	if(htim->Instance == htim1.Instance) {
	
	i++;
		
			if(i<=5) { //5
			GPIOA->ODR =	(GPIOA->ODR & 0xFFFFFF80) | T[birler];
			HAL_GPIO_WritePin(GPIOB,birler_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB,onlar_Pin,GPIO_PIN_RESET);	
			HAL_GPIO_WritePin(GPIOB,yuzler_Pin,GPIO_PIN_RESET);	
			HAL_GPIO_WritePin(GPIOB,binler_Pin,GPIO_PIN_RESET);
              		}
			
									
			if(i>5&&i<=11) { 
		  GPIOA->ODR =	(GPIOA->ODR & 0xFFFFFF80) | T[onlar];	
			HAL_GPIO_WritePin(GPIOB,birler_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,onlar_Pin,GPIO_PIN_SET);	
			HAL_GPIO_WritePin(GPIOB,yuzler_Pin,GPIO_PIN_RESET);	
			HAL_GPIO_WritePin(GPIOB,binler_Pin,GPIO_PIN_RESET);	
              		}						
									
												
			if(i>11&&i<=16) {
			GPIOA->ODR =	(GPIOA->ODR & 0xFFFFFF80) | T[yuzler];				
			HAL_GPIO_WritePin(GPIOB,birler_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,onlar_Pin,GPIO_PIN_RESET);	
			HAL_GPIO_WritePin(GPIOB,yuzler_Pin,GPIO_PIN_SET);	
			HAL_GPIO_WritePin(GPIOB,binler_Pin,GPIO_PIN_RESET);	
              		}						
									
									
			if(i>16) {
			GPIOA->ODR =	(GPIOA->ODR & 0xFFFFFF80) | T[binler];		
			HAL_GPIO_WritePin(GPIOB,birler_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB,onlar_Pin,GPIO_PIN_RESET);	
			HAL_GPIO_WritePin(GPIOB,yuzler_Pin,GPIO_PIN_RESET);	
			HAL_GPIO_WritePin(GPIOB,binler_Pin,GPIO_PIN_SET);	
				
      if(i>21)i=0;
								}	
}

// *********************** DISPLAY TARAMA SON ************************************

// *********************** SANIYE LEDLERI BASNANGIÇ ******************************
	if(htim->Instance == htim2.Instance) {
		ab++;

	if(ab>0&&ab<ab1) {
	HAL_GPIO_WritePin(GPIOA,SN1_Pin,GPIO_PIN_SET);	
  HAL_GPIO_WritePin(GPIOA,SN2_Pin,GPIO_PIN_SET);	
	}	
	if(ab>ab1&&ab<ab2) {
	HAL_GPIO_WritePin(GPIOA,SN1_Pin,GPIO_PIN_RESET);	
  HAL_GPIO_WritePin(GPIOA,SN2_Pin,GPIO_PIN_RESET);	
	}	
	if(ab>ab2) ab=0;		
			
	}
// *********************** SANIYE LEDLERI SON *************************************	

} // TIMER KESME SONU

void KAPA(void){

	if(saat==18&&dakika==0&&saniye<=1) {
	HAL_GPIO_WritePin(GPIOA,a_Pin,GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOA,b_Pin,GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOA,c_Pin,GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOA,d_Pin,GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOA,e_Pin,GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOA,e_Pin,GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOA,f_Pin,GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOA,g_Pin,GPIO_PIN_RESET);				
	HAL_GPIO_WritePin(GPIOA,SN1_Pin,GPIO_PIN_RESET);	
  HAL_GPIO_WritePin(GPIOA,SN2_Pin,GPIO_PIN_RESET);	
	HAL_TIM_Base_Stop_IT(&htim1);
  HAL_TIM_Base_Stop_IT(&htim2);
	ab=0;ab1=0;ab2=0;	
		
	}
	
	if(saat==8&&dakika==0&&saniye<=1) {
	ab=0;ab1=1000;ab2=2000;
	HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start_IT(&htim2);
		
	}
}



void SAATAYAR(void){
	
		s1=saat/10; s2=saat%10; s3=dakika/10; s4=dakika%10; 
		ab1=50;ab2=100;

	
AYARBAS:	
		
		while (deger==1) {
		binler=11;HAL_Delay(100);	
	  if(HAL_GPIO_ReadPin(GPIOB,ARTIR_Pin)==1) {HAL_Delay(50);s1++;if(s1>=3)s1=2; }
		if(HAL_GPIO_ReadPin(GPIOB,AZALT_Pin)==1) {HAL_Delay(50);s1--;if(s1==255)s1=0;}
		if(s2>3&&s1>1)s1=1;
		binler=s1;
   	while(HAL_GPIO_ReadPin(GPIOB,ARTIR_Pin)==1||HAL_GPIO_ReadPin(GPIOB,AZALT_Pin)==1);
    if(HAL_GPIO_ReadPin(GPIOB,MENU_Pin)==1){HAL_Delay(50);goto AYARSON;} 
		HAL_Delay(100);} 
		
		while (deger==2) {
		yuzler=11;HAL_Delay(100);	
		if(HAL_GPIO_ReadPin(GPIOB,ARTIR_Pin)==1) {HAL_Delay(50);s2++;if(s2>=10)s2=9; }
		if(HAL_GPIO_ReadPin(GPIOB,AZALT_Pin)==1) {HAL_Delay(50);s2--;if(s2==255)s2=0;}
		if(s1==2&&s2>3)s2=3;
		yuzler=s2;
		while(HAL_GPIO_ReadPin(GPIOB,ARTIR_Pin)==1||HAL_GPIO_ReadPin(GPIOB,AZALT_Pin)==1);
  	if(HAL_GPIO_ReadPin(GPIOB,MENU_Pin)==1){HAL_Delay(50);goto AYARSON;}
		HAL_Delay(100);} 
		
		while (deger==3) {
		onlar=11;HAL_Delay(100);	
		if(HAL_GPIO_ReadPin(GPIOB,ARTIR_Pin)==1) {HAL_Delay(50);s3++;if(s3>5)s3=5; }
		if(HAL_GPIO_ReadPin(GPIOB,AZALT_Pin)==1) {HAL_Delay(50);s3--;if(s3==255)s3=0;}
		if(s3>5)s3=5;
		onlar=s3;
		while(HAL_GPIO_ReadPin(GPIOB,ARTIR_Pin)==1||HAL_GPIO_ReadPin(GPIOB,AZALT_Pin)==1);
		if(HAL_GPIO_ReadPin(GPIOB,MENU_Pin)==1){HAL_Delay(50);goto AYARSON;}
		HAL_Delay(100);} 
		
		while (deger==4) {
		birler=11;HAL_Delay(100);	
		if(HAL_GPIO_ReadPin(GPIOB,ARTIR_Pin)==1) {HAL_Delay(50);s4++;if(s4>9)s4=9; }
		if(HAL_GPIO_ReadPin(GPIOB,AZALT_Pin)==1) {HAL_Delay(50);s4--;if(s4==255)s4=0;}
		if(s3==5&&s4>9)s4=9;
		birler=s4;
		while(HAL_GPIO_ReadPin(GPIOB,ARTIR_Pin)==1||HAL_GPIO_ReadPin(GPIOB,AZALT_Pin)==1);
		if(HAL_GPIO_ReadPin(GPIOB,MENU_Pin)==1){HAL_Delay(50);goto AYARSON;}
		HAL_Delay(100);} 

	  if(deger>4) {deger=0;goto AYARSON;}

AYARSON: 
  deger=0; saniye=0;
	dakika=s3*10+s4;
	saat=s1*10+s2;	
	setHour(saat);  setMinutes(dakika); setSeconds(saniye);
	DS3231_sendData(hi2c1, ds3231Addr);
  while (HAL_I2C_GetState( & hi2c1) != HAL_I2C_STATE_READY){}
  DS3231_setDate(hi2c1, ds3231Addr);
	HAL_GPIO_WritePin(GPIOA,SN1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,SN2_Pin,GPIO_PIN_RESET);	
	ab1=1000; ab2=2000;	
	while(HAL_GPIO_ReadPin(GPIOB,MENU_Pin)==1);			
	return;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start_IT(&htim2);
	
	ab1=1000;ab2=2000;
	
	deger=0;


	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

 //	while(menudeger==0) {		
	
	if(deger==1){HAL_Delay(300);SAATAYAR();}	
	
	DS3231_sendData(hi2c1, ds3231Addr);
	while (HAL_I2C_GetState( & hi2c1) != HAL_I2C_STATE_READY) {}
	I2C_ReadCalendarData(hi2c1, ds3231Addr); 
	saniye= atoi(readSeconds()); dakika = atoi(readMinutes());  saat = atoi(readHours());
 	
	birler=dakika%10; onlar=dakika/10; yuzler=saat%10; binler=saat/10;

  HAL_Delay(500);	
		
	KAPA();	
	
//}
	
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 32768;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 23999;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 2399;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 19;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, a_Pin|b_Pin|c_Pin|d_Pin
                          |e_Pin|f_Pin|g_Pin|SN1_Pin
                          |SN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, birler_Pin|onlar_Pin|yuzler_Pin|binler_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : a_Pin b_Pin c_Pin d_Pin
                           e_Pin f_Pin g_Pin SN1_Pin
                           SN2_Pin */
  GPIO_InitStruct.Pin = a_Pin|b_Pin|c_Pin|d_Pin
                          |e_Pin|f_Pin|g_Pin|SN1_Pin
                          |SN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : birler_Pin onlar_Pin yuzler_Pin binler_Pin */
  GPIO_InitStruct.Pin = birler_Pin|onlar_Pin|yuzler_Pin|binler_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : MENU_Pin AYAR_Pin */
  GPIO_InitStruct.Pin = MENU_Pin|AYAR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : ARTIR_Pin AZALT_Pin */
  GPIO_InitStruct.Pin = ARTIR_Pin|AZALT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
