#include "motor.h"

void PID_Motor()
{
    extern int move_to;
    extern int curr_pos;

    int direct = 0;
    int proportion;
    int old_proportion = 0;
    int integral;
    int old_integral = 0;
    int derivative;
    int Kp = 25;
    int Ki = 0;
    int Kd = 0;
    int pwm =0;

    // setup of LEDC
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HS_MODE,           // timer mode
        .timer_num = LEDC_HS_TIMER,           // timer index
        .clk_cfg = LEDC_AUTO_CLK,             // Auto select the source clock
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
         .channel = LEDC_HS_CH0_CHANNEL,
         .duty = 0,
         .gpio_num = MOTOR_PWM,
         .speed_mode = LEDC_HS_MODE,
         .hpoint = 0,
         .timer_sel = LEDC_HS_TIMER
    };
        ledc_channel_config(&ledc_channel);

        gpio_pad_select_gpio(MOTOR_DIR);
        gpio_set_direction(MOTOR_DIR, GPIO_MODE_OUTPUT);

    // continuious loop through the winch and update PWD and direction
    while (true)
    {
        proportion = curr_pos - move_to;
        direct = proportion / abs(proportion);
        integral = proportion + old_integral;
        derivative = proportion - old_proportion;

        pwm = Kp * proportion + Ki * integral - Kd * derivative;
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, abs(pwm));
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);

        if (direct > 0)  gpio_set_level(MOTOR_DIR, 1);
        else  gpio_set_level(MOTOR_DIR, 0);

        old_integral = integral;
        old_proportion = proportion;

        vTaskDelay(20 / portTICK_PERIOD_MS);

        //printf("M2 %i  CP %i  pwm %i  \n", move_to, curr_pos, pwm);
    }
}
