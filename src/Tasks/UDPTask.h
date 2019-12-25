#ifndef _UDP_TASK_H_
#define _UDP_TASK_H_

#include "Task.hpp"
#include "Queue.hpp"
#include "SACN/E131.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include <sys/param.h>
#include <string.h>
#include <FastLED-idf/FastLED.h>

//#define IP "192.168.2.20"
//#define GATEWAY "192.168.2.1"
//#define NETMASK "255.255.255.0"

#define UDP_PORT 5568

#define NUM_LEDS  (184)      
#define CHANNEL_COUNT (NUM_LEDS*3)  // (552 ch)
#define UNIVERSE_COUNT 2 
#define LEDS_PER_UNIVERSE (NUM_LEDS/UNIVERSE_COUNT) //92 per universe

//#define FASTLED_RMT_MAX_CHANNELS 1

#define DMX_UNIVERSE 1 //**Start** universe
// The pin the data line is connected to for WS2812b
#define DATA_PIN 13


class UDPTask : public RTOS::Task
{
private:
    RTOS::Queue<e131_packet_t *> *fila;
    CRGB leds[NUM_LEDS];
protected:
    void run(void *arg);
    void Show(e131_packet_t *packet);
public:
    UDPTask();
};

#endif // _UDP_TASK_H_