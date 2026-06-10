#include "RadioManager.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <WiFi.h>
#include <esp_now.h>

QueueHandle_t RadioManager::_incomingQueue = NULL;

RadioManager::RadioManager() {}

void RadioManager::OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if (len != 120 * sizeof(int16_t)) {
        Serial.printf("Invalid packet length: %d\n", len);
        return;
    }
    
    if (incomingData != NULL) {
        xQueueSend(_incomingQueue, incomingData, 0);
    }
}

bool RadioManager::begin(QueueHandle_t audioQueue) {
    _incomingQueue = audioQueue;

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed!");
        return false;
    }

    esp_now_register_recv_cb(OnDataRecv);
    return true;
}

void RadioManager::addPeer(const uint8_t* peerMac) {
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, peerMac, 6);
    peerInfo.channel = 0;     
    peerInfo.encrypt = false; 

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer!");
    }
}

void RadioManager::sendAudio(const int16_t* buffer, size_t count) {
    esp_now_send(NULL, (const uint8_t*)buffer, count * sizeof(int16_t));
}