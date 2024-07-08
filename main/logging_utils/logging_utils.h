/*******************************************************************************
 * @file        logging_utils.h
 * @brief       Header file containing logging utility functions and definitions
 * @author      Leonardo Acha Boiano
 * @date        01 Jul 2024
 * 
 * @note        This code is written in C and is used on an ESP32 38 PIN WROOM
 *
 ******************************************************************************/

#ifndef LOGGING_UTILS_H
#define LOGGING_UTILS_H

#include <esp_log.h>
#include <stdarg.h>

/**
 * @brief Tag for TWAI (Two-Wire Automotive Interface)
 */
extern const char *TWAI_TAG;

/**
 * @brief Tag for Servo operations
 */
extern const char *SERVO_TAG;

/**
 * @brief Tag for Interrupt handling
 */
extern const char *INTERRUPT_TAG;

/**
 * @brief Tag for Main application logic
 */
extern const char *MAIN_TAG;

/**
 * @brief Tag for GPIO (General-Purpose Input/Output) operations
 */
extern const char *GPIO_TAG;

#endif  // LOGGING_UTILS_H

/********************************* END OF FILE ********************************/
/******************************************************************************/
