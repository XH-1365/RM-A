#ifndef UART_H
#define UART_H

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "main.h"


extern UART_HandleTypeDef huart2;

void Serial_Init(void);
//void Serialplot_Call_Back(void);


#endif //UART_H
