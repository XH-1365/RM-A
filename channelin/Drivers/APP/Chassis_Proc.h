#ifndef CHASSIS_PROC_H
#define CHASSIS_PROC_H

#include "stm32f4xx_hal.h"
#include "main.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

//--电机控制部分--can
void can_filter_init(void);
void Motor_init(void);

//--基本初始化
void BSP_Init(void);

#endif

