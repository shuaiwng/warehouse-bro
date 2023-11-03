#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/gpio.h"

const uint PIR_PIN = 15;
const uint HEADLIGHT_PIN = 14;
const uint HC_TRIG_PIN = 7;
const uint HC_ECHO_PIN = 8;

int timeout = 26100;

int main()
{

	stdio_init_all();
	sleep_ms(1000);
    const uint LED_PIN = HEADLIGHT_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(PIR_PIN);
    gpio_set_dir(PIR_PIN, GPIO_IN);

    gpio_init(HC_TRIG_PIN);
    gpio_init(HC_ECHO_PIN);
    gpio_set_dir(HC_TRIG_PIN, GPIO_OUT);
    gpio_set_dir(HC_ECHO_PIN, GPIO_IN);


    while(true){
        if (gpio_get(PIR_PIN)){
            printf("Object Detected!\n");
            gpio_put(LED_PIN, 1);
            sleep_ms(100);
            gpio_put(LED_PIN, 0);
            sleep_ms(100);
        }
        // HC measurement
        gpio_put(HC_TRIG_PIN, 1);
        sleep_us(1);
        gpio_put(HC_TRIG_PIN, 0);

        uint width = 0;
        while(gpio_get(HC_ECHO_PIN) == 0){ tight_loop_contents(); }
        while(gpio_get(HC_ECHO_PIN) == 1){
            width++;
            if (width > timeout) {
                printf("HC bad measurement! \n");
            };
        }
        float dist_cm = (float)width / 29 / 2;
        printf("Distance in [CM]: %.3f\n", dist_cm);
        sleep_ms(100);
    }
    return 0;
}


