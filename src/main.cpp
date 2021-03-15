#include <mbed.h>
#include "DRV.h"
#include "FastPWM.h"

#define PIN_U PA_10
#define PIN_V PA_9
#define PIN_W PA_8
#define PWM_ARR 0x8CA /// timer autoreload value

// typedef struct
// {
//   DigitalOut *enable;
//   DigitalOut *led;
//   FastPWM *pwm_u, *pwm_v, *pwm_w;
// } GPIOStruct;

DigitalOut led1(LED1);
DigitalOut led2(LED2);
// GPIOStruct gpio;
InterruptIn button(BUTTON1);

// void Init_PWM(GPIOStruct *gpio)
// {

//   RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // enable the clock to GPIOC
//   RCC->APB1ENR |= 0x00000001;          // enable TIM2 clock
//   RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;  // enable TIM1 clock

//   GPIOC->MODER |= (1 << 10); // set pin 5 to be general purpose output for LED
//   gpio->pwm_u = new FastPWM(PIN_U);
//   gpio->pwm_v = new FastPWM(PIN_V);
//   gpio->pwm_w = new FastPWM(PIN_W);

//   //ISR Setup

//   NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); //Enable TIM1 IRQ

//   TIM1->DIER |= TIM_DIER_UIE; // enable update interrupt
//   TIM1->CR1 = 0x40;           // CMS = 10, interrupt only when counting up
//   TIM1->CR1 |= TIM_CR1_UDIS;
//   TIM1->CR1 |= TIM_CR1_ARPE; // autoreload on,
//   TIM1->RCR |= 0x001;        // update event once per up/down count of tim1
//   TIM1->EGR |= TIM_EGR_UG;

//   //PWM Setup

//   TIM1->PSC = 0x0;                 // no prescaler, timer counts up in sync with the peripheral clock
//   TIM1->ARR = PWM_ARR;             // set auto reload, 40 khz
//   TIM1->CCER |= ~(TIM_CCER_CC1NP); // Interupt when low side is on.
//   TIM1->CR1 |= TIM_CR1_CEN;        // enable TIM1
// }

// extern "C" void TIM1_UP_TIM10_IRQHandler(void)
// {
//   TIM1->CCR3 = (PWM_ARR)*0.1f;
//   TIM1->CCR2 = (PWM_ARR)*0.2f;
//   TIM1->CCR1 = (PWM_ARR)*0.3f;

//   TIM1->SR = 0x0; // reset the status register
// }

int main()
{
  //Init_PWM(&gpio);

  // put your setup code here, to run once:
  Serial pc(PA_2, PA_3);
  pc.baud(9600);
  pc.printf("Hello world\r\n");

  DigitalOut drv_en(PE_10);
  drv_en = 1;
  SPI drv_spi(PA_7, PA_6, PA_5); // mosi, miso, sclk, cs
  DigitalOut drv_cs(PA_4);

  DRV8301 drv(&drv_spi, &drv_cs, &drv_en);
  int pete = drv.read_register(CTR1) & 0x7FF;
  int anja = drv.read_register(CTR2) & 0x7FF;
  drv.write_CTR1(VDS_LVL_1_892, OCP_NONE, PWM_MODE_3X, GATE_RESET_FAULTS, 0);
  drv.write_CTR2(0, CSA2_CONNECT, CSA1_CONNECT, CSA_GAIN_40, REPORT_OT);
  int spen = drv.read_register(CTR1) & 0x7FF;
  int karl = drv.read_register(CTR2) & 0x7FF;

  while (1)
  {
    led1 = !led1;
    pc.printf("Pete Says: %d\r\n", pete);
    pc.printf("Anja Says: %d\r\n", anja);
    pc.printf("Spencer Says: %d\r\n", spen); // should be 1912
    pc.printf("Karl Says: %d\r\n\n", karl);  // should be 9 (unless writes change)
    thread_sleep_for(1000);
  }
}