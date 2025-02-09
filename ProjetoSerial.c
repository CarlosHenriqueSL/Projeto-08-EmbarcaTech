#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "numeros/numeros.h"
#include "hardware/clocks.h"
#include "ProjetoSerial.pio.h"

#define WS2812_PIN 7
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define LED_PIN_GREEN 11
#define LED_PIN_BLUE 12
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C
#define DEBOUNCE_DELAY_MS 500

volatile static uint32_t last_button_press = 0;
volatile static uint current_numero = 0;
volatile static bool green_updated = false; // Nova flag para LED verde
volatile static bool blue_updated = false;  // Nova flag para LED azul

double *numeros[10] = {numero0, numero1, numero2, numero3, numero4,
                       numero5, numero6, numero7, numero8, numero9};

void iniciar()
{
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_put(LED_PIN_GREEN, 0);

    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
    gpio_put(LED_PIN_BLUE, 0);

    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
}

uint32_t matrix_rgb(double r, double g, double b)
{
    unsigned char R = r * 255;
    unsigned char G = g * 255;
    unsigned char B = b * 255;
    // WS2812 espera os dados no formato GRB nos 24 bits mais significativos
    return (G << 24) | (R << 16) | (B << 8);
}

void desenho_pio(double *desenho, PIO pio, uint sm, double r, double g, double b)
{
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        uint32_t led_val = matrix_rgb(desenho[24 - i] * r, 0, 0);
        pio_sm_put_blocking(pio, sm, led_val);
    }
}

void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t now = to_ms_since_boot(get_absolute_time());

    if ((now - last_button_press) < DEBOUNCE_DELAY_MS) // Debounce
    {
        return;
    }

    last_button_press = now;

    if (gpio == BUTTON_A_PIN && (events & GPIO_IRQ_EDGE_FALL))
    {
        bool current_state = gpio_get(LED_PIN_GREEN);
        gpio_put(LED_PIN_GREEN, !current_state);
        green_updated = true;
    }
    else if (gpio == BUTTON_B_PIN && (events & GPIO_IRQ_EDGE_FALL))
    {
        bool current_state = gpio_get(LED_PIN_BLUE);
        gpio_put(LED_PIN_BLUE, !current_state);
        blue_updated = true; // Ativa flag para mensagem azul
    }
}

int main()
{
    stdio_init_all();
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_t ssd;
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &ProjetoSerial_program);
    uint sm = pio_claim_unused_sm(pio, true);
    ProjetoSerial_program_init(pio, sm, offset, WS2812_PIN);

    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    iniciar();

    bool cor = true;
    char c;

    // Desenha um retângulo no OLED (apenas para referência)
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);

    while (true)
    {
        // Verificação não bloqueante de entrada serial
        c = getchar_timeout_us(0);
        if (c != PICO_ERROR_TIMEOUT)
        {
            if (c >= '0' && c <= '9')
            {
                current_numero = c - '0';
                ssd1306_fill(&ssd, false); // Limpar o display
                ssd1306_draw_big_char(&ssd, c, 4);
                ssd1306_send_data(&ssd); // Atualiza o display
                desenho_pio(numeros[current_numero], pio, sm, 1.0, 0.0, 0.0);
            }
            else if (isupper(c))
            {
                ssd1306_fill(&ssd, false); // Limpar o display
                ssd1306_draw_big_char(&ssd, c, 4);
                ssd1306_send_data(&ssd); // Atualiza o display
                desenho_pio(ledsDesligados, pio, sm, 0.0, 0.0, 0.0);
            }
            else if (islower(c))
            {
                ssd1306_fill(&ssd, false); // Limpar o display
                ssd1306_draw_big_char(&ssd, c, 4);
                ssd1306_send_data(&ssd); // Atualiza o display
                desenho_pio(ledsDesligados, pio, sm, 0.0, 0.0, 0.0);
            }
        }

        if (green_updated)
        {
            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, "LED verde alternou", 1, 25);
            ssd1306_send_data(&ssd);
            green_updated = false;
        }
        else if (blue_updated)
        {
            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, "LED azul alternou", 4, 25);
            ssd1306_send_data(&ssd);
            blue_updated = false;
        }

        tight_loop_contents();

        // Atualização periódica do display
        cor = !cor;

        sleep_ms(100);
    }
    return 0;
}
