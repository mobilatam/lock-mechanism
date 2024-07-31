/*******************************************************************************
 * @file        servo_utils.c
 * @brief       Utilities for configuring and controlling servomotors
 *              to manage battery and door locking mechanisms.          
 * @author      Leonardo Acha Boiano
 * @date        01 Jul 2024
 * 
 * @note        This code is written in C and is used on an ESP32 38 PIN WROOM
 *
 *******************************************************************************/
#include "../servo_utils/servo_utils.h"

// Configuración del servomotor
void configure_servo() {
    // Configura el canal del LEDC para controlar el servomotor
    // Servo1
    ledc_channel_config_t ledc_channel_servo_1 = {
        .channel    = LEDC_CHANNEL_0, // Canal del LEDC
        .duty       = 0, // Duty cycle inicial
        .gpio_num   = SERVO_BATTERY, // Pin GPIO conectado al servomotor
        .speed_mode = LEDC_HIGH_SPEED_MODE, // Modo de alta velocidad
        .timer_sel  = LEDC_TIMER_0 // Selección del temporizador
    };
    if (ledc_channel_config(&ledc_channel_servo_1) != ESP_OK) {
        ESP_LOGE(SERVO_TAG, "Error configuring channel 0: SERVO BATTERY");
    }

    //Servo2
    ledc_channel_config_t ledc_channel_servo_2 = {
        .channel    = LEDC_CHANNEL_1, // Canal del LEDC
        .duty       = 0, // Duty cycle inicial
        .gpio_num   = SERVO_DOOR, // Pin GPIO conectado al servomotor
        .speed_mode = LEDC_HIGH_SPEED_MODE, // Modo de alta velocidad
        .timer_sel  = LEDC_TIMER_0 // Selección del temporizador
    };
    if (ledc_channel_config(&ledc_channel_servo_2) != ESP_OK) {
        ESP_LOGE(SERVO_TAG, "Error configuring channel 1: SERVO DOOR");
    }

    // Configura el temporizador del LEDC para controlar el servomotor
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_16_BIT, // Resolución de 16 bits para el duty cycle
        .freq_hz = 50, // Frecuencia de 50 Hz
        .speed_mode = LEDC_HIGH_SPEED_MODE, // Modo de alta velocidad
        .timer_num = LEDC_TIMER_0 // Número del temporizador
    };
    if (ledc_timer_config(&ledc_timer) != ESP_OK) {
        ESP_LOGE(SERVO_TAG, "Error configuring timer");
    }
    ESP_LOGI(SERVO_TAG, "Servos Configurados");
}

// Función para mover el servomotor a un ángulo específico
void move_servo(int duty) {
    // Configura el duty cycle del LEDC para mover el servomotor
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
}

void lock_battery(void) {
    static uint8_t counter = 0;
    ESP_LOGE(SERVO_TAG, "Lock battery");

    move_servo(DUTY_CYCLE_SERVO_BATTERY_CLOSE);
    vTaskDelay(pdMS_TO_TICKS(1000));

    if (gpio_get_level(GPIO_BATTERY_LOCK_CLOSE)) {
        counter++;
        if (counter < MAX_ATTEMPTS) {
            unlock_battery();
            vTaskDelay(pdMS_TO_TICKS(1000));
            lock_battery();
        } else {
            counter = 0;
        }
    } else {
        counter = 0;
    }
}

void unlock_battery(void) {
    static uint8_t counter = 0;
    ESP_LOGE(SERVO_TAG, "Unlock battery");

    move_servo(DUTY_CYCLE_SERVO_BATTERY_OPEN);
    vTaskDelay(pdMS_TO_TICKS(1000));

    if (gpio_get_level(GPIO_BATTERY_LOCK_OPEN)) {
        counter++;
        if (counter < MAX_ATTEMPTS) {
            lock_battery();
            vTaskDelay(pdMS_TO_TICKS(1000));
            unlock_battery();
        } else {
            counter = 0;
        }
    } else {
        counter = 0;
    }
}

void lock_door(void) {
    static uint8_t counter = 0;
    ESP_LOGE(SERVO_TAG, "Lock door");

    move_servo(DUTY_CYCLE_SERVO_DOOR_CLOSE);
    vTaskDelay(pdMS_TO_TICKS(1000));

    if (gpio_get_level(GPIO_DOOR_LOCK_CLOSE)) {
        counter++;
        if (counter < MAX_ATTEMPTS) {
            unlock_door();
            vTaskDelay(pdMS_TO_TICKS(1000));
            lock_door();
        } else {
            counter = 0;
        }
    } else {
        counter = 0;
    }
}


void unlock_door(void) {
    static uint8_t counter = 0;
    ESP_LOGE(SERVO_TAG, "Unlock door");

    move_servo(DUTY_CYCLE_SERVO_DOOR_OPEN);
    vTaskDelay(pdMS_TO_TICKS(1000));

    if (gpio_get_level(GPIO_DOOR_LOCK_OPEN)) {
        counter++;
        if (counter < MAX_ATTEMPTS) {
            lock_door();
            vTaskDelay(pdMS_TO_TICKS(1000));
            unlock_door();
        } else {
            counter = 0;
        }
    } else {
        counter = 0;
    }
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
