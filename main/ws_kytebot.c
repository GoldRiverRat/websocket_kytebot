#include "kytebot.h"

int comm_in [2];
char str[100];

int winch_num = 0;      // right winch is 1, left winch is 0
int set_to_zero = 1;    // set the steering line to zero
int32_t move_to = 0;    // move to the current position
int32_t curr_pos = 0;   // current position
int32_t tension = 0;    // tension of the power line

#define NO_DATA_TIMEOUT_SEC 5

static const char *TAG = "WEBSOCKET";

static void websocket_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;
    switch (event_id) {
    case WEBSOCKET_EVENT_CONNECTED:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_CONNECTED");
        break;
    case WEBSOCKET_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_DISCONNECTED");
        break;
    case WEBSOCKET_EVENT_DATA:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_DATA");
        ESP_LOGI(TAG, "Received opcode=%d", data->op_code);
        if (data->op_code == 0x08 && data->data_len == 2) {
            ESP_LOGW(TAG, "Received closed message with code=%d", 256*data->data_ptr[0] + data->data_ptr[1]);
        } else {
            ESP_LOGW(TAG, "Received=%.*s", data->data_len, (char *)data->data_ptr);
        }

        sprintf (str, "%.*s\r\n", data->data_len, data->data_ptr);
        char* token = strtok(str, ","); 
        int i = 0;
        while (token != NULL) { 
            comm_in[i] = atoi(token);
            i++;
            token = strtok(NULL, ","); 
        } 
        move_to     = comm_in[0];
        set_to_zero = comm_in[1];

        ESP_LOGW(TAG, "move to=%d, zero=%d\r\n", move_to, set_to_zero);

        break;
    case WEBSOCKET_EVENT_ERROR:
        ESP_LOGI(TAG, "WEBSOCKET_EVENT_ERROR");
        break;
    }
}

static void websocket_app_start(void)
{
    esp_websocket_client_config_t websocket_cfg = {};

    websocket_cfg.uri = "ws://echo.websocket.events";

    ESP_LOGI(TAG, "Connecting to %s...", websocket_cfg.uri);

    esp_websocket_client_handle_t client = esp_websocket_client_init(&websocket_cfg);
    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocket_event_handler, (void *)client);
    esp_websocket_client_start(client);

    char data[32];
    while (1)
    {
        int len = sprintf(data, "%04d, %04d", curr_pos, tension);
        ESP_LOGI(TAG, "Sending %s", data);
        esp_websocket_client_send_text(client, data, len, portMAX_DELAY);
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("WEBSOCKET_CLIENT", ESP_LOG_DEBUG);
    esp_log_level_set("TRANSPORT_WS", ESP_LOG_DEBUG);
    esp_log_level_set("TRANS_TCP", ESP_LOG_DEBUG);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());


    xTaskCreatePinnedToCore(&get_encoder, "encoder", 2048, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(&drive_motor, "motor", 2048, NULL, 4, NULL, 0);
    xTaskCreatePinnedToCore(&get_tension, "tension", 2048, NULL, 3, NULL, 0);

    websocket_app_start();
}
