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
    USART1->CR1 |= USART_CR1_RE;

}

void USART_SEND(uint8_t byte) {
    // wait until transmit data register is empty (ready for new data)
    while (!(USART1->SR & USART_SR_TXE));
    // write data to the Data Register (DR)
    USART1->DR = byte;
}

uint8_t USART_READ() {
    //wait until receiver data register is full
    while (!(USART1->SR & USART_SR_RXNE)); 
    return USART1->DR;
}

int main(void) {
    USART1_INIT();

    // configure PC13 (LED)
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
    GPIOC->CRH |= GPIO_CRH_MODE13_1;

    while (1) {
        char data = USART_READ();
        USART_SEND(data);
        if (data == '1') GPIOC->BSRR = GPIO_BSRR_BS13;
        if (data == '0') GPIOC->BRR = GPIO_BRR_BR13;
    }
}
