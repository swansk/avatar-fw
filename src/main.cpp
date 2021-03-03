#include "mbed.h"

DigitalOut led1(LED1); // orange
DigitalOut led2(LED2); // green
DigitalOut led5(LED5); // red
DigitalOut led6(LED6); // blue
Thread thread1;
Thread thread2;
InterruptIn button(BUTTON1);

void int_routine()
{
  led1 = 0;
  led2 = 0;
  led5 = 0;
  led6 = 0;
  wait_us(1000000);
}

void led1_thread()
{
  while (true)
  {
    led1 = !led1;
    ThisThread::sleep_for(500);
  }
}

void led2_thread()
{
  while (true)
  {
    led2 = !led2;
    ThisThread::sleep_for(750);
  }
}

int main()
{
  // put your setup code here, to run once:
  button.fall(&int_routine);
  thread1.start(led1_thread);
  thread2.start(led2_thread);
  while (1)
  {
    led5 = !led5;
    led6 = !led6;
    ThisThread::sleep_for(1000);
  }
}