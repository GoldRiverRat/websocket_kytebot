#include "encoder.h"

extern int curr_pos;
extern int set_to_zero;

void funcEncoder(void)
{
    rotary_encoder_config_t config = ROTARY_ENCODER_DEFAULT_CONFIG((rotary_encoder_dev_t) 0, 17, 5);
    rotary_encoder_t *encoder = NULL;
    ESP_ERROR_CHECK(rotary_encoder_new_ec11(&config, &encoder));
    ESP_ERROR_CHECK(encoder->set_glitch_filter(encoder, 1));
    ESP_ERROR_CHECK(encoder->start(encoder));

    while (1) {
          if(!set_to_zero) {
          // if(set_to_zero) {
            pcnt_counter_pause(0);
            pcnt_counter_clear(0);
            pcnt_counter_resume(0);
         };
        curr_pos = encoder->get_counter_value(encoder);
        // printf("set to zero = %i encoder value = %i  \n", set_to_zero, curr_pos );
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
