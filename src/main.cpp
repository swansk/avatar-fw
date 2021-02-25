#include <mbed.h>

DigitalOut led1(LED1);
DigitalOut led2(LED2);

int main() {

  // put your setup code here, to run once:
  while(1) {
    led1 = !led1;
    thread_sleep_for(1000);
  }
}