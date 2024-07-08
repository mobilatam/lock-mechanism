/*******************************************************************************
 * @file        twai_utils.h
 * @author      Leonardo Acha Boiano
 * @date        01 Jul 2024
 * 
 * @note        This code is written in C and is used on an ESP32 38 PIN WROOM
 *
 *******************************************************************************/
#ifndef TWAI_CONFIG_H_
#define TWAI_CONFIG_H_

#include "driver/twai.h"
#include "../logging_utils/logging_utils.h"
#include "../servo_utils/servo_utils.h"
#include "../gpio_utils/gpio_utils.h"

esp_err_t configure_twai(void);
void handle_twai_message(twai_message_t message);
void send_twai_message(uint8_t command);

//GPIOs conectados al transceptor CAM(TWAI)
#define CTX_IO GPIO_NUM_12
#define CRX_IO GPIO_NUM_14 

#endif /* TWAI_CONFIG_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/