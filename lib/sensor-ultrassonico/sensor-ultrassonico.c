#include "sensor-ultrassonico.h"

volatile uint16_t tick_subida = 0;
volatile uint16_t tick_descida = 0;
volatile uint16_t tick = 0;
volatile float t = 0; // tempo que o pulso ficou ativo em microssegundos
volatile float dist = 0; // distancia em cm

__ramfunc void tpm1_isr(void *arg) {
    TPM1->STATUS |= TPM_STATUS_CH0F_MASK; // zerra a flag que gerou a interrupção
    if ((PTB->PDIR & 1) == 1) {
        tick_subida = TPM1->CONTROLS[0].CnV;
        return;
    }
    else {
        tick_descida = TPM1->CONTROLS[0].CnV;
        if (tick_descida > tick_subida) {
            tick = tick_descida - tick_subida;
            t = tick / 3;
            dist = t * 0.017f;
        }
        else {
            tick = 65536 - (tick_subida - tick_descida);
            t = tick / 3;
            dist = t * 0.017f;
        }
    }
}

void sensorUltrassonicoInit(void) {
    // Configura a porta B0 como input
    const struct device *input_dev;
    input_dev = DEVICE_DT_GET(INPUT_PORT);
    gpio_pin_configure(input_dev, INPUT_PIN, GPIO_INPUT);

    // Conecta a interrupção via Zephyr
    IRQ_CONNECT(TPM_IRQ_LINE, TPM_IRQ_PRIORITY, tpm1_isr, NULL, 0);
    irq_enable(TPM_IRQ_LINE);

    // Inicializa TPM1 e TPM0 com módulo e prescaler desejado
    pwm_tpm_Init(TPM1, TPM_PLLFLL, 65535, TPM_CLK, PS_16, EDGE_PWM);
    pwm_tpm_Init(TPM0, TPM_PLLFLL, 960, TPM_CLK, PS_1, EDGE_PWM);

    // Configura TPM1_CH0 como input capture na borda de subida
    pwm_tpm_Ch_Init(TPM1, 0, TPM_INPUT_CAPTURE_BOTH | TPM_CHANNEL_INTERRUPT, GPIOB, 0);
    pwm_tpm_Ch_Init(TPM0, 1, TPM_PWM_H, GPIOD, 1);
    pwm_tpm_CnV(TPM0, 1, 480);
}