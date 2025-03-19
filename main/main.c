#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "led_strip.h"
#include "sdkconfig.h"

#include "addressable_leds.h"

static const char *TAG = "IHC";
#define LED_BLINK_PERIOD 1000


static void main_task()
/* This task runs once a second
*/
{
    
    while(true)
    {
        static int i = 0;
        led_off(i);
        i++;
        if (i >= LED_STRIP_LENGTH) i = 0;
        led_on(i);
        led_strip_update();    
        ESP_LOGI(TAG, "LED %d on", i);  
        vTaskDelay(LED_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
    
}


void app_main(void) {
    // Create the default event loop needed by the main app

    configure_IO();
    initiate_LEDS();
    for (int i=0; i<LED_STRIP_LENGTH; i++) 
        set_led(i,LED_ON,LED_SOLID,255,0,0,0);

    printf("IO now active\n");

    xTaskCreate(&main_task, "Main Task", configMINIMAL_STACK_SIZE*10, NULL, 5, NULL);
    
    printf("Main app now running \n");
    

}