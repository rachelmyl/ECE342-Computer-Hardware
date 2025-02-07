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
#include "main.h"

#include "dfr0151.h"
#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void get_eeprom(uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *week_day, uint8_t *day, uint8_t *month, uint8_t *year){
	*sec = eeprom_read(0x0);
	*min = eeprom_read(0x1);
	*hour = eeprom_read(0x2);
	*week_day = eeprom_read(0x3);
	*day = eeprom_read(0x4);
	*month = eeprom_read(0x5);
	*year = eeprom_read(0x6);
}

void get_eeprom_alarm(uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *week_day, uint8_t *day, uint8_t *month, uint8_t *year){
	*sec = eeprom_read(0x7);
	*min = eeprom_read(0x8);
	*hour = eeprom_read(0x9);
	*week_day = eeprom_read(0xA);
	*day = eeprom_read(0xB);
	*month = eeprom_read(0xC);
	*year = eeprom_read(0xD);
}

int main(void)
{
  /* Reset of all peripherals. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();  
  rtc_init();

  char message[100];

  // Your start up code here
	
	const char *weekday[7];
	weekday[0] = "Sunday";
	weekday[1] = "Monday";
	weekday[2] = "Tuesday";
	weekday[3] = "Wednesday";
	weekday[4] = "Thursday";
	weekday[5] = "Friday";
	weekday[6] = "Saturday";

	uint8_t sec = 0;
	uint8_t min = 0;
	uint8_t hr = 13;
	uint8_t wd = 2;
	uint8_t day = 1;
	uint8_t mo = 7;
	uint8_t yr = 25;
	
	uint8_t data[7];
	
	sprintf(message, "hello");
	print_msg(message);
	
	// Part 2: Set time to eeprom time
	/*
	get_eeprom(&hr, &min, &sec, &wd, &day, &mo, &yr);
	rtc_set_date(wd,day,mo,yr);
	rtc_set_time(hr,min,sec);
	*/
	
	bool alarm = false;
	uint8_t new_min = 1;
	print_msg(message);
	// Part 4.4: Reset after power off
	eeprom_write(0x0, &sec, 1);
	print_msg(message);
	eeprom_write(0x1, &min, 1);
	print_msg(message);
	eeprom_write(0x2, &hr, 1);
	print_msg(message);
	eeprom_write(0x3, &wd, 1);
	print_msg(message);
	eeprom_write(0x4, &day, 1);
	print_msg(message);
	eeprom_write(0x5, &mo, 1);
	print_msg(message);
	eeprom_write(0x6, &yr, 1);
	print_msg(message);
	
	hr = 13;
	
	eeprom_write(0x7, &sec, 1);
	print_msg(message);
	eeprom_write(0x8, &new_min, 1);
	print_msg(message);
	eeprom_write(0x9, &hr, 1);
	print_msg(message);
	eeprom_write(0xA, &wd, 1);
	print_msg(message);
	eeprom_write(0xB, &day, 1);
	print_msg(message);
	eeprom_write(0xC, &mo, 1);
	print_msg(message);
	eeprom_write(0xD, &yr, 1);
	print_msg(message);
	
	get_eeprom(&hr, &min, &sec, &wd, &day, &mo, &yr);
	print_msg(message);
	
	rtc_set_date(wd,day,mo,yr);
	print_msg(message);
	rtc_set_time(hr,min,sec);
	print_msg(message);
	
  while (1)
  {
    // Your code here
		
		// Part 1: Set time
		/*
		if (__HAL_GPIO_EXTI_GET_FLAG(USER_Btn_Pin)) {
			rtc_set_date(wd,day,mo,yr);
			rtc_set_time(hr,min,sec);
			HAL_GPIO_EXTI_IRQHandler(USER_Btn_Pin);
		}
		*/
		
		rtc_get_date(&data[3],&data[4],&data[5],&data[6]);
		rtc_get_time(&data[2],&data[1],&data[0]);
		
		sprintf(message, "Time: 20%u-%u-%u; %s; %u:%u:%u\n", 
						data[6], data[5], data[4], weekday[data[3]], data[2], data[1], data[0]);
		print_msg(message);
		
		
		// Part 2: Interact with EEPROM
		/*
		if (__HAL_GPIO_EXTI_GET_FLAG(USER_Btn_Pin)) {
			eeprom_write(0x0, data, 7);
			HAL_GPIO_EXTI_IRQHandler(USER_Btn_Pin);
		}
		*/
		
		// Part 3: Setting an Alarm
		/*
		if (__HAL_GPIO_EXTI_GET_FLAG(USER_Btn_Pin)) {
			/*
			if (data[1] != 59){
				data[1] += 1;
			} else{
				data[1] = 0;
				data[2] += 1;
			}
			*//*
			new_min = data[1] + 1; // Set timer to one minite later
			eeprom_write(0x7, data, 7);
			print_msg(message);
			eeprom_write(0x8, &new_min, 1);
			print_msg(message);
			alarm = true;
			print_msg(message);
			HAL_Delay(100);
			HAL_GPIO_EXTI_IRQHandler(USER_Btn_Pin);
		}
		// Setting alarm when button is pressed
		
		get_eeprom_alarm(&hr, &min, &sec, &wd, &day, &mo, &yr);
		sprintf(message, "Alarm: 20%u-%u-%u; %s; %u:%u:%u\n", 
						yr, mo, day, weekday[wd], hr, min, sec);
		print_msg(message);
		
		if (alarm && data[0] == sec && data[1] == min && data[2] == hr 
			&& data[3] == wd && data[4] == day && data[5] == mo && data[6] == yr){
				sprintf(message, "ALARM\n");
				print_msg(message);
				
				for (int i = 0; i < 9; i++){
					if (i % 2 == 0){
						HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET); 
						HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
						HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
						
						HAL_Delay(1000);  // Wait for 1 second (1000 ms)
						
						HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET); 
						HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); 
						HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET); 
					}
					else {
						HAL_Delay(1000);  // Wait for 1 second (1000 ms)
					}
				}
				// alarm = false;
		}
		// Keep checking if the time matches
		*/
		
		// Part 4.2: When button pressed
		//*
		if (__HAL_GPIO_EXTI_GET_FLAG(USER_Btn_Pin)) {
			hr = 13;
			rtc_set_date(wd,day,mo,yr);
			rtc_set_time(hr,min,sec);
			eeprom_write(0x9, &hr, 1);
			print_msg(message);
			HAL_GPIO_EXTI_IRQHandler(USER_Btn_Pin);
		}
		
		get_eeprom_alarm(&hr, &new_min, &sec, &wd, &day, &mo, &yr);
		sprintf(message, "Alarm: 20%u-%u-%u; %s; %u:%u:%u\n", 
						yr, mo, day, weekday[wd], hr, new_min, sec);
		print_msg(message);
		
		if (data[0] == sec && data[1] == new_min && data[2] == hr 
			&& data[3] == wd && data[4] == day && data[5] == mo && data[6] == yr){
				sprintf(message, "ALARM\n");
				print_msg(message);
				
				for (int i = 0; i < 9; i++){
					if (i % 2 == 0){
						HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET); 
						HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
						HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
						
						HAL_Delay(1000);  // Wait for 1 second (1000 ms)
						
						HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET); 
						HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); 
						HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET); 
					}
					else {
						HAL_Delay(1000);  // Wait for 1 second (1000 ms)
					}
				}
				// alarm = false;
		}
		// Keep checking if the time matches
		
		//*
		
		HAL_Delay(50);
  }
}