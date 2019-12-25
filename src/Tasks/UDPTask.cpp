#include "UDPTask.h"

UDPTask::UDPTask()
{
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(&(this->leds[0]), NUM_LEDS);
    LEDS.showColor(CRGB(255, 0, 0)); //turn all pixels on red
    vTaskDelay(2000);
    LEDS.showColor(CRGB(0, 255, 0)); //turn all pixels on green
    vTaskDelay(2000);
    LEDS.showColor(CRGB(0, 0, 255)); //turn all pixels on blue
    vTaskDelay(2000);
    LEDS.showColor(CRGB(255, 255, 255)); //turn all Branco
    vTaskDelay(2000);
    LEDS.showColor(CRGB(0, 0, 0)); //turn all pixels off
}

void UDPTask::run(void *arg)
{
    char addr_str[128];
    int addr_family;
    int ip_protocol;

    while (1)
    {
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(UDP_PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0)
        {
            printf("Unable to create socket: errno %d\n", errno);
            break;
        }

        int err = bind(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err < 0)
        {
            printf("Socket unable to bind: errno %d\n", errno);
        }
        e131_packet_t *packet = (e131_packet_t *)malloc(sizeof(e131_packet_t));
        while (1)
        {
            memset(packet, 0, sizeof(e131_packet_t));
            struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
            socklen_t socklen = sizeof(source_addr);
            int len = recvfrom(sock, packet->raw, sizeof(packet->raw) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

            // Error occurred during receiving
            if (len < 0)
            {
                printf("recvfrom failed: errno %d\n", errno);
                break;
            }
            // Data received
            else
            {
                Show(packet);
            }
            //printf("Pointer Value %p\n",packet);

            //closesocket(sock);
            //break;
        }

        if (sock != -1)
        {
            printf("Shutting down socket and restarting...\n");
            shutdown(sock, 0);
            close(sock);
        }
    }
}

void UDPTask::Show(e131_packet_t *packet)
{

    short b = ntohs(packet->frame.universe);
    if (b >= DMX_UNIVERSE && b <= DMX_UNIVERSE + UNIVERSE_COUNT)
    {
        if (packet->dmp.prop_val[0] == 0)
        { //start code must be 0
            int ledNumber = (b - DMX_UNIVERSE) * LEDS_PER_UNIVERSE;
            // sACN packets come in seperate RGB but we have to set each led's RGB value together
            // this 'reads ahead' for all 3 colours before moving to the next led.
            //Serial.println("*");
            for (int i = 1; i < ntohs(packet->dmp.prop_val_cnt) - 1; i = i + 3)
            {
                byte charValueR = packet->dmp.prop_val[i];
                byte charValueG = packet->dmp.prop_val[i + 1];
                byte charValueB = packet->dmp.prop_val[i + 2];
                leds[ledNumber].setRGB(charValueR, charValueG, charValueB);
                ledNumber++;
            }
            FastLED.show(); //Do it!
        }
    }
}