/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
    U1_CB_Init();
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
    HAL_UART_Receive_IT(&huart1,U1_RX_Struct.RX_In->start,RX_MAX);
  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
uint8_t U1_TX_Buffer[100];
uint8_t U1_RX_Buffer[RX_BUFFER_SIZE];

U1_RX_CB U1_RX_Struct;

/**
 *
 * @param format
 * @param ...
 */
void U1_Printf(char *format, ...)
{
    va_list list_data;
    va_start(list_data, format);
    vsprintf((char *) U1_TX_Buffer, format, list_data);
    va_end(list_data);

    HAL_UART_Transmit(&huart1, U1_TX_Buffer, strlen((char *) U1_TX_Buffer), 0xff);
}

void U1_CB_Init()
{
    U1_RX_Struct.RX_Counter = 0;
    U1_RX_Struct.RX_In = U1_RX_Struct.SE_Buffer;
    U1_RX_Struct.RX_Out = U1_RX_Struct.SE_Buffer;
    U1_RX_Struct.RX_End = &U1_RX_Struct.SE_Buffer[9];
    U1_RX_Struct.RX_In->start = U1_RX_Buffer;
}


uint8_t UART_Receive(uint8_t *RecData)
{
    if (U1_RX_Struct.RX_Out != U1_RX_Struct.RX_In)//Indicate that the UART1 receive buffer has some data
    {
        //U1_Printf("Receive %d data\r\n", U1_RX_Struct.RX_Out->end - U1_RX_Struct.RX_Out->start + 1);
        *RecData = U1_RX_Struct.RX_Out->end - U1_RX_Struct.RX_Out->start;
        U1_RX_Struct.RX_Out++;//get the next data
        if (U1_RX_Struct.RX_Out == U1_RX_Struct.RX_End)
            U1_RX_Struct.RX_Out = &U1_RX_Struct.SE_Buffer[0];
        return (U1_RX_Struct.RX_Out->end - U1_RX_Struct.RX_Out->start + 1);
    }
}
/* USER CODE END 1 */
