/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body (CAN 控制电机，去掉串口功能)
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "drv_bsp.h"
#include "drv_can.h"
#include "alg_pid.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
Class_PID pid_omega;

void SystemClock_Config(void);

int16_t Rx_Encoder, Rx_Omega, Rx_Torque, Rx_Temperature;
float Now_Omega, Target_Omega = 50.0f * PI;
uint32_t Counter = 0;
int32_t Output;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
/**
 * @brief CAN报文回调函数
 */
void CAN_Motor_Call_Back(Struct_CAN_Rx_Buffer *Rx_Buffer)
{
    uint8_t *Rx_Data = Rx_Buffer->Data;
    switch (Rx_Buffer->Header.StdId)
    {
			case (0x201):
			{
					Rx_Encoder = (Rx_Data[0] << 8) | Rx_Data[1];
					Rx_Omega   = (Rx_Data[2] << 8) | Rx_Data[3];
					Rx_Torque  = (Rx_Data[4] << 8) | Rx_Data[5];
					Rx_Temperature = Rx_Data[6];
			}
    break;
    }
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_CAN1_Init();
    MX_USART2_UART_Init();

    /* USER CODE BEGIN 2 */
    BSP_Init(BSP_DC24_LU_ON | BSP_DC24_LD_ON | BSP_DC24_RU_ON | BSP_DC24_RD_ON);
    CAN_Init(&hcan1, CAN_Motor_Call_Back);
    CAN_Filter_Mask_Config(&hcan1,
                           CAN_FILTER(0) | CAN_FIFO_1 | CAN_STDID | CAN_DATA_TYPE,
                           0, 0);

    pid_omega.Init(0.0f, 5.0f, 0.1f, 0.0f, 16384.0f, 16384.0f);
    /* USER CODE END 2 */

    /* Infinite loop */
    while (1)
    {
        Counter++;
        if(Counter >= 2000)
        {
            Counter = 0;
            if(Target_Omega == 50.0f * PI)
                Target_Omega = 100.0f * PI;
            else
                Target_Omega = 50.0f * PI;
        }
				
        // 电机反馈转速
        Now_Omega = Rx_Omega * 2.0f * PI / 60.0f;

        // PID 计算
        pid_omega.Set_Target(Target_Omega);
        pid_omega.Set_Now(Now_Omega);
        pid_omega.TIM_Adjust_PeriodElapsedCallback();
        Output = pid_omega.Get_Out();

				//Output = 1000;
        // CAN 发给电机
				CAN1_0x200_Tx_Data[0] = Output >> 8;
				CAN1_0x200_Tx_Data[1] = Output;
				CAN1_0x200_Tx_Data[2] = 0;
				CAN1_0x200_Tx_Data[3] = 0;
				CAN1_0x200_Tx_Data[4] = 0;
				CAN1_0x200_Tx_Data[5] = 0;
				CAN1_0x200_Tx_Data[6] = 0;
				CAN1_0x200_Tx_Data[7] = 0;
				CAN_Send_Data(&hcan1, 0x200, CAN1_0x200_Tx_Data, 8);


        HAL_Delay(1); // 建议保持 1ms 控制周期
    }
}


/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 6;
    RCC_OscInitStruct.PLL.PLLN = 180;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Activate the Over-Drive mode
     */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
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
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
