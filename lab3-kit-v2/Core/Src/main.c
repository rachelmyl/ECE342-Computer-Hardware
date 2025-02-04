/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include <stdio.h>
#include <math.h>
#include "config.h"
#include "fixedpt.h"
#include "string.h"

#define PI 3.14159265359
float LUT[62];	// look up table that stores the value of sine

/**
  * @brief  The application entry point.
  * @retval int
  */
	
float square_wave(float x, int n) {		// n: number of terms
		float sum = 0;
		for (int i = 0; i < n; i++) {
			float sin_output = sin((2*i + 1) * x);
			sum += (float)sin_output * (1.0/(2*i+1.0));
		}
		return 0.5 + 2.0/PI * sum;
}

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  MX_USART3_Init();
	
	HAL_DAC_Init(&hdac);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	__HAL_TIM_SET_PRESCALER(&htim6, 10000);
	HAL_TIM_Base_Start(&htim6);
	
	char message[100];
	sprintf(message, "Printing test! ");
	HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);

	
	// Part 1: Timer
	HAL_TIM_Base_Start(&htim6);
	
	for (int i = 0; i <= 30; i++){
		HAL_Delay(50);
	}
	
	double c = __HAL_TIM_GET_COUNTER(&htim6)/1000;
	sprintf(message, "count = %f\n", c);
	HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);

	float count;
	count = 0.0;
	float value = 0;	// stores the value projected on to dac output
	int y = 0;
	//int x = 0;
	int initial_time = __HAL_TIM_GET_COUNTER(&htim6);
	int final_time;
	
  while (1)
  {
		// sprintf(message, "hi2");
		// HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
		// DAC example
		// HAL_DAC_Init(&hdac);
		// HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
		// HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 255);
		
		
		// Part 2.1: sin()
		///*
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
		double ratio = 4095/3.3;
		value = (sin(count)*1.65 + 1.65) * ratio;
		count += 0.05;
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
		// sprintf(message, "value = %f\n", value);
		// HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
		
		// measure time
		sprintf(message, "count=%f\r\n", count);
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 100);
		if (count > 6.28){
			final_time = __HAL_TIM_GET_COUNTER(&htim6);
			sprintf(message, "time=%d\r\n", final_time - initial_time);
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 100);
		}
		
		//*/
		
		// Part 2.2.1: sin in LUT float point
		/*
		value = sin(count)*1.65 + 1.65;	
		double ratio = 4095/3.3;		// digital output 4096-1; voltage max = 3.3; need to adjust value by multiplying ratio
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
		if (count <= 2*PI) {
			LUT[y] = value;
			y += 1;
			count += 0.1;
		}
		else{
			for(y = 0; y < 62; y++){
				value = LUT[y] * ratio;
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
			}
		}
		
		sprintf(message, "value = %f\n", value);
		HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
		*/
		
		// Part 2.2.2: sin in LUT fixed point
		/*
		double ratio = 4095/3.3;
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
		
		if (count > 628) {
				initial_time = __HAL_TIM_GET_COUNTER(&htim6);
				for(y = 0; y < 62; y++){
					uint16_t fixed_point;
					fixed_point = (uint16_t)floor(LUT[y] * ratio);
					HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, fixed_point);
				}
				
				final_time = __HAL_TIM_GET_COUNTER(&htim6);
				sprintf(message, "time=%d\r\n", final_time - initial_time);
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 100);
		}
		
		else if (count <= 628) {
			value = sin(count/100.0)*1.65 + 1.65;	
			LUT[y] = value;
			y += 1;
		}
		count += 10;
		sprintf(message, "value = %f\n", value);
		HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
		sprintf(message, "count = %d\n", count);
		HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
		*/
		
		//Part 3.1 generating square wave using sin()
		/*
		if (count > 6.28) {
			count = 0;
		}
		else {
			count += 0.01;
		}
		value = square_wave(count, 6);
		double ratio= 4095/5;
		uint16_t fixed_point;
		fixed_point = (uint16_t)floor(value * ratio);
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, fixed_point);
		sprintf(message, "value = %f\n", value);
		HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
		*/
		
		// Part 3.2.1 generating square wave using LUT float point
		/*
		value = square_wave(count, 6);	
		double ratio = 4095/5;		// digital output 4096-1; voltage max = 3.3; need to adjust value by multiplying ratio
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
		if (count <= 2*PI) {
			LUT[y] = value;
			y += 1;
			count += 0.1;
		}
		else {
			for(y = 0; y < 62; y++){
				value = LUT[y] * ratio;
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
			}
		}
		sprintf(message, "value = %f\n", value);
		HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
		*/
		
		// Part 3.2.2 generating square wave using LUT fixed point
		/*
		value = square_wave(count, 6);	
		double ratio = 4095/5;		// digital output 4096-1; voltage max = 3.3; need to adjust value by multiplying ratio
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
		if (count <= 6.28) {
			LUT[y] = value;
			y += 1;
		}
		else {
			sprintf(message, "hi");
			HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
			for(y = 0; y < 62; y++){
				uint16_t fixed_point;
				fixed_point = (uint16_t)floor(LUT[y] * ratio);
				HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, fixed_point);
			}
		}
		count += 0.1;
		sprintf(message, "value = %f\n", value);
		HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
		sprintf(message, "count = %f\n", count);
		HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
		*/
		
		
		// Part 4: Numerical representation
		/*
		fixedpt sin_values[62];
		int k=0;
		__HAL_TIM_SET_COUNTER(&htim6, 0);
		initial_time = __HAL_TIM_GET_COUNTER(&htim6);
		for(float rad = 0.00; rad < (2*3.14); rad+=0.01){
			//out = sin(rad);
			sin_values[k]=FXD_FROM_FLOAT(sin(rad));
			//test//
			sprintf(message, "Error: %f\n", sin_values[k]-sin(rad));
			HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, sin_values[k]);
			
			k++;
			
		}
		final_time = __HAL_TIM_GET_COUNTER(&htim6);
		
		float time_taken = (final_time - initial_time);
		sprintf(message, "\n\n\ntime taken: %f\n\n\n", time_taken );
		HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
		*/
	
	}
	sprintf(message, "hi");
	HAL_USART_Transmit(&husart3, (uint8_t *)message, strlen(message), 100);
}
