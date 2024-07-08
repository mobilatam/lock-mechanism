/*******************************************************************************
 * @file        main.c
 * @author      Leonardo Acha Boiano
 * @date        01 Jul 2024
 * @brief       Main code CK1 lock and unlock mechanism
 * 
 * @note        This code is written in C and is used on an ESP32 38 PIN WROOM
 *
********************************************************************************/

#include "main.h"

twai_message_t message;

void app_main(void) {
    init_gpio();
    init_isr();
    // Configura el servomotor
    configure_servo();
    // Configura el controlador TWAI
    configure_twai();
    //Set initial state
    if ( //currentState=CLOSE_BL_OPEN_DL
        ~gpio_get_level(GPIO_BATTERY_LOCK_CLOSE)
        &gpio_get_level(GPIO_BATTERY_LOCK_OPEN)
        &gpio_get_level(GPIO_DOOR_LOCK_CLOSE)
        &~gpio_get_level(GPIO_DOOR_LOCK_OPEN)){
            lock_battery();
            unlock_door();
    }
    else if( //currentState=OPEN_BL_CLOSE_DL
        gpio_get_level(GPIO_BATTERY_LOCK_CLOSE)
        &~gpio_get_level(GPIO_BATTERY_LOCK_OPEN)
        &~gpio_get_level(GPIO_DOOR_LOCK_CLOSE)
        &gpio_get_level(GPIO_DOOR_LOCK_OPEN)){
            unlock_battery();
            lock_door();
    }
    else if ( //currentState=OPEN_BDL
        gpio_get_level(GPIO_BATTERY_LOCK_CLOSE)
        &~gpio_get_level(GPIO_BATTERY_LOCK_OPEN)
        &gpio_get_level(GPIO_DOOR_LOCK_CLOSE)
        &~gpio_get_level(GPIO_DOOR_LOCK_OPEN)){
            unlock_battery();
            unlock_door();
    }
    else if ( //currentState=CLOSE_BDL
        ~gpio_get_level(GPIO_BATTERY_LOCK_CLOSE)
        &gpio_get_level(GPIO_BATTERY_LOCK_OPEN)
        &~gpio_get_level(GPIO_DOOR_LOCK_CLOSE)
        &gpio_get_level(GPIO_DOOR_LOCK_OPEN)){ 
            lock_battery();
            lock_door();
    }

    ESP_LOGI(MAIN_TAG, "Inicializacion Completa");

    while (1) {
        //State machine logic
        // Intenta recibir un mensaje TWAI(CAN)
        if (twai_receive(&message, pdMS_TO_TICKS(10)) == ESP_OK) {
            handle_twai_message(message);
        }
    }
}

/********************************* END OF FILE ********************************/
/******************************************************************************/