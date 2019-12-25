#include "WiFiController.hpp"

namespace Perifericos
{

RTOS::EventGroup *WiFiController::event = nullptr;
WiFiController *WiFiController::instance = nullptr;

WiFiController::WiFiController()
{
    
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    if (event == nullptr)
        event = new RTOS::EventGroup();
    tcpip_adapter_init();
    
    ESP_ERROR_CHECK(esp_event_loop_init(EventHandler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
}

uint8_t WiFiController::ConnectAP(const std::string &ssid, const std::string &password, uint8_t num_max_tentativas, wifi_mode_t mode)
{
    wifi_config_t sta_config;
    memset(&sta_config, 0, sizeof(sta_config));
    memcpy(sta_config.sta.ssid, ssid.data(), ssid.size());
    memcpy(sta_config.sta.password, password.data(), password.size());
    ESP_ERROR_CHECK(esp_wifi_set_mode(mode));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_disconnect());
    uint8_t tentativas = 0;
    while (true)
    {
        EventBits_t e = event->WaitBits(WIFI_CONNECTED_EVENT | WIFI_STA_STARTED_EVENT | WIFI_DISCONNECTED_EVENT,
                                        pdTRUE, pdFALSE, portMAX_DELAY);
        if (e & WIFI_STA_STARTED_EVENT)
            ESP_ERROR_CHECK(esp_wifi_connect());
        if (e & WIFI_DISCONNECTED_EVENT)
        {
            if (tentativas < num_max_tentativas)
            {
                ESP_ERROR_CHECK(esp_wifi_connect());
                event->ClearBits(WIFI_CONNECTED_EVENT);
                tentativas++;
            }
            else
                return 0xFF;
        }
        if (e & WIFI_CONNECTED_EVENT)
            return 0x00;
    }
}

std::vector<wifi_ap_record_t> WiFiController::Scan()
{
    //ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    wifi_scan_config_t scan_config;
    memset(&scan_config, 0, sizeof(scan_config));
    scan_config.show_hidden = true;
    scan_config.scan_type = WIFI_SCAN_TYPE_ACTIVE;
    scan_config.scan_time.active.min = 1000;
    scan_config.scan_time.active.max = 1500;
    ESP_ERROR_CHECK(esp_wifi_start());
    EventBits_t e = event->WaitBits(WIFI_STA_STARTED_EVENT, pdTRUE, pdTRUE, portMAX_DELAY);
    if (e & WIFI_STA_STARTED_EVENT)
    {
        ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
        uint16_t num = 0;
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&num));
        std::vector<wifi_ap_record_t> buffer(num);
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&num, buffer.data()));
        return buffer;
    }
    else return {};
}

uint8_t WiFiController::StartAP(const std::string &ssid, const std::string &password, wifi_mode_t mode)
{
    wifi_config_t ap_config;
    memset(&ap_config,0,sizeof(ap_config));
    memcpy(ap_config.ap.ssid,ssid.data(),ssid.size());
    memcpy(ap_config.ap.password,password.data(),password.size());
    ap_config.ap.authmode =  WIFI_AUTH_WPA_WPA2_PSK;
    ap_config.ap.beacon_interval = 100;
    ap_config.ap.max_connection = 4;
    ESP_ERROR_CHECK(esp_wifi_set_mode(mode));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP,&ap_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    return 0;
}

esp_err_t WiFiController::EventHandler(void *ctx, system_event_t *e)
{
    switch (e->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        event->SetBits(WIFI_STA_STARTED_EVENT);
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        event->SetBits(WIFI_CONNECTED_EVENT);
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        event->SetBits(WIFI_CONNECTED_EVENT);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        event->SetBits(WIFI_DISCONNECTED_EVENT);
        break;
    default:
        break;
    }
    return ESP_OK;
}

} // namespace Perifericos
