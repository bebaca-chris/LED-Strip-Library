/**
 * LED strip functions - Header file
 * 
 * Include this file in your main program
 * Call configure_IO() to set up the GPIO port for strip LED
 * Call initiate_LEDS() to set up the LED's
 * In main task call blink_led() at regular intervals to update/flashing the LED's
 * Indidivual LED's can be turned on or off using led_on(led_number) and led_off(led_number)
 * 
 * Chris Horne
 * 2025-03-19
 */

#define LED_STRIP_LENGTH 15 //The number of LEDs in the strip
#define STRIP_LED_GPIO 6	//The GPIO port used to send the data to the LED strip

#define LED_FLASH 1
#define LED_SOLID 0
#define LED_ON 1
#define LED_OFF 0

// Functions to initiate the LED strip. Include in code at start up
void initiate_LEDS(void);
void configure_IO(void);

// Functions to control the LED strip - call at least once per second from main task
// Call this function more often to flash LED's faster
void led_strip_update();

//Function to change the LED colour and fashing
void set_led(uint8_t led, uint8_t status, uint8_t flashing, uint8_t red, uint8_t green, uint8_t blue, uint8_t white);

//Functions to turn on and off individual LED's
void led_on(uint8_t led);
void led_off(uint8_t led);