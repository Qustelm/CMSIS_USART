#include "stm32f10x.h"
#include <stdint.h>

void USART1_INIT() {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // usart clock enable
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // GPIOA clock enable

    // configure PA9 (TX) as alternate function (push-pull output)
    GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH |= GPIO_CRH_MODE9_1;

    USART1->BRR = SystemCoreClock/9600; // set baud rate
    USART1->CR1 |= USART_CR1_UE; // USART1 enable
    USART1->CR1 |= USART_CR1_TE; // transmitter enable
}

void USART_SEND(uint8_t byte) {
    // wait until transmit data register is empty (ready for new data)
    while (!(USART1->SR & USART_SR_TXE));
    // write data to the Data Register (DR)
    USART1->DR = byte;
}

int main(void) {
    USART1_INIT();

    while (1) {
        USART_SEND('1');
    }
}
