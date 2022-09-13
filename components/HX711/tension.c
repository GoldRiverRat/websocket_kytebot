#include "tension.h"

#define PD_SCK_GPIO1 21
#define DOUT_GPIO1   22

extern int tension;

hx711_t dev1 = {
    .dout = DOUT_GPIO1,
    .pd_sck = PD_SCK_GPIO1,
    .gain = HX711_GAIN_A_64
};

void funcTension(void)
{
    // initialize device
    while (1)
    {
        esp_err_t r = hx711_init(&dev1);
        if (r == ESP_OK)
            break;

        printf("Could not initialize HX711: %d (%s)\n", r, esp_err_to_name(r));
        vTaskDelay(portTICK_PERIOD_MS);
    }

    while(1)
    {
        // read from device
        esp_err_t r = hx711_wait(&dev1, 500);
        if (r != ESP_OK)
        {
            printf("Device not found: %d (%s)\n", r, esp_err_to_name(r));
        }

        r = hx711_read_data(&dev1, &tension);

        printf("the power line reading = %d   ", tension);        

        if (r != ESP_OK)
        {
            printf("Could not read data: %d (%s)\n", r, esp_err_to_name(r));
        }
    }
}