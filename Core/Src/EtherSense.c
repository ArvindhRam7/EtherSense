#include "EtherSense.h"
#include "main.h"
#include "wizchip_conf.h"
#include "socket.h"
#include "stm32f1xx_ll_usart.h"
#include "w5500_hal.h"
#include <stdio.h>
#include <string.h>
extern SPI_HandleTypeDef hspi1; // Ensure SPI1 is visible here

// TX/RX memory size per socket
uint8_t txsize[8] = {2,2,2,2,2,2,2,2};
uint8_t rxsize[8] = {2,2,2,2,2,2,2,2};

void StartEthernetTask(void *argument)
{
    printf("EthernetTask started\r\n");

    // W5500 reset
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
    osDelay(50);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
    osDelay(200);

    printf("Reset done\r\n");

    // Register SPI and CS functions
    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
    reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);
    printf("SPI + CS callbacks registered\r\n");

    // W5500 Init
    if (wizchip_init(txsize, rxsize) != 0) {
        printf("W5500 init failed\r\n");
        while(1);
    }
    printf("W5500 init OK\r\n");

    if (ctlwizchip(CW_INIT_WIZCHIP, (void*)txsize) == -1) {
        printf("ctlwizchip() failed\r\n");
        while(1);
    }
    printf("WIZCHIP configured\r\n");

    // Set static IP
    wiz_NetInfo netinfo = {
        .mac = {0x00, 0x08, 0xDC, 0x11, 0x22, 0x33},
        .ip =  {192, 168, 100, 77},
        .sn =  {255, 255, 255, 0},
        .gw =  {192, 168, 100, 1},
        .dhcp = NETINFO_STATIC
    };

    printf("Applying static IP config...\r\n");
    wizchip_setnetinfo(&netinfo);

    // Read back and print
    wiz_NetInfo current;
    wizchip_getnetinfo(&current);
    printf("Static IP set to %d.%d.%d.%d\r\n",
           current.ip[0], current.ip[1], current.ip[2], current.ip[3]);

    // Optional sanity: Read Mode Register
    printf("W5500 MR = 0x%02X\r\n", getMR());

    while (1) {
        osDelay(5000);
    }
}

void UdpSenderTask(void *argument)
{
	 osDelay(2000);
    printf("UDP Sender Task started\r\n");

    uint8_t socket_num = 0;
    uint16_t local_port = 4000;

    printf("Opening socket...\r\n");
    int8_t s = socket(socket_num, Sn_MR_UDP, local_port, 0);
    if (s != socket_num) {
        printf("UDP socket open failed: %d\r\n", s);
        while (1);
    }
    printf("UDP socket opened\r\n");

    uint8_t dest_ip[4] = {192, 168, 100, 10};
    uint16_t dest_port = 5000;
    char udp_msg[64];
    float temperature = 24.8;
    float humidity = 40.5;

    while (1) {
        printf("Sending UDP packet...\r\n");
        snprintf(udp_msg, sizeof(udp_msg), "TEMP:%.1f,HUM:%.1f", temperature, humidity);
        printf("Sending UDP packet: %s\r\n", udp_msg);
        sendto(socket_num, (uint8_t *)udp_msg, strlen(udp_msg), dest_ip, dest_port);

        printf("Sent UDP packet\r\n");
        osDelay(2000);
    }
}
