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
    ESP_LOGW(TWAI_TAG, "CAN ID: 0x%lX", message.identifier);
    switch (message.identifier) {
        case (0x10 << 24) | (0x16 << 19) | (0x00 << 16) | (0x04 << 8) | 0x00:
            // Process the first byte of the message data to unlock/lock the battery
            if (message.data_length_code == 8) {
                uint8_t command = message.data[0];
                
                switch (command) {
                    case 0x00: // Unlock
                        ESP_LOGI(TWAI_TAG, "Received Unlock command");
                        //OPEN BL
                        if (~gpio_get_level(GPIO_BATTERY_LOCK_CLOSE)){
                            unlock_battery(); 
                            send_open_status(0x16);
                        } 
                        break;
                    case 0x01: // Lock
                        ESP_LOGI(TWAI_TAG, "Received Lock command");
                        //CLOSE BL
                        if (~gpio_get_level(GPIO_BATTERY_LOCK_OPEN)){
                            lock_battery(); 
                            send_closed_status(0x16);
                        }
                        break;
                    default:
                        ESP_LOGW(TWAI_TAG, "Unknown command received: 0x%X", command);
                        break;
                } 
            } else {
                ESP_LOGW(TWAI_TAG, "Unexpected data length: %d", message.data_length_code);
            }
        case (0x10 << 24) | (0x18 << 19) | (0x00 << 16) | (0x04 << 8) | 0x00: 
            // Process the first byte of the message data to unlock/lock the battery
            if (message.data_length_code == 8) {
                uint8_t command = message.data[0];
                
                switch (command) {
                    case 0x00: // Unlock
                        ESP_LOGI(TWAI_TAG, "Received Unlock command");
                        //OPEN DL
                        if (~gpio_get_level(GPIO_DOOR_LOCK_CLOSE)){
                            unlock_door();
                            send_open_status(0x18);
                        } 
                        break;
                    case 0x01: // Lock
                        ESP_LOGI(TWAI_TAG, "Received Lock command");
                        //CLOSE DL
                        if (~gpio_get_level(GPIO_DOOR_LOCK_OPEN)){
                            lock_door(); 
                            send_closed_status(0x18);
                        }
                        break;
                    default:
                        ESP_LOGW(TWAI_TAG, "Unknown command received: 0x%X", command);
                        break;
                }
            } else {
                ESP_LOGW(TWAI_TAG, "Unexpected data length: %d", message.data_length_code);
                }
        default:
            ESP_LOGW(TWAI_TAG, "Unknown command received: 0x%X", message.data[0]);
            break;
    }
}

// Función para enviar un mensaje TWAI (CAN)
void send_twai_message(uint32_t identifier, uint8_t status) {
    // Create a TWAI message
    twai_message_t message;
    message.identifier = identifier; // Message ID
    message.extd = 0; // Standard format (11-bit identifier)
    message.rtr = 0; // Data frame
    message.data_length_code = 8; // DLC - length of data
    message.data[0] = 0x01; // Firmware version MSB
    message.data[1] = 0x02; // Firmware version LSB
    message.data[2] = status; // Status code (locked/unlocked)
    // Reserve (remaining bytes)

    // Transmit the TWAI message
    if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
        ESP_LOGI(TWAI_TAG, "Message transmitted: Status: 0x%02X", status);
    } else {
        ESP_LOGE(TWAI_TAG, "Failed to transmit message");
    }
}

// Function to send 'open' status
void send_open_status(uint8_t src) {
    uint32_t can_id = (src << 24) | (0x10 << 19) | (0x2 << 16) | (0x04 << 8) | (0x01);
    send_twai_message(can_id, 0x01); // 0x01 represents 'unlocked'
}

// Function to send 'closed' status
void send_closed_status(uint8_t src) {
    uint32_t can_id = (src << 24) | (0x10 << 19) | (0x2 << 16) | (0x04 << 8) | (0x01);
    send_twai_message(can_id, 0x00); // 0x00 represents 'locked'
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
