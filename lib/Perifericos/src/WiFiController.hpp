/**
 * @file WiFiController.hpp
 * @author Taivison Martins Leite (taivison@grupocriar)
 * @brief Define a classe para Manipular o WiFi do ESP32
 * @version 0.1
 * @date 2019-10-29
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _WIFI_CONTROLLER_HPP_
#define _WIFI_CONTROLLER_HPP_

#include <string>
#include <stdio.h>
#include <string.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>
#include <lwip/err.h>
#include <lwip/sys.h>
#include "EventGroup.hpp"
#include <vector>
#include <nvs_flash.h>

namespace Perifericos
{
class WiFiController
{
private:
    static uint32_t const WIFI_CONNECTED_EVENT = BIT(0);
    static uint32_t const WIFI_STA_STARTED_EVENT = BIT(1);
    static uint32_t const WIFI_GOT_IP_EVENT = BIT(2);
    static uint32_t const WIFI_DISCONNECTED_EVENT = BIT(3);
    static uint32_t const WIFI_SCAN_COMPLETE_EVENT = BIT(4);
    static WiFiController* instance;
    static RTOS::EventGroup *event;
    WiFiController();
    static esp_err_t EventHandler(void *ctx, system_event_t *e);
public:
    uint8_t ConnectAP(const std::string &ssid, const std::string &password,uint8_t num_max_tentativas, wifi_mode_t mode = WIFI_MODE_STA);
    std::vector<wifi_ap_record_t> Scan(void);
    uint8_t StartAP(const std::string &ssid, const std::string &password, wifi_mode_t mode = WIFI_MODE_AP);
    static inline WiFiController* getInstance()
    {
        if(instance == nullptr)
            instance = new WiFiController();
        return instance;
    }
};
} // namespace Perifericos
#endif // _WIFI_CONTROLLER_HPP_