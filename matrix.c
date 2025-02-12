#include <stdio.h>             // Biblioteca padrão para entrada e saída, utilizada para printf.
#include "pico/stdlib.h"       // Biblioteca padrão para funções básicas do Pico, como GPIO e temporização.
#include "hardware/adc.h"      // Biblioteca para controle do ADC (Conversor Analógico-Digital).
#include <time.h>
#include "stdlib.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"  // Arquivo gerado pelo PIO

#define LED_PIN 7  // Pino de controle dos LEDs
#define NUM_LEDS 25  // Número total de LEDs na matriz

PIO pio = pio0;
uint sm;

const uint8_t MATRIX[5][5] =  {
    {24, 23, 22, 21, 20},
    {15, 16, 17, 18, 19},
    {14, 13, 12, 11, 10},
    {5, 6, 7, 8, 9},
    {4, 3, 2, 1, 0}
};


void ws2812b_init() {
    sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, LED_PIN, 800000, false);

}

void set_pixel_color(uint pixel, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = (r << 16) | (g << 8) | b;
    pio_sm_put_blocking(pio, sm, color << 8u);
}

void update_matrix() {
    for (int i = 0; i < NUM_LEDS; i++) {
        set_pixel_color(i, 255, 0, 0);  
    }
    sleep_ms(500);
}
void clear_matrix() {
    for (int i = 0; i < NUM_LEDS; i++) {
        set_pixel_color(i, 0, 0, 0);
    }
}


