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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "ov7670.h"

/* USER CODE BEGIN PV */
#define PREAMBLE "\r\n!START!\r\n"
#define DELTA_PREAMBLE "\r\n!DELTA!\r\n"
#define SUFFIX "!END!\r\n"

uint16_t snapshot_buff[IMG_ROWS * IMG_COLS];
uint8_t old_snapshot_buff[IMG_ROWS * IMG_COLS];

uint8_t truncated_snapshot[IMG_ROWS * IMG_COLS /2];
uint8_t rle_snapshot[IMG_ROWS * IMG_COLS];

uint8_t tx_buff[sizeof(PREAMBLE) + 2 * IMG_ROWS * IMG_COLS + sizeof(SUFFIX)];
size_t tx_buff_len = 0;

// This is set in stm32f4xx_it.c in the DCMI_IRQHandler function.
uint8_t dma_flag = 0;

// Add function definitions for any other functions you add here.
void print_buf(void);

void print_test(void);
void print_img(void);
void truncate(uint8_t* snapshot, uint8_t* truncated, int size);
void print_truncated(void);
unsigned int rle(uint8_t* snapshot, uint8_t* result, int size);


int main(void)
{
  /* Reset of all peripherals */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_DCMI_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM6_Init();
  
  char msg[100];
  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  
  ov7670_init();
  HAL_Delay(100);
  ov7670_capture(snapshot_buff);
  
  // Add variables you need here.
	
	
  
  while (1)
  {
    
		/*
    if (HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin)) {
      HAL_Delay(100);  // debounce
			dma_flag = 0;
    }
		*/
		
		// part 1
		// print_test();
		
		// part 2
		//*
		ov7670_capture(snapshot_buff);
		HAL_DCMI_Stop(&hdcmi);
		print_img();
		/*/
		
		// part 3.1 data truncation
		/*
		ov7670_capture(snapshot_buff);
		HAL_DCMI_Stop(&hdcmi);
		print_truncated();
		*/
		
		// part 3.3 RLE	
		/*
    unsigned int size;

    while (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY) {}
		HAL_DCMI_Stop(&hdcmi);
    print_msg("\r\n!START!\r\n");

    uint8_t* ptr = (uint8_t*)snapshot_buff;
    for (int i = 0; i < (IMG_ROWS * IMG_COLS); i++) {
			old_snapshot_buff[i] = *(ptr + 2 * i + 1) & 0xF0;  // Keep only MSB
    }
		size = rle(old_snapshot_buff, rle_snapshot, IMG_ROWS * IMG_COLS + 1);

    ov7670_capture(snapshot_buff);
		while (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY) {}

    // Send the RLE encoded image
    uart_send_bin(rle_snapshot, size);  // ? Use `size` instead of `size+1`

    while (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY) {}
		print_msg("!END!\r\n");
		while (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY) {}

    HAL_Delay(50);
		*/
  }
}

// part 1
void print_test(){
	for (int i = 0; i < (IMG_ROWS*IMG_COLS); i++){
			snapshot_buff[i] = 0x00;
		}
		// print_msg("\r\nPREAMBLE!\r\n");
		print_msg("\r\n!START!\r\n");
		uart_send_bin(snapshot_buff, 25056);
		HAL_Delay(1000);
		print_msg("!END!\r\n");
}

// part 2
void print_img() {
	print_msg("\r\n!START!\r\n");
	uint8_t* ptr = (uint8_t*)snapshot_buff;
	/*
	for(int i=0; i<(IMG_ROWS*IMG_COLS); i++) {
		old_snapshot_buff[i] = *(ptr+(2*i+1));
	}
	*/
	
	//*
	for(int i=0; i<(IMG_ROWS*IMG_COLS); i++) {
		while (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY) {
			// Wait until UART is ready
		}
		// HAL_UART_Transmit_DMA(&huart3, ptr+(2*i+1), 1);
		uart_send_bin(ptr+(2*i+1), 1);
	}
	print_msg("!END!\r\n");
	/*/
	/*
	while (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY) {
			// Wait until UART is ready
  }
	uart_send_bin((uint8_t*) old_snapshot_buff, (IMG_ROWS*IMG_COLS));
	*/
}

// part 3.1
void truncate(uint8_t* snapshot, uint8_t* truncated, int size){
	// size contains how many 8 bits (bytes) are there in the buffer
	uint8_t first;
	uint8_t second;
	uint8_t new_byte;
	int j = 0;
	
	for (int i=0; i < size; i += 2){
		first = snapshot[i] & 0xF0;
		second = (snapshot[i+1] & 0xF0) >> 4;
		new_byte = first + second;
		truncated[j] = new_byte;	// truncated stores only 4 bits
		j++;
	}
	/*
	uint8_t new_byte;
	for (int i=0; i < size; i += 2){
		new_byte = snapshot[i] & 0xF0;
		truncated[i] = new_byte;	// truncated stores only 4 bits
	}
	*/
}

void print_truncated(){
	uint8_t* ptr = (uint8_t*)snapshot_buff;
	print_msg("\r\n!START!\r\n");
	for(int i=0; i<(IMG_ROWS*IMG_COLS); i++) {
		old_snapshot_buff[i] = *(ptr + 2*i + 1);
	}
	truncate(old_snapshot_buff, truncated_snapshot, IMG_ROWS * IMG_COLS);
	ptr = (uint8_t*) truncated_snapshot;

	while (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY) {
			// Wait until UART is ready
  }
	HAL_UART_Transmit_DMA(&huart3, truncated_snapshot, IMG_ROWS * IMG_COLS/2);
	while (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY) {
			// Wait until UART is ready
  }
	print_msg("!END!\r\n");
	while (HAL_UART_GetState(&huart3) != HAL_UART_STATE_READY) {
			// Wait until UART is ready
  }
	HAL_Delay(50);
}

// part 3.3
unsigned int rle(uint8_t* snapshot, uint8_t* result, int size){
	int count = 1;
	int j = 0;
	uint8_t prev = snapshot[0] & 0xF0;
	for(int i = 1; i < size; i++){
		uint8_t current = snapshot[i] & 0xF0;
		
		if(current == prev){
			count += 1;
		}
		else{
			uint8_t first = prev;
			uint8_t second = count & 0x0F;
			result[j] = first|second;
			j++;
			count = 1;
		}
		if(count > 15){
			uint8_t first = prev;
			uint8_t second = (count-1) & 0x0F;
			result[j] = first|second;
			j++;
			count = 1;
		}
		prev = current;
	}
	uint8_t first = snapshot[size-1] & 0xF0;
	uint8_t second = count & 0x0F;
	result[j] = first|second;
	
	return j;
}


void print_buf() {
  
  // Create a new buffer from the snapshot_buffer than the DCMI copied the 16-bit pixel values into.
  uint8_t *buffer = (uint8_t *) snapshot_buff;
  
  // Add the START preamble message to the start of the buffer for the serial-monitor program. 
  for (int i = 0; i < sizeof(PREAMBLE); i++) {
    tx_buff[i] = PREAMBLE[i];
  }
  
  // Write code to copy every other byte from the main frame buffer to 
  // our temporary buffer (this converts the image to grey scale)
  
  
  
	
  // Load the END suffix message to the end of the message.
  for (int i = 0; i < sizeof(SUFFIX); i++) {
    tx_buff[tx_buff_len++] = SUFFIX[i];
  }
  
  // Once the data is copied into the buffer, call the function to send it via UART. 
  uart_send_bin(tx_buff, sizeof(PREAMBLE) + IMG_COLS * IMG_ROWS);
}

