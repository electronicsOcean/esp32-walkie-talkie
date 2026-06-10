#include "AudioHandler.h"
#include "driver/i2s.h"

AudioHandler::AudioHandler(int bclk, int ws, int dout, int din) : _bclk(bclk), _ws(ws), _dout(dout), _din(din) {}

void AudioHandler::begin() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
        .sample_rate = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 128,
        .use_apll = false,
    };
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    
    i2s_pin_config_t i2s_pins = {
        .bck_io_num = _bclk,
        .ws_io_num = _ws,
        .data_out_num = _dout,
        .data_in_num = _din
    };
    i2s_set_pin(I2S_NUM_0, &i2s_pins);
}

int AudioHandler::readAudio(int16_t *buffer, size_t samplesCount) {
    size_t bytesRead = 0;
    esp_err_t res = i2s_read(I2S_NUM_0, buffer, samplesCount * sizeof(int16_t), &bytesRead, portMAX_DELAY);
    return bytesRead / sizeof(int16_t);
}

int AudioHandler::playAudio(int16_t *buffer, size_t samplesCount) {
    size_t bytesWritten = 0;
    esp_err_t res = i2s_write(I2S_NUM_0, buffer, samplesCount * sizeof(int16_t), &bytesWritten, portMAX_DELAY);
    return bytesWritten / sizeof(int16_t);
}