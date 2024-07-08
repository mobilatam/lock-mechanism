/*******************************************************************************
 * @file        gpio_utils.c
 * @author      Leonardo Acha Boiano
 * @date        01 Jul 2024
 * 
 * @note        This code is written in C and is used on an ESP32 38 PIN WROOM
 *
 *******************************************************************************/
#include "gpio_utils/gpio_utils.h"

gpio_config_t limitSwitch;
gpio_config_t pushButton;

esp_err_t init_gpio(void) {
   // Configure Digital Input pins

    limitSwitch.pin_bit_mask = (GPIO_DOOR_LOCK_CLOSE_BIT_MASK   | 
                                GPIO_DOOR_LOCK_OPEN_BIT_MASK    |
                                GPIO_BATTERY_LOCK_CLOSE_BIT_MASK|
                                GPIO_BATTERY_LOCK_OPEN_BIT_MASK);
    limitSwitch.pull_down_en = GPIO_PULLDOWN_DISABLE;
    limitSwitch.pull_up_en = GPIO_PULLUP_ENABLE;
    limitSwitch.mode = GPIO_MODE_INPUT;
    limitSwitch.intr_type = GPIO_INTR_DISABLE;

    pushButton.pin_bit_mask = SERVICIO_BIT_MASK;
    pushButton.pull_down_en = GPIO_PULLDOWN_DISABLE;
    pushButton.pull_up_en = GPIO_PULLUP_ENABLE;
    pushButton.mode = GPIO_MODE_INPUT;
    pushButton.intr_type = GPIO_INTR_POSEDGE;

    gpio_config(&limitSwitch);

    gpio_config(&pushButton);

    ESP_LOGI(GPIO_TAG, "GPIOs Initialized");

    return ESP_OK;
}

    
esp_err_t init_isr(void)
{
    // Define the interrupt initialization parameters
    InterruptInitParams_t const InterruptInitParameters[] = {
        {GPIO_SERVICIO, servicio_isr, NULL, GPIO_INTR_POSEDGE},
        // Add more interrupts here if needed
    };

    for (size_t i = 0; i < sizeof(InterruptInitParameters) / sizeof(InterruptInitParameters[0]); i++)
    {
        gpio_config_t gpioConfig;
        gpioConfig.pin_bit_mask = (1ULL << InterruptInitParameters[i].gpioNum);
        gpioConfig.mode = GPIO_MODE_DEF_INPUT;
        gpioConfig.pull_up_en = GPIO_PULLUP_ENABLE;
        gpioConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
        gpioConfig.intr_type = InterruptInitParameters[i].interruptType;

        esp_err_t err = gpio_config(&gpioConfig);

        //install gpio isr service
        gpio_install_isr_service(0);

        if (err != ESP_OK) {
            ESP_LOGE(INTERRUPT_TAG, "GPIO config failed for pin %d with error 0x%x", InterruptInitParameters[i].gpioNum, err);
            return err;
        }

        err = gpio_isr_handler_add(InterruptInitParameters[i].gpioNum, InterruptInitParameters[i].isrHandler, InterruptInitParameters[i].userData);
        if (err != ESP_OK) {
            ESP_LOGE(INTERRUPT_TAG, "GPIO ISR handler add failed for pin %d with error 0x%x", InterruptInitParameters[i].gpioNum, err);
            return err;
        }
    }

    ESP_LOGI(INTERRUPT_TAG, "Interrupts Initialized");

    return ESP_OK;
}

// ISR for GPIO_LDR pin
void servicio_isr(void* arg)
{
    if ((~gpio_get_level(GPIO_BATTERY_LOCK_CLOSE)&~gpio_get_level(GPIO_DOOR_LOCK_CLOSE))
        |(~gpio_get_level(GPIO_BATTERY_LOCK_OPEN)|~gpio_get_level(GPIO_DOOR_LOCK_OPEN)))
    {
        move_servo(DUTY_CYCLE_SERVO_DOOR_OPEN);//unlock_door();
        move_servo(DUTY_CYCLE_SERVO_BATTERY_OPEN);//unlock_battery();
    }
    else if((~gpio_get_level(GPIO_BATTERY_LOCK_OPEN)&~gpio_get_level(GPIO_DOOR_LOCK_OPEN))
         |(~gpio_get_level(GPIO_BATTERY_LOCK_CLOSE)|~gpio_get_level(GPIO_DOOR_LOCK_CLOSE))
    ){
        move_servo(DUTY_CYCLE_SERVO_DOOR_CLOSE);//lock_door();
        move_servo(DUTY_CYCLE_SERVO_BATTERY_CLOSE); //lock_battery();
    }
}

/********************************* END OF FILE ********************************/
/******************************************************************************/
