#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/gpio.h"

const uint PIR_PIN = 15;
const uint HEADLIGHT_PIN = 14;
const uint HC_TRIG_PIN = 21;
const uint HC_ECHO_PIN = 20;

int timeout = 26100;

int main()
{
    sleep_ms(2000);
	stdio_init_all();
	sleep_ms(1000);
    gpio_init(HEADLIGHT_PIN);
    gpio_set_dir(HEADLIGHT_PIN, GPIO_OUT);

    gpio_init(PIR_PIN);
    gpio_set_dir(PIR_PIN, GPIO_IN);

    gpio_init(HC_TRIG_PIN);
    gpio_init(HC_ECHO_PIN);
    gpio_set_dir(HC_TRIG_PIN, GPIO_OUT);
    gpio_set_dir(HC_ECHO_PIN, GPIO_IN);

    int workflow = 1;
    while(true)
    {
        // Request command from the host
        int retHostCmd = getchar_timeout_us(200);
        if (PICO_ERROR_TIMEOUT == retHostCmd){
            sleep_ms(100);
        }
        if( (char)retHostCmd == '1' ){
            workflow = 1;
        }
        if( (char)retHostCmd == '2' ){
            workflow = 2;
        }
        else if ( (char)retHostCmd == '3' ){
            workflow = 3;
        }

        // Standby
        if( workflow == 1){
            gpio_put(HEADLIGHT_PIN, 0);
            printf("[>>STB1<<]");
            sleep_ms(1000);
        }

        // Surveillance workflow
        if ( workflow == 2){
            if (gpio_get(PIR_PIN)){
                printf("[>>SWF1<<]");
                gpio_put(HEADLIGHT_PIN, 1);
            } else {
                printf("[>>SWF0<<]");
                sleep_ms(1000);
            }
        }

        // Operation workflow
        if ( workflow == 3){
            gpio_put(HEADLIGHT_PIN, 0);
            sleep_ms(10);
            gpio_put(HC_TRIG_PIN, 1);
            sleep_us(1);
            gpio_put(HC_TRIG_PIN, 0);

            uint width = 0;
            bool b_valid = true;
            while(gpio_get(HC_ECHO_PIN) == 0){ tight_loop_contents(); }
            while(gpio_get(HC_ECHO_PIN) == 1){
                width++;
                if (width > timeout) {
                    b_valid = false;
                    printf("[>>OWF0<<]");
                    break;
                }
            }
            if (b_valid){
                float dist_cm = (float)width / 29 / 2;
                printf("[>>OWF1<<](%.3f)\n", dist_cm);
            }
            sleep_ms(200);
        }
        
        sleep_ms(100);
    }
    return 0;
}

