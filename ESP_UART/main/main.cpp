#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define UART_NUM UART_NUM_1
#define BUF_SIZE 2048
#define TXD_PIN (GPIO_NUM_1) 
#define RXD_PIN (GPIO_NUM_3)

#define LOG_LOCAL_LEVEL ESP_LOG_INFO

static const char *TAG = "UART_EEPROM";

void init_nvs() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void uart_init() {
    const uart_config_t uart_config = {
        .baud_rate = 2400,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void write_to_eeprom(uint8_t *data, size_t len) {
    nvs_handle_t my_handle;
    nvs_open("storage", NVS_READWRITE, &my_handle);
    nvs_set_blob(my_handle, "data", data, len);
    nvs_commit(my_handle);
    nvs_close(my_handle);
}

void read_from_eeprom(uint8_t *data) {
    nvs_handle_t my_handle;
    nvs_open("storage", NVS_READONLY, &my_handle);
    size_t required_size;
    nvs_get_blob(my_handle, "data", NULL, &required_size);
    nvs_get_blob(my_handle, "data", data, &required_size);
    nvs_close(my_handle);
}

void print_stored_data() {
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE + 1);
    read_from_eeprom(data);
    ESP_LOGI(TAG, "Stored Data: %s", data);
    free(data);
}

void print_task(void *arg) {
    while (1) {
        print_stored_data();
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void uart_task(void *arg) {
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE + 1);
    size_t total_bytes_received = 0;

    while (1) {
        int len = uart_read_bytes(UART_NUM, data + total_bytes_received, BUF_SIZE - total_bytes_received - 1, pdMS_TO_TICKS(1000));
        if (len > 0) {
            total_bytes_received += len;
            write_to_eeprom(data + total_bytes_received - len, len);
            uart_write_bytes(UART_NUM, data + total_bytes_received - len, len);

            unsigned long bits_sent = total_bytes_received * 8;
            unsigned long elapsed_time_ms = esp_timer_get_time() / 1000;

            if (elapsed_time_ms > 0) {
                float speed_bps = bits_sent / (elapsed_time_ms / 1000.0);
                ESP_LOGI(TAG, "Current Transmission Speed: %.2f bps", speed_bps);
            }
        }

        if (total_bytes_received >= 2500) {
            break;
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    free(data);
}

extern "C" void app_main() {
    init_nvs();
    uart_init();
    
    xTaskCreate(print_task, "print_task", 2048, NULL, 10, NULL);
    xTaskCreate(uart_task, "uart_task", 2048, NULL, 10, NULL);
}