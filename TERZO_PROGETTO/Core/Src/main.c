/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
 /* USER CODE END Header */

#include "main.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PV */

// Righe (OUTPUT)
#define R1_GPIO GPIOA
#define R1_Pin  GPIO_PIN_15

#define R2_GPIO GPIOB
#define R2_Pin  GPIO_PIN_5

#define R3_GPIO GPIOB
#define R3_Pin  GPIO_PIN_4

#define R4_GPIO GPIOB
#define R4_Pin  GPIO_PIN_10

// Colonne (INPUT)
#define C1_GPIO GPIOB
#define C1_Pin  GPIO_PIN_3

#define C2_GPIO GPIOA
#define C2_Pin  GPIO_PIN_10

#define C3_GPIO GPIOB
#define C3_Pin  GPIO_PIN_6

#define C4_GPIO GPIOB
#define C4_Pin  GPIO_PIN_7

// LED binari
#define LED0_GPIO GPIOA
#define LED0_Pin  GPIO_PIN_0

#define LED1_GPIO GPIOA
#define LED1_Pin  GPIO_PIN_1

#define LED2_GPIO GPIOA
#define LED2_Pin  GPIO_PIN_2

#define LED3_GPIO GPIOA
#define LED3_Pin  GPIO_PIN_3

// LED interno
#define LED_INT_GPIO GPIOA
#define LED_INT_Pin  GPIO_PIN_5

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
int readKeypad(void);
void displayBinary(int value);
/* USER CODE END PFP */

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  while (1)
  {
      int key = readKeypad();

      if (key != -1)
      {
          HAL_GPIO_WritePin(LED_INT_GPIO, LED_INT_Pin, GPIO_PIN_SET);
          displayBinary(key);
          HAL_Delay(200);
      }
      else
      {
          HAL_GPIO_WritePin(LED_INT_GPIO, LED_INT_Pin, GPIO_PIN_RESET);
      }
  }
}

/* USER CODE BEGIN 4 */

int readKeypad()
{
    GPIO_TypeDef* rowGPIO[4] = {R1_GPIO, R2_GPIO, R3_GPIO, R4_GPIO};
    uint16_t rowPin[4]       = {R1_Pin, R2_Pin, R3_Pin, R4_Pin};

    GPIO_TypeDef* colGPIO[4] = {C1_GPIO, C2_GPIO, C3_GPIO, C4_GPIO};
    uint16_t colPin[4]       = {C1_Pin, C2_Pin, C3_Pin, C4_Pin};

    // Porta tutte le righe LOW
    HAL_GPIO_WritePin(R1_GPIO, R1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R2_GPIO, R2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R3_GPIO, R3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R4_GPIO, R4_Pin, GPIO_PIN_RESET);

    for (int r = 0; r < 4; r++)
    {
        HAL_GPIO_WritePin(rowGPIO[r], rowPin[r], GPIO_PIN_SET);
        HAL_Delay(2);

        for (int c = 0; c < 4; c++)
        {
            if (HAL_GPIO_ReadPin(colGPIO[c], colPin[c]) == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(rowGPIO[r], rowPin[r], GPIO_PIN_RESET);
                return r * 4 + c;
            }
        }

        HAL_GPIO_WritePin(rowGPIO[r], rowPin[r], GPIO_PIN_RESET);
    }

    return -1;
}

void displayBinary(int value)
{
    HAL_GPIO_WritePin(LED0_GPIO, LED0_Pin, (value & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED1_GPIO, LED1_Pin, (value & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED2_GPIO, LED2_Pin, (value & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED3_GPIO, LED3_Pin, (value & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/* USER CODE END 4 */

/**
  * @brief System Clock Configuration for STM32C031
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // OUTPUT: R1–R4
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  GPIO_InitStruct.Pin = R1_Pin;
  HAL_GPIO_Init(R1_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = R2_Pin;
  HAL_GPIO_Init(R2_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = R3_Pin;
  HAL_GPIO_Init(R3_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = R4_Pin;
  HAL_GPIO_Init(R4_GPIO, &GPIO_InitStruct);

  // OUTPUT: LED binari + LED interno
  GPIO_InitStruct.Pin = LED0_Pin | LED1_Pin | LED2_Pin | LED3_Pin | LED_INT_Pin;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // INPUT: colonne con pull-down
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;

  GPIO_InitStruct.Pin = C1_Pin;
  HAL_GPIO_Init(C1_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = C2_Pin;
  HAL_GPIO_Init(C2_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = C3_Pin;
  HAL_GPIO_Init(C3_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = C4_Pin;
  HAL_GPIO_Init(C4_GPIO, &GPIO_InitStruct);
}
