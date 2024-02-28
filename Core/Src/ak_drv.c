/*
 * ak_drv.c
 *
 *  Created on: 24 нояб. 2023 г.
 *      Author: NakedSnake
 */
#include "ak_drv.h"

#include <cmsis_gcc.h>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#include "inttypes.h"
#include "stdbool.h"

extern TxMailbox;
extern huart2;
extern hcan1;


void comm_can_transmit_eid(uint32_t id, const uint8_t *data, uint8_t len) {

	uint8_t i=0;

	if (len > 8) {
			len = 8;
	}

	CAN_TxHeaderTypeDef TxMessage;
	TxMessage.StdId = 0;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.ExtId = id;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = len;
	//memcpy(txmsg.data8, data, len);
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1) == 0);
	HAL_CAN_AddTxMessage(&hcan1, &TxMessage, data, &TxMailbox);

}

void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index) {
//	int32_t temp = 0;
//	temp = number >> 24;
//	buffer[(*index)++] = (temp & 0xF0) >> 4 | (temp & 0x0F) << 4;
//	temp = number >> 16;
//	buffer[(*index)++] = (temp & 0xF0) >> 4 | (temp & 0x0F) << 4;
//	temp = number >> 8;
//	buffer[(*index)++] = (temp & 0xF0) >> 4 | (temp & 0x0F) << 4;
//	temp = number;
//	buffer[(*index)++] = (temp & 0xF0) >> 4 | (temp & 0x0F) << 4;
		buffer[(*index)++] = number >> 24;

		buffer[(*index)++] = number >> 16;

		buffer[(*index)++] = number >> 8;

		buffer[(*index)++] = number;
}

void buffer_append_int16(uint8_t* buffer, int16_t number, int16_t *index) {
//	int32_t temp = 0;
//	temp = number >> 8;
//	buffer[(*index)++] = (temp & 0xF0) >> 4 | (temp & 0x0F) << 4;
//	temp = number;
//	buffer[(*index)++] = (temp & 0xF0) >> 4 | (temp & 0x0F) << 4;
	buffer[(*index)++] = number >> 8;
	buffer[(*index)++] = number;

}

void comm_can_set_pos(uint8_t controller_id, float pos) {

	int32_t send_index = 0;
	uint8_t buffer[4];

	buffer_append_int32(buffer, (int32_t)(pos * 10000), &send_index);
	comm_can_transmit_eid(controller_id | ((uint32_t)CAN_PACKET_SET_POS << 8), buffer, send_index);
}

void comm_can_set_pos_spd(uint8_t controller_id, float pos,int16_t spd, int16_t RPA ) {

	int32_t send_index = 0;
	int16_t send_index1 = 4;

//	int cnt = 0;

	uint8_t buffer[8] = {0};

	buffer_append_int32(buffer, (int32_t)(pos * 10000), &send_index);
	buffer_append_int16(buffer,(int16_t)spd/10.0, &send_index1);
	buffer_append_int16(buffer,(int16_t) RPA/10.0, &send_index1);
//	for(int i = 0, j = 7; i < 8/2; i++, j--)
//	    {
//	        cnt = buffer[i];
//	        buffer[i] = buffer[j];
//	        buffer[j] = cnt;
//	    }
	comm_can_transmit_eid(controller_id | ((uint32_t)CAN_PACKET_SET_POS_SPD << 8), buffer, 8);
}

void comm_can_set_origin(uint8_t controller_id, uint8_t set_origin_mode) {
	int32_t send_index = 0;
	uint8_t buffer;

	buffer = set_origin_mode;

	comm_can_transmit_eid(controller_id |((uint32_t) CAN_PACKET_SET_ORIGIN_HERE << 8), &buffer, send_index);
}

void comm_can_set_rpm(uint8_t controller_id, float rpm) {
	int32_t send_index = 0;
	uint8_t buffer[4];

	buffer_append_int32(buffer, (int32_t)rpm, &send_index);

	comm_can_transmit_eid(controller_id | ((uint32_t)CAN_PACKET_SET_RPM << 8), buffer, send_index);
}

void comm_can_set_cb(uint8_t controller_id, float current) {
	int32_t send_index = 0;
	uint8_t buffer[4];
	buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
	comm_can_transmit_eid(controller_id | ((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE << 8), buffer, send_index);
}

void comm_can_set_current(uint8_t controller_id, float current) {
	int32_t send_index = 0;
	uint8_t buffer[4];
	buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
	comm_can_transmit_eid(controller_id |
	((uint32_t)CAN_PACKET_SET_CURRENT << 8), buffer, send_index);
}

void comm_can_set_duty(uint8_t controller_id, float duty) {
	int32_t send_index = 0;
	uint8_t buffer[4];
	buffer_append_int32(buffer, (int32_t)(duty * 100000.0), &send_index);
	comm_can_transmit_eid(controller_id |((uint32_t)CAN_PACKET_SET_DUTY << 8), buffer, send_index);
}

void motor_receive(MotorValues_t* motor, uint8_t dataReceived[8], uint32_t frameId)
{
	int16_t pos_int = (dataReceived[0] << 8) | dataReceived[1];
	int16_t spd_int = (dataReceived[2] << 8) | dataReceived[3];
	int16_t cur_int = (dataReceived[4] << 8) | dataReceived[5];
	motor->position= (float)( pos_int * 0.1f); // Motor Position
	motor->speed= (float)( spd_int * 10.0f);// Motor Speed
	motor->current= (float) ( cur_int * 0.01f);// Motor Current
	motor->temperature= dataReceived[6] ;// Motor Temperature
	motor->error= dataReceived[7] ;// Motor Error Code
	motor->id = frameId;
	motor->receiveId = motor->id + 0x2300;
}

void motorInit(MotorValues_t motor, uint8_t idEntered){
	motor.id = idEntered;
}

void motor_Calibration(MotorValues_t* motor, float* maxLeft, float* maxRight){

	float timestamp = 50;
	bool stopFlag = false;
	int incremPos = motor->position + 1;
	int previousPos = 0;

	while (stopFlag == false)
		  {
			comm_can_set_pos(motor->id, incremPos);
			HAL_Delay(timestamp);

			if(motor->position == previousPos){
				stopFlag = true;
				*maxLeft = motor->position;
				}

			previousPos = motor->position;

			incremPos += +1;
		  }

	stopFlag = false;
	incremPos = -1;

	while (stopFlag == false)
		  {
			comm_can_set_pos(motor->id, incremPos);
			HAL_Delay(timestamp);

			if(motor->position == previousPos){
				stopFlag = true;
				*maxRight = motor->position;
				}

			previousPos = motor->position;

			incremPos += -1;
		  }
}
