#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

class RadioManager {
public:
    RadioManager();
    void addPeer(const uint8_t *mac);
    bool begin(QueueHandle_t queue);
    void sendAudio(const int16_t* buffer, size_t len);
    
private:
    static void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
    static QueueHandle_t _incomingQueue; 
};