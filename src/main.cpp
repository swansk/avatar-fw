#include <mbed.h>

#include "Controller.h"
#include "RS485KatzComm.h"
#include "DRV8301.h"
#include "AS5047P.h"
#include "RS485.h"

#define CPR 8192
#define OFFSET 0
#define POLE_PAIRS 21

DigitalOut led1(LED1);
DigitalOut led2(LED2);

int main() {
  Serial pc = Serial(PA_9, PA_10);

  SPI spi = SPI(NC, NC, NC);
  DigitalOut drv8301CS = DigitalOut(NC);

  RS485 rs485 = RS485(NC, NC, NC);
  DigitalOut duplexSel = DigitalOut(NC);

  DRV8301 drv8301 = DRV8301(&spi, &drv8301CS);
  AS5047P as5047p = AS5047P(CPR, OFFSET, POLE_PAIRS);

  Controller controller = Controller(as5047p, drv8301);

  RS485KatzComm katzComm = RS485KatzComm(rs485, duplexSel, &controller);

  katzComm.listen_begin();

}