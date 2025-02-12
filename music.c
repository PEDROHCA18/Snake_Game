#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Define o pino do buzzer
#define BUZZER_PIN 10
#define BUZZER_PIN2 21


#define BUFFER_SIZE 1024
uint16_t audio_buffer[BUFFER_SIZE];

// Tabela de frequências para notas musicais (em Hz)
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0




// Configura PWM no buzzer
void play_note(uint buzzer_pin, int frequency, int duration_ms) {
    if (frequency == 0) {
        gpio_put(buzzer_pin, 0); // Silenciar o buzzer
        sleep_ms(duration_ms);
        return;
    }

    uint slice_num = pwm_gpio_to_slice_num(buzzer_pin);
    pwm_set_clkdiv(slice_num, 4.0); // Define a divisão do clock PWM
    pwm_set_wrap(slice_num, 125000000 / (4 * frequency) - 1); // Ajusta o período
    pwm_set_enabled(slice_num, true); // Ativa o PWM

    sleep_ms(duration_ms); // Toca a nota pelo tempo desejado

    pwm_set_enabled(slice_num, false); // Desativa o PWM após a nota
}
//

void setup_buzzer() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);  // Define o pino como saída PWM
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN); // Obtém o slice do PWM
    gpio_set_function(BUZZER_PIN2, GPIO_FUNC_PWM);  // Define o pino como saída PWM
    uint slice_num2 = pwm_gpio_to_slice_num(BUZZER_PIN2); // Obtém o slice do PWM

    pwm_set_wrap(slice_num, 12500); // Define o período do PWM (frequência ajustável)
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(BUZZER_PIN), 6250); // 50% duty cycle
    pwm_set_chan_level(slice_num2, pwm_gpio_to_channel(BUZZER_PIN2), 6250); // 50% duty cycle
    pwm_set_enabled(slice_num, true); // Liga o PWM
}


// Toca uma pequena melodia
void play_melody() {
    int melody[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};
    int durations[] = {300, 300, 300, 300, 300, 300, 300, 500}; // Duração de cada nota

    for (int i = 0; i < 8; i++) {
        play_note(BUZZER_PIN, melody[i], durations[i]);
        sleep_ms(100); // Pequeno intervalo entre notas
    }
}


void packman(){
    // change this to make the song slower or faster
int tempo = 105;
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {
  // Pacman
  // Score available at https://musescore.com/user/85429/scores/107109
  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, //1
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,
  NOTE_B4, 16,  NOTE_B5, 16,  NOTE_FS5, 16,   NOTE_DS5, 16,  NOTE_B5, 32,  //2
  NOTE_FS5, -16, NOTE_DS5, 8,  NOTE_DS5, 32, NOTE_E5, 32,  NOTE_F5, 32,
  NOTE_F5, 32,  NOTE_FS5, 32,  NOTE_G5, 32,  NOTE_G5, 32, NOTE_GS5, 32,  NOTE_A5, 16, NOTE_B5, 8
};
// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;
// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;

for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    // we only play the note for 90% of the duration, leaving 10% as a pause
    play_note(BUZZER_PIN, melody[thisNote], noteDuration * 0.9);
  }
}

void play_explosion_sound() {
    for (int freq = 1500; freq > 100; freq -= 100) { // Decresce a frequência
        play_note(BUZZER_PIN,freq,10);
        sleep_ms(10); // Tempo entre variações de frequência
    }
}

void play_pacman_walk_sound() {
    for (int i = 0; i < 1; i++) {  // Toca 4 "waka-waka"
        // Primeira nota (Waka 1)
        play_note(BUZZER_PIN,NOTE_DS1,20);
        play_note(BUZZER_PIN,NOTE_AS1,20);
        sleep_ms(50);
        // Segunda nota (Waka 2)
        play_note(BUZZER_PIN,NOTE_C1,50);
        play_note(BUZZER_PIN,NOTE_G1,50);
        
    }
}
