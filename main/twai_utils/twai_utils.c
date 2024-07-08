/*******************************************************************************
 * @file        twai_utils.c
 * @brief       Utility functions for configuring and using the TWAI (CAN) 
 *              controller on an ESP32. It includes functions for configuring the TWAI controller,
 *              handling received TWAI messages, and sending TWAI messages.
 * @author      Leonardo Acha Boiano
 * @date        01 Jul 2024
 * 
 * @note        This code is written in C and is used on an ESP32 38 PIN WROOM.
 *              
 *******************************************************************************/
#include "../twai_utils/twai_utils.h"

// Configuración del TWAI (CAN)
esp_err_t configure_twai(void) {
    // Configura los parámetros generales del controlador TWAI
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CTX_IO, CRX_IO, TWAI_MODE_NORMAL);
    // Configura los parámetros de temporización para una velocidad de 500 Kbps
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    // Configura el filtro para aceptar todos los mensajes
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    // Instala el controlador TWAI
    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
        ESP_LOGE(TWAI_TAG, "Failed to install TWAI driver");
        return ESP_FAIL;
    }
    // Inicia el controlador TWAI
    if (twai_start() != ESP_OK) {
        ESP_LOGE(TWAI_TAG, "Failed to start TWAI driver");
        return ESP_FAIL;
    }
    return ESP_OK;
}

// Función para manejar los mensajes recibidos
void handle_twai_message(twai_message_t message) {
    // Verifica el comando recibido y mueve el servomotor en consecuencia
    switch (message.data[0]) {
        case 0x01: //OPEN BL
            if (~gpio_get_level(GPIO_BATTERY_LOCK_CLOSE))
            {
                unlock_battery(); 
            }
            break;
        case 0x02: //OPEN DL
            if (~gpio_get_level(GPIO_DOOR_LOCK_CLOSE))
            {
                unlock_door(); 
            }
            break;
        case 0x03: //CLOSE BL
            if (~gpio_get_level(GPIO_BATTERY_LOCK_OPEN))
            {
                lock_battery(); 
            }
            break;
        case 0x04: //CLOSE DL
            if (~gpio_get_level(GPIO_DOOR_LOCK_OPEN))
            {
                lock_door();
            }
            break;
        default:
            ESP_LOGW(TWAI_TAG, "Unknown command received: 0x%X", message.data[0]);
            break;
    }
}

// Función para enviar un mensaje TWAI (CAN)
void send_twai_message(uint8_t command) {
    // Crea un mensaje TWAI
    twai_message_t message;
    message.identifier = 0x101; // ID del mensaje
    message.extd = 0; // Usa formato de ID estándar
    message.data_length_code = 1; // Longitud de los datos del mensaje
    message.data[0] = command; // Dato del mensaje

    // Envía el mensaje TWAI
    if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        ESP_LOGI(TWAI_TAG, "Message transmitted: 0x%X", command);
    } else {
        ESP_LOGE(TWAI_TAG, "Failed to transmit message");
    }
}

/********************************* END OF FILE ********************************/
/******************************************************************************/