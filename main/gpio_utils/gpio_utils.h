/*******************************************************************************
 * @file        gpio_utils.h
 * @author      Leonardo Acha Boiano
 * @date        01 Jul 2024
 * 
 * @note        This code is written in C and is used on an ESP32 38 PIN WROOM
 *
 *******************************************************************************/
#ifndef GPIO_UTILS_H_
#define GPIO_UTILS_H_

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../logging_utils/logging_utils.h"
#include "../servo_utils/servo_utils.h"

// Function declarations for the interrupt service routines
void servicio_isr(void* arg);

esp_err_t init_gpio(void);

/**
 * @brief Interrupt configuration structure used to initialize interrupts.
 */
typedef struct
{
    gpio_num_t gpioNum;                /*< GPIO number for the interrupt pin */
    gpio_isr_t isrHandler;             /*< Pointer to the interrupt service routine */
    void *userData;                    /*< User data to be passed to the ISR */
    gpio_int_type_t interruptType;     /*< Interrupt type (e.g., GPIO_INTR_ANYEDGE) */
} InterruptInitParams_t;

/**
 * @brief Initializes the interrupts.
 */
esp_err_t init_isr(void);

// Entradas digitales
#define GPIO_DOOR_LOCK_CLOSE GPIO_NUM_25 //FINAL DE CARRERA
#define GPIO_DOOR_LOCK_OPEN GPIO_NUM_26 //INICIO DE CARRERA
#define GPIO_BATTERY_LOCK_CLOSE GPIO_NUM_32 //FINAL DE CARRERA
#define GPIO_BATTERY_LOCK_OPEN GPIO_NUM_33 //INICIO DE CARRERA
#define GPIO_SERVICIO GPIO_NUM_27

#define GPIO_DOOR_LOCK_CLOSE_BIT_MASK (1ULL << GPIO_DOOR_LOCK_CLOSE)
#define GPIO_DOOR_LOCK_OPEN_BIT_MASK (1ULL << GPIO_DOOR_LOCK_OPEN)
#define GPIO_BATTERY_LOCK_CLOSE_BIT_MASK (1ULL << GPIO_BATTERY_LOCK_CLOSE)
#define GPIO_BATTERY_LOCK_OPEN_BIT_MASK (1ULL << GPIO_BATTERY_LOCK_OPEN)
#define SERVICIO_BIT_MASK (1ULL << GPIO_SERVICIO)

#endif /* GPIO_UTILS_H_ */

/********************************* END OF FILE ********************************/
/******************************************************************************/
