/**
 * LED strip functions
 * 
 * This file is not complete and is only a snippet
 * only functions for addressable LED strip are implemented
 * 
 * Chris Horne
 * 2025-03-19
 */
 
 #include <stdio.h>
 #include "freertos/FreeRTOS.h"
 #include "freertos/task.h"
 #include "driver/gpio.h"
 #include "esp_log.h"
 #include "led_strip.h"

 #include "addressable_leds.h"
 
static const char *TAG = "LED STRIP";

typedef struct {
    uint8_t port;
    uint8_t flashing;
    uint8_t on;
    uint8_t red;
    uint8_t blue;
    uint8_t green;
    uint8_t white;
} s_led_t;

static uint8_t s_led_state = 0;
static led_strip_handle_t led_strip;
static s_led_t led_config[LED_STRIP_LENGTH];

// Functions to allow external code to access the LEDs

/**
 * @brief Set the LED's to their initial state
 */
void initiate_LEDS(void)
{
    //Start with the LED Guage
    for (int i=0; i<LED_STRIP_LENGTH; i++) 
        set_led(i,LED_ON,LED_SOLID,0,0,0,0);
    
}

/**
 * @brief Set one LED to the required state
 * @param led_number integer value of the LED to be set
 * @param status LED_ON or LED_OFF
 * @param flashing LED_FLASH or LED_SOLID
 * @param red (0-255)
 * @param green (0-255)
 * @param blue (0-255)
 * @param white (0-255 for RGBW LED's)
 */
void set_led(uint8_t led, uint8_t status, uint8_t flashing, uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
{
    if (led < LED_STRIP_LENGTH){
        led_config[led].on=status;
        led_config[led].flashing=flashing;      
        led_config[led].red=red;
        led_config[led].green=green;
        led_config[led].blue=blue;
        led_config[led].white=white;
    } else {
        ESP_LOGE(TAG,"SET LED: Value out of bounds %d", led);
    }
}

/**
 * @brief Turn on one LED
 * @param led_number
 * @note LEDs are numbered from left to right, but wired right to left
 * @note so need to work backwards from the end of the strip
 */
void led_on(uint8_t led)
{
if (led < LED_STRIP_LENGTH){
        if (led < LED_STRIP_LENGTH) led=(LED_STRIP_LENGTH-1)-led;
        led_config[led].on=LED_ON;
    } else {
        ESP_LOGE(TAG,"LED ON: Value out of bounds %d", led);
    }
}

/**
 * @brief Turn off one LED
 * @param led_number
 * @note LEDs are numbered from left to right, but wired right to left
 * @note so need to work backwards from the end of the strip
 */
void led_off(uint8_t led)
{
if (led < LED_STRIP_LENGTH){
        if (led < LED_STRIP_LENGTH) led=(LED_STRIP_LENGTH-1)-led;
        led_config[led].on=LED_OFF;
    } else {
        ESP_LOGE(TAG,"LED OFF: Value out of bounds %d", led);
    }
}


/**
 * @brief Configure the GPIO's for the system
 */
void configure_IO(void)
{
//  Configure GPIO Outputs
//  Addressable LEDs
    ESP_LOGI(TAG, "Configure addressable LED!");
    initiate_LEDS();
    
    
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = STRIP_LED_GPIO,
        .max_leds = LED_STRIP_LENGTH, // at least one LED on board
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        //.led_pixel_format = LED_PIXEL_FORMAT_GRBW, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812, // LED strip model
        //.led_model = LED_MODEL_SK6812, // LED strip model        
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        //.mem_block_symbols = 128,
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    /* Set all LED off to clear all pixels */
    led_strip_clear(led_strip);
}

/**
 * @brief Control the LED strip, flashing the LED's as required
 * @note led.config[i] contains the status of each LED
 * @note Code is configutrd for RGB LED
 * @note Commented out lines are for RGBW LEDs
 */
void led_strip_update()
{
 
    for (int i=0 ; i<LED_STRIP_LENGTH; i++){
        int led_on = led_config[i].on;
        int led_flash = led_config[i].flashing;
        if (led_on && (!led_flash || (led_flash && s_led_state)))
        {
            //led_strip_set_pixel_rgbw(led_strip, i,led_config[i].green,led_config[i].red,led_config[i].blue,led_config[i].white);
            led_strip_set_pixel(led_strip, i,led_config[i].green,led_config[i].red,led_config[i].blue);
        } else {
            //led_strip_set_pixel_rgbw(led_strip, i, 0, 0, 0,0);
            led_strip_set_pixel(led_strip, i, 0, 0, 0);
        }
    }

    /* Refresh the strip to send data */
    led_strip_refresh(led_strip);
    
   
}