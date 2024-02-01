#include "Uart.h"

#define NUM 100

uint8_t Rxst1[10];
uint8_t Txst1[10];

//串口发送
void Serial_Init(void)
{
    printf("Welcome to the XH library ^v^\n");
}

void Serialplot_Call_Back(void)
{
    if (Txst1[0] == '0')
    {
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,1);

    }
    else if (Txst1[0] == '1')
    {
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,1);
    }
    else if (Txst1[0] == '2')
    {
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,0);
        HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,0);
    }
}

