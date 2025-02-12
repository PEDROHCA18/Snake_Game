#include "snake.c"

#include "hardware/pwm.h"

#define BUTTON_A 5

int main() {
    srand(time(0));
    stdio_init_all();
    
    //inicia modulos
    adc_init();
    ws2812b_init();
    // Configura os pinos GPIO 26 e 27 para leitura analógica do ADC.
    adc_gpio_init(VRX_PIN); // Configura GP26 (ADC0) para o eixo X do joystick.
    adc_gpio_init(VRY_PIN); // Configura GP27 (ADC1) para o eixo Y do joystick.

    // Configura o pino do botão do joystick como entrada digital com pull-up interno.
    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN); // Habilita o pull-up interno para garantir leitura estável.

    //CONFIG BUTTON A
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);




    
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM); // Configura o pino para PWM
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    gpio_set_function(BUZZER_PIN2, GPIO_FUNC_PWM); // Configura o pino para PWM 2
    uint slice_num2 = pwm_gpio_to_slice_num(BUZZER_PIN2);
    pwm_set_enabled(slice_num2, true);
    setup_buzzer();
    play_note(BUZZER_PIN2,1,10);


    while (1) {

        seta();
        packman();
        
        do{
            sleep_ms(50);
        }while(gpio_get(BUTTON_A) != 0 );
        
        init();
        counting();
        verifica();
        show();
        set_matrix();
        sleep_ms(1000);
        while(vida){
            // Leitura do valor do ADC para VRX (Eixo X do joystick)
            adc_select_input(0); // Seleciona canal 0 (GP26 - VRX)
            uint16_t vrx_value = adc_read(); // Lê o valor do eixo X, de 0 a 4095.

            // Leitura do valor do ADC para VRY (Eixo Y do joystick)
            adc_select_input(1); // Seleciona canal 1 (GP27 - VRY)
            uint16_t vry_value = adc_read(); // Lê o valor do eixo Y, de 0 a 4095.

            // Leitura do estado do botão do joystick (SW)
            bool sw_value = gpio_get(SW_PIN) == 0; // 0 indica que o botão está pressionado.

            tick(vrx_value,vry_value);
            sleep_ms(400);
        }
        
    }
    // Retorna 0 indicando que o programa terminou com sucesso.
    
    return 0;
}
