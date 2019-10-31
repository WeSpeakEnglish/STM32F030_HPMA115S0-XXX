#include "HPMA115S0.h"
#include "stm32f0xx.h"
#include "usart.h"

uint8_t volatile  MessageReceived = 0;
uint8_t received_string[MAX_STRLEN+5] = {0};
uint8_t MSG_START[]       ={0x68, 0x01, 0x01, 0x96};
uint8_t MSG_STOP[]        ={0x68, 0x01, 0x02, 0x95};
uint8_t MSG_READ[]        ={0x68, 0x01, 0x04, 0x93};
uint8_t MSG_AUTO_STOP[]   ={0x68, 0x01, 0x20, 0x77};
uint8_t MSG_AUTO_START[]  ={0x68, 0x01, 0x40, 0x57};
uint8_t StartMeasureFlag  = 0;

uint8_t ReceivedShortData[8]     ={0};
uint8_t ReceivedLongData[32]     ={0};
uint8_t ReceivedACK[2]           ={2};

uint16_t PM2_5 = 0;
uint16_t PM10  = 0;

enum LengthMessage      LengthMSG       = CMD_MSG_ACK_LEN;
enum TypeMessage        TypeMSG         = START_MSG;


void usart1_puts(uint8_t *Buffer, uint8_t len){
  HAL_UART_Transmit(&huart1, Buffer, len, 1000);
}

void StartMeasureHPMA115(void){
   usart1_puts(MSG_START, sizeof(MSG_START));
   LengthMSG       = CMD_MSG_ACK_LEN;
   TypeMSG         = START_MSG;
}

void ReadMeasureHPMA115(void){
   usart1_puts(MSG_READ, sizeof(MSG_READ));
   LengthMSG       = MEASURE_RESPONSE_LEN;
   TypeMSG         = READ_MESURE_MSG;
   HAL_UART_Receive_IT(&huart1, received_string, LengthMSG);
}

void ReadMeasureLongHPMA115(void){
   usart1_puts(MSG_AUTO_START, sizeof(MSG_AUTO_START));
   LengthMSG       = MEASURE_RESPONSE_LONG_LEN;
   TypeMSG         = READ_LONG_MEASURE_MSG;
  HAL_UART_Receive_IT(&huart1, received_string, 2);
}

void ProceedMSG(void){
  switch(TypeMSG){
    case START_MSG: 
      for(int i = 0; i < LengthMSG; i++ )
      {  ReceivedACK[i] = received_string[i];}
      break;
    case READ_MESURE_MSG: 
      for(int i = 0; i < LengthMSG; i++ )
      {  ReceivedShortData[i] = received_string[i];}
      break;
    case READ_LONG_MEASURE_MSG: 
      for(int i = 0; i < MEASURE_RESPONSE_LONG_LEN; i++ )
      {  ReceivedLongData[i] = received_string[i];
      PM2_5 = ReceivedLongData[7] + 256* ReceivedLongData[6] ;
      PM10 = ReceivedLongData[9] + 256* ReceivedLongData[8] ;
      
      }
      break;
  }

}


//Interrupt callback routine
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)  
{
  if (huart->Instance == USART1)  //current UART
   { ProceedMSG();
     MessageReceived = 1;
   }
}