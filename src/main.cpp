#include <mbed.h>
#include "DRV.h"
// #include <string.h>
DigitalOut led1(LED1);
DigitalOut led2(LED2);

// string to_bin(int num)
// {
//   string ret = "";
//   for (int i = 10; i >= 0; i--)
//   {
//     if (num >> i & 1 == 1)
//       ret.append("1");
//     else
//       ret.append("0");
//   }
//   return ret;
// }

int main()
{

  // put your setup code here, to run once:
  Serial pc(PA_2, PA_3);
  pc.baud(9600);
  pc.printf("Hello world\r\n");

  DigitalOut drv_en(PE_10);
  drv_en = 1;
  SPI drv_spi(PA_7, PA_6, PA_5); // mosi, miso, sclk, cs
  DigitalOut drv_cs(PA_4);

  DRV8301 drv(&drv_spi, &drv_cs, &drv_en);
  int pete = drv.read_register(CR1) & 0x7FF;
  int anja = drv.read_register(CR2) & 0x7FF;
  drv.write_CR1(VDS_LVL_1_892, OCP_NONE, PWM_MODE_3X, GATE_RESET_FAULTS, 0);
  drv.write_CR2(0, CSA2_DISCONNECT, CSA1_DISCONNECT, CSA_GAIN_40, REPORT_OT);
  int spen = drv.read_register(CR1) & 0x7FF;
  int karl = drv.read_register(CR2) & 0x7FF;

  while (1)
  {
    led1 = !led1;
    pc.printf("Pete Says: %d\r\n", pete);
    pc.printf("Anja Says: %d\r\n", anja);
    pc.printf("Spencer Says: %d\r\n", spen);
    pc.printf("Karl Says: %d\r\n\n", karl);
    thread_sleep_for(1000);
  }
}