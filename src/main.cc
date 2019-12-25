#include <sdkconfig.h>
#include "SACN/E131.h"
#include "WiFiController.hpp"
#include "EventGroup.hpp"
#include "Queue.hpp"
#include "Tasks/UDPTask.h"

#define TENTATIVAS_MAX 30
#define SSID "NATAL2019"
#define PASSWORD "12345678"


extern "C"
{
    void app_main(void);
}

UDPTask udpTask;
/**
 * @brief Função da Tarefa Principal
 * 
 */
void app_main(void)
{
    printf("Iniciando...\n");
    Perifericos::WiFiController *wifiController = Perifericos::WiFiController::getInstance();
    wifiController->ConnectAP(SSID,PASSWORD,TENTATIVAS_MAX);
    
    udpTask.Start(NULL,"UDP TASK",4096,1,0);
}