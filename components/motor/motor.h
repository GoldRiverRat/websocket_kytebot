#ifndef _MOTOR_H
#define _MOTOR_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/pcnt.h"

#define LEDC_HS_TIMER LEDC_TIMER_0
#define LEDC_HS_MODE LEDC_HIGH_SPEED_MODE
#define MOTOR_DIR (15)
#define MOTOR_PWM (02)
#define LEDC_HS_CH0_CHANNEL LEDC_CHANNEL_0

#define LEDC_DUTY_MAX (8200)

void PID_Motor(void);

#endif