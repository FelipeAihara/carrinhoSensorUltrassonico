#include <zephyr/kernel.h>             // Funções básicas do Zephyr (ex: k_msleep, k_thread, etc.)
#include <zephyr/device.h>             // API para obter e utilizar dispositivos do sistema
#include <zephyr/drivers/gpio.h>       // API para controle de pinos de entrada/saída (GPIO)
#include <pwm_z42.h>                // Biblioteca personalizada com funções de controle do TPM (Timer/PWM Module)
#include <sensor-ultrassonico.h>

int main(void) {
    sensorUltrassonicoInit();

    while (1) {
        printk("Distancia em cm = %.1f\n", dist);
        k_msleep(1000);
    }
}