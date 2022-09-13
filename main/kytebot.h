
#include <stdio.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_websocket_client.h"
#include "esp_event.h"

#include "encoder.h"
#include "motor.h"
#include "tension.h"


void get_encoder() {
    funcEncoder();
}
void drive_motor() {
    PID_Motor();
}
void get_tension() {
    funcTension();
}