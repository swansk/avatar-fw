#include <mbed.h>
#include "DRV.h"
DigitalOut led1(LED1);
DigitalOut led2(LED2);

int main() {

  // put your setup code here, to run once:
  Serial pc(PA_2, PA_3);
  pc.baud(921600);
  pc.printf("Hello world\r\n");
  SPI drv_spi(PA_7, PA_6, PA_5);
  DigitalOut drv_cs(PA_4);
  DRV8301 drv(&drv_spi, &drv_cs);

  drv.write_CR1(1, 1, 0, 1, 1);
  int karl = drv.read_register(CR1);

  

  while(1) {
    led1 = !led1;
    pc.printf("Karl Says: %d", karl);
    thread_sleep_for(1000);
  }
}