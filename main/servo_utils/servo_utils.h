/*******************************************************************************
 * @file        servo_utils.h
 * @author      Leonardo Acha Boiano
 * @date        01 Jul 2024
 * 
 * @note        This code is written in C and is used on an ESP32 38 PIN WROOM
 *
 *******************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "../logging_utils/logging_utils.h"
#include "../gpio_utils/gpio_utils.h"

#define MAX_ATTEMPTS 3

// GPIOs configurados como PWM
#define SERVO_BATTERY GPIO_NUM_23 //PWM
#define SERVO_DOOR GPIO_NUM_22 //PWM

// Valores de duty cycle para diferentes ángulos del servomotor
#define DUTY_CYCLE_SERVO_BATTERY_OPEN 1638
#define DUTY_CYCLE_SERVO_BATTERY_CLOSE 8192

#define DUTY_CYCLE_SERVO_DOOR_OPEN 1638
#define DUTY_CYCLE_SERVO_DOOR_CLOSE 8192

void configure_servo(void);
void move_servo(int duty);

void lock_battery(void);
void unlock_battery(void);

void lock_door(void);
void unlock_door(void);

/********************************* END OF FILE ********************************/
/******************************************************************************/
