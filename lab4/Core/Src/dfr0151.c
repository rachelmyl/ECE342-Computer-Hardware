#include "dfr0151.h"
#include <stdio.h>
#include "config.h"

extern I2C_HandleTypeDef hi2c1;


uint8_t bcd2bin(uint8_t data){
 return ((data >> 4) * 10) + (data & 0x0F);
}

uint8_t bin2bcd(uint8_t data){
  return ((data / 10) << 4)|(data % 10);
}

uint8_t rtc_read(uint8_t address)
{
  uint8_t data;

  if (HAL_I2C_Mem_Read(&hi2c1,ADDR_DS1307,address,I2C_MEMADD_SIZE_8BIT,&data,1,100) != HAL_OK) {
    Error_Handler();
  }

  return data;
}

void rtc_write(uint8_t address,uint8_t data)
{
  if (HAL_I2C_Mem_Write(&hi2c1,ADDR_DS1307,address,I2C_MEMADD_SIZE_8BIT,&data,1,100) != HAL_OK){
    Error_Handler();
  }
}

void rtc_init()
{
  // Initialize Real-Time Clock peripheral
  uint8_t rs=0, sqwe=1, out=0;
  
  rs&=3;
  if (sqwe) rs|=0x10;
  if (out) rs|=0x80;

  rtc_write(0x07,rs);
}

void rtc_get_time(uint8_t *hour,uint8_t *min,uint8_t *sec)
{
  // Your code here
	uint8_t time[3];

  if (HAL_I2C_Mem_Read(&hi2c1,ADDR_DS1307, 0x00, I2C_MEMADD_SIZE_8BIT, time, 3, 100) != HAL_OK) {
    Error_Handler();
  }
	
	*hour = bcd2bin(time[2]);
	*min = bcd2bin(time[1]);
	*sec = bcd2bin(time[0]);
	
}

void rtc_set_time(uint8_t hour,uint8_t min,uint8_t sec)
{
  // Your code here
	uint8_t time[3];
	
	time[2] = bin2bcd(hour);
	time[1] = bin2bcd(min);
	time[0] = bin2bcd(sec);
	
	
	if (HAL_I2C_Mem_Write(&hi2c1,ADDR_DS1307, 0x00, I2C_MEMADD_SIZE_8BIT, time, 3, 100) != HAL_OK){ 
    Error_Handler();
  }
	// 0x00 represent where the time will be written, since seconds has memory address 0, we write data there
	
}

void rtc_get_date(uint8_t *week_day,uint8_t *day,uint8_t *month,uint8_t *year)
{
  // Your code here
		uint8_t date[4];

  if (HAL_I2C_Mem_Read(&hi2c1,ADDR_DS1307, 0x03, I2C_MEMADD_SIZE_8BIT, date, 4, 100) != HAL_OK) {
    Error_Handler();
  }
	
	*week_day = bcd2bin(date[0]);
	*day = bcd2bin(date[1]);
	*month = bcd2bin(date[2]);
	*year = bcd2bin(date[3]);
	
}

void rtc_set_date(uint8_t week_day,uint8_t day,uint8_t month,uint8_t year)
{
 // Your code here
	uint8_t date[4];
	
	date[0] = bin2bcd(week_day);
	date[1] = bin2bcd(day);
	date[2] = bin2bcd(month);
	date[3] = bin2bcd(year);
	
	if (HAL_I2C_Mem_Write(&hi2c1,ADDR_DS1307, 0x03, I2C_MEMADD_SIZE_8BIT, date, 4, 100) != HAL_OK){
    Error_Handler();
  }
	// 0x03 represent where the date will be written, since weekday has memory address 3, we write data there
	
}

void eeprom_write(uint16_t address, uint8_t *data, uint16_t size) {
  // Your code here
	HAL_I2C_Mem_Write(&hi2c1,ADDR_EEPROM,address,I2C_MEMADD_SIZE_16BIT,data,size,100);
    //char message[100];
		//sprintf(message, "EEPROM write error\n");
		//print_msg(message);
		//Error_Handler();
  
}

uint8_t eeprom_read(uint16_t address) {
  // Your code here
	uint8_t data;
	if (HAL_I2C_Mem_Read(&hi2c1,ADDR_EEPROM,address,I2C_MEMADD_SIZE_16BIT,&data,1,100) != HAL_OK) {
		Error_Handler();
  }
  return data;
}