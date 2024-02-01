#include "Chassis_Proc.h"

 uint8_t TX_data[8] = {0X03, 0X00,};             //创建用于存放所发送数据的数组
 void can_filter_init(void)
 {
     CAN_FilterTypeDef can_filter_st;

     can_filter_st.FilterActivation = ENABLE;
     can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
     can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
     can_filter_st.FilterIdHigh = 0x0000;
     can_filter_st.FilterIdLow = 0x0000;
     can_filter_st.FilterMaskIdHigh = 0x0000;
     can_filter_st.FilterMaskIdLow = 0x0000;
     can_filter_st.FilterBank = 0;
     can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
     HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);

     HAL_CAN_Start(&hcan1);
 }
 void Motor_init(void)
 {
   CAN_TxHeaderTypeDef Tx_Header;                  //定义Tx为can的配置
   uint32_t mailbox;                               //作为SendMessage的参数，返回数据存放的邮箱

   Tx_Header.StdId = 0x200;
   Tx_Header.ExtId = 0;
   Tx_Header.IDE = CAN_ID_STD;
   Tx_Header.RTR = CAN_RTR_DATA;
   Tx_Header.DLC = 8;
   Tx_Header.TransmitGlobalTime = DISABLE;

   HAL_CAN_AddTxMessage(&hcan1,&Tx_Header,TX_data,&mailbox);
 }
 void BSP_Init(void)
 {
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,1);
  
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_3, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, GPIO_PIN_SET);
 } //--以下是对24V的电源口进行高电平



