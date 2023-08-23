#include "TCP_server.h"

#include <string.h>
#include "lwip/sockets.h"

#include <netinet/in.h>
#include "esp_log.h"

#define TAG "TCP Klient"
#define SERVER_IP "192.168.10.104"  // Adres IP serwera
#define SERVER_PORT 5000           // Port serwera

void tcp_client_task(void *pvParameter) {
    char rx_buffer[128];
    char tx_buffer[128];

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SERVER_PORT);

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) {
        ESP_LOGE(TAG, "Nie można utworzyć gniazda: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }

    int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in));
    if (err != 0) {
        ESP_LOGE(TAG, "Nie można nawiązać połączenia z serwerem: errno %d", errno);
        close(sock);
        vTaskDelete(NULL);
        return;
    }

    strcpy(tx_buffer, "Witaj z ESP32!");

    int bytes_to_send = strlen(tx_buffer);
    err = send(sock, tx_buffer, bytes_to_send, 0);

    if (err < 0) {
        ESP_LOGE(TAG, "Błąd podczas wysyłania: errno %d", errno);
    } else {
        ESP_LOGI(TAG, "Chcieliśmy wysłać %d bajtów, wysłaliśmy %d bajtów", bytes_to_send, err);
    }


    close(sock);
    vTaskDelete(NULL);
}
