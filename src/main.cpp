#include <Arduino.h>
#include "AudioHandler.h"
#include "RadioManager.h"

static constexpr int I2S_BCLK = 26;
static constexpr int I2S_WS   = 25;
static constexpr int I2S_DOUT = 22;
static constexpr int I2S_DIN  = 35;
static constexpr int BUTTON   = 39; 

uint8_t peerAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

AudioHandler audio(I2S_BCLK, I2S_WS, I2S_DOUT, I2S_DIN);
RadioManager radio;
TaskHandle_t AudioTaskHandle = NULL;
QueueHandle_t audioQueue = NULL;

void AudioTask(void *pvParameters) {
    static constexpr size_t samplesCount = 120; 
    int16_t buffer[128] = {0}; 
  
    while(true) {
        bool state = digitalRead(BUTTON);

        if (state == LOW) { 
            int read = audio.readAudio(buffer, samplesCount);
            if (read == samplesCount) {
                radio.sendAudio(buffer, samplesCount);  
            } 
        } else {
            if (xQueueReceive(audioQueue, buffer, pdMS_TO_TICKS(10)) == pdPASS) {
                audio.playAudio(buffer, samplesCount);
            }
        }
    }
}

void setup() {
    Serial.begin(115200);
    audio.begin();
    pinMode(BUTTON, INPUT_PULLUP);

    audioQueue = xQueueCreate(10, 120 * sizeof(int16_t));

    if (radio.begin(audioQueue)) {
        radio.addPeer(peerAddress);
        Serial.println("Radio initialized successfully!");
    }
    
    xTaskCreatePinnedToCore(
        AudioTask,       
        "AudioTask",     
        10000,           
        NULL,            
        1,               
        &AudioTaskHandle,
        1                
    );
}

void loop() {
    // Empty loop, operations handled by RTOS tasks
}