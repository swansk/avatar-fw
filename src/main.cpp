#include <mbed.h>

#include "Controller.h"
#include "RS485KatzComm.h"
#include "DRV8301.h"
#include "AS5047P.h"
#include "RS485.h"
#include "USBSerial.h"

#define CPR 16384
#define OFFSET 0
#define POLE_PAIRS 21

DigitalOut led1(LED1);
DigitalOut led2(LED2);

int main() {
  DigitalOut det = DigitalOut(PE_2);
  // Serial pc = Serial(PA_2, PA_3);
  // pc.baud(9600);

  SPI spi = SPI(PB_15, PB_14, PB_13);
  spi.format(16, 1);  // mbed v>127 breaks 16-bit spi, so transaction is broken into 2 8-bit words
  spi.frequency(1000000);
  DigitalOut encoderSel = DigitalOut(PB_12);

  RS485 rs485 = RS485(PA_0, PA_1, NC);
  DigitalOut de = DigitalOut(PC_15);
  DigitalOut nre = DigitalOut(PA_2);

  DigitalOut drvSel = DigitalOut(PA_6);
  DigitalOut drvEn = DigitalOut(PE_14);
  DRV8301 drv8301 = DRV8301(&spi, &drvSel, &drvEn);
  AS5047P as5047p = AS5047P(CPR, OFFSET, POLE_PAIRS, spi, encoderSel);

  Controller controller = Controller(as5047p, drv8301);
  // Controller controller = Controller(as5047p);
  RS485KatzComm katzComm = RS485KatzComm(rs485, de, nre, &controller);

  // katzComm.listen_begin();

  uint8_t indicator = 0;
  while (true) {
    indicator++;
    // det.write(indicator);
    drv8301.write_CTR1(VDS_LVL_1_892, OCP_NONE, PWM_MODE_3X, GATE_RESET_FAULTS, 0);
    uint16_t drv_read = drv8301.read_register(CTR1) & 0x7FF;
    as5047p.Sample(0.000025);
    // pc.printf("val: %d\n", as5047p.GetRawPosition());
    // led1.write(indicator);
    uint16_t read = as5047p.GetRawPosition();
    // float ranged_read = ((float)read / 4096.0) * 2 * PI;
    katzComm.send_status_msg(2, read, drv_read, 4);
    //   uint8_t m[2] = {0x00, 0x01};
    //   rs485.sendMsg(m, sizeof(m));
    ThisThread::sleep_for(1000);
  }

}