// Osahon Ojo - 816005001
// ECNG 3006 Lab #1

/* gpio example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"

static const char *TAG = "main";

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO2: output
 * GPIO0: input, interrupt from rising edge and fallign edge
 *
 * Test:
 * Connect GPIO2 (output) with GPIO0 (input)
 * Generate pulses on GPIO2 that triggers interrupt on GPIO0
 *
 */

// 1ULL: ULL changes the 'literal' 1 to unsigned long long (64 bits)
// | - bitwise OR

#define GPIO_OUTPUT_IO_0     2
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<GPIO_OUTPUT_IO_0)
#define GPIO_INPUT_IO_0      0
#define GPIO_INPUT_PIN_SEL   (1ULL<<GPIO_INPUT_IO_0)

//semaphore variable
SemaphoreHandle_t xSemaphore = NULL;


//releases/frees semaphore
static void gpio_isr_handler(void *arg)
{
    xSemaphoreGiveFromISR(xSemaphore, pdFALSE);
    ESP_LOGI(TAG, "Released semaphore.\n");
}

//obtains/takes semaphore
static void gpio_task_example(void *arg)
{
    for (;;) {
        if (xSemaphoreTake(xSemaphore, (TickType_t) 10)) {
            ESP_LOGI(TAG, "GPIO[%d] intr, val: %d\n", GPIO_INPUT_IO_0, gpio_get_level(GPIO_INPUT_IO_0));
            ESP_LOGI(TAG, "Semaphore obtained!\n");
        }
    }
}

void app_main(void)
{
    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO2
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //interrupt of falling edge
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    //bit mask of the pins, use GPIO0 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //create binary semaphore
    xSemaphore = xSemaphoreCreateBinary();
    ESP_LOGI(TAG, "Semaphore created!\n");

    //start gpio task
    //xTaskCreate(task_function, task_name_for_logs, stack_depth_in_words, void* task_parameters, task_priority, task_name_as_return_value_of_xTaskCreate)
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(0);

    //hook isr handler for GPIO0 pin
    //assumed parameters: io_pin, handler_fxn, parameter_for_handler_fxn 
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void *) GPIO_INPUT_IO_0);

    int cnt = 0;

    while (1) {
        ESP_LOGI(TAG, "cnt: %d\n", cnt++);
        vTaskDelay(1000 / portTICK_RATE_MS);
        gpio_set_level(GPIO_OUTPUT_IO_0, cnt % 2);
    }

    vSemaphoreDelete(xSemaphore);
    ESP_LOGI(TAG, "Semaphore destroyed!\n");
}


