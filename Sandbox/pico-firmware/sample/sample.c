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
	stdio_init_all();
	sleep_ms(100);
    gpio_init(HEADLIGHT_PIN);
    gpio_set_dir(HEADLIGHT_PIN, GPIO_OUT);

    gpio_init(PIR_PIN);
    gpio_set_dir(PIR_PIN, GPIO_IN);

    gpio_init(HC_TRIG_PIN);
    gpio_init(HC_ECHO_PIN);
    gpio_set_dir(HC_TRIG_PIN, GPIO_OUT);
    gpio_set_dir(HC_ECHO_PIN, GPIO_IN);


    uint workflow = 2;
    while(true)
    {
        // Surveillance workflow
        if ( workflow == 1)
        {
            for(int i = 0; i < 10; i++){
                if (gpio_get(PIR_PIN)){
                    printf("Object detected! Turn on the Headlights...\n");
                    gpio_put(HEADLIGHT_PIN, 1);
                    sleep_ms(100);
                    gpio_put(HEADLIGHT_PIN, 0);
                    sleep_ms(100);
                } else {
                    printf("Factory remains still.\n");
                    sleep_ms(1000);
                }
            }
        }

        // Operation workflow
        if ( workflow == 2)
        {
            for(int i = 0; i < 10; i++){
                gpio_put(HC_TRIG_PIN, 1);
                sleep_us(1);
                gpio_put(HC_TRIG_PIN, 0);

                uint width = 0;
                while(gpio_get(HC_ECHO_PIN) == 0){ tight_loop_contents(); }
                while(gpio_get(HC_ECHO_PIN) == 1){
                    width++;
                    if (width > timeout) {
                        printf("Bad measurement!\n");
                        sleep_ms(100);
                    };
                }
                float dist_cm = (float)width / 29 / 2;
                printf("Distance in [CM]: %.3f\n", dist_cm);
                sleep_ms(200);
            }
        }

        // Request command from the host
        printf("[>>WKF<<]: work finished, what to do next?");
        int retHostCmd = getchar_timeout_us(2000000);
        if( (char)retHostCmd == '1' ){
            printf("Start workflow: Surveillance ...\n");
            workflow = 1;
            sleep_ms(100);
        }
        else if ( (char)retHostCmd == '2' ){
            printf("Start workflow: Operation ...\n");
            workflow = 2;
            sleep_ms(100);
        }
        sleep_ms(1000);
    }
    return 0;
}


