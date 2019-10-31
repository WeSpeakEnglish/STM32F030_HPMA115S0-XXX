#ifndef HPMA115S0_H
#define HPMA115S0_H
#include <stdint.h>

#define MAX_STRLEN  32

extern uint8_t LengthMessageReceive;
extern volatile uint8_t MessageReceived;
extern uint8_t received_string[MAX_STRLEN+5]; // zde se bude ukládat prijímaná zpráva
extern enum LengthMessage       {CMD_MSG_ACK_LEN = 2, MEASURE_RESPONSE_LEN = 8, MEASURE_RESPONSE_LONG_LEN = 32}LengthMSG;
extern enum TypeMessage         {START_MSG = 0, STOP_MSG, READ_MESURE_MSG, READ_LONG_MEASURE_MSG}TypeMSG;
extern uint8_t StartMeasureFlag;

void StartMeasureHPMA115(void);
void ReadMeasureHPMA115(void);
void ReadMeasureLongHPMA115(void);
void ProceedMSG(void);
#endif