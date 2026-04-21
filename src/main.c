#include <zephyr/kernel.h>             // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saída (GPIO)
#include <pwm_z42.h>                // Biblioteca personalizada com funções de controle do TPM (Timer/PWM Module)

#define TPM_IRQ_LINE TPM1_IRQn  // relaciona a interrupção ao timer TPM1
#define TPM_IRQ_PRIORITY 1      // define a prioridade da interrupção
#define INPUT_PORT DT_NODELABEL(gpiob)
#define INPUT_PIN 0

const struct device *input_dev;
input_dev = DEVICE_DT_GET(INPUT_PORT);
volatile uint16_t t_subida = 0;
volatile uint16_t t_descida = 0;
volatile uint16_t t = 0; 

__ramfunc int16_t tpm1_isr(void *arg) {
    TPM1->STATUS |= TPM_STATUS_CH0F_MASK; // zerra a flag que gerou a interrupção
    if (gpio_pin_get(input_dev, INPUT_PIN) == 1) {
        t_subida = TPM1->CONTROLS[0].CnV;
        return 0;
    }
    else {
        t_descida = TPM1->CONTROLS[0].CnV;
    }
}

int main(void)
{
    // Configura a porta B0 como input
    gpio_pin_configure(input_dev, INPUT_PIN, GPIO_INPUT);

    // Conecta a interrupção via Zephyr
    IRQ_CONNECT(TPM_IRQ_LINE, TPM_IRQ_PRIORITY, tpm1_isr, NULL, 0);
    irq_enable(TPM_IRQ_LINE);

    // Inicializa TPM1 e TPM0 com módulo e prescaler desejado
    pwm_tpm_Init(TPM1, TPM_PLLFLL, 65535, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Init(TPM0, TPM_PLLFLL, 960, TPM_CLK, PS_1, EDGE_PWM);

    // Configura TPM1_CH0 como input capture na borda de subida
    pwm_tpm_Ch_Init(TPM1, 0, TPM_INPUT_CAPTURE_BOTH | TPM_CHANNEL_INTERRUPT, GPIOB, 0);
    pwm_tpm_Ch_Init(TPM0, 1, TPM_PWM_H, GPIOD, 1);
    pwm_tpm_CnV(TPM0, 1, 480);

    while (1)
    {
        //int temp =  gpio_pin_get(input_dev, INPUT_PIN);
        //printk("Temp = %d\n", temp);
        //printk("Valor do TPM1: %u\n", captured);
        k_msleep(1000); 
    }
}