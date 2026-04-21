#include <zephyr/kernel.h>             // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saída (GPIO)
#include <pwm_z42.h>                // Biblioteca personalizada com funções de controle do TPM (Timer/PWM Module)
#include <sensor-ultrassonico.h>

#define TPM_MODULE 1000         // Define a frequência do PWM fpwm = (TPM_CLK / (TPM_MODULE * PS))
#define SLEEP_TIME_MS 1000


int main(void) {
    sensorUltrassonicoInit();

    // Configura os motores
    pwm_tpm_Init(TPM0, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_128, CENTER_PWM);
    pwm_tpm_Ch_Init(TPM0, 3, TPM_PWM_H,GPIOD,3);
    pwm_tpm_Ch_Init(TPM0, 2, TPM_PWM_H,GPIOD,2);
    pwm_tpm_Ch_Init(TPM0, 0, TPM_PWM_H,GPIOD,0);
    pwm_tpm_Ch_Init(TPM0, 5, TPM_PWM_H,GPIOD,5);

    while (1) {
        if (dist > 25) {
            pwm_tpm_CnV(TPM0, 3, 700);
            pwm_tpm_CnV(TPM0, 2, 0);
            pwm_tpm_CnV(TPM0, 0, 700);
            pwm_tpm_CnV(TPM0, 5, 0);
        }
        else {
            pwm_tpm_CnV(TPM0, 3, 0);
            pwm_tpm_CnV(TPM0, 2, 0);
            pwm_tpm_CnV(TPM0, 0, 0);
            pwm_tpm_CnV(TPM0, 5, 0);
        }
    }
}