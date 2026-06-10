#pragma once
#include <Arduino.h>
#include <stdint.h>
#include <cstdint>

class AudioHandler {
public:
    AudioHandler(int bclk, int ws, int dout, int din);
    void begin();
    int readAudio(int16_t *buffer, size_t samplesCount);
    int playAudio(int16_t *buffer, size_t samplesCount);
    
private:
    int _bclk;
    int _ws;
    int _din;
    int _dout;
};