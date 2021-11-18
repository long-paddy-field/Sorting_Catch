asm(".global _printf_float");

#include"mbed.h"
#include"stepper.h"

CAN can(PA_11,PA_12,50000);
CANMessage msg5(0x5,CANStandard);
CANMessage msg7(0x7,CANStandard);
CANMessage msg8(0x8,CANStandard);
DigitalIn sw(PA_10);
PwmOut dc_con(PA_7);
DigitalOut dc_dir(PA_6);

DigitalOut stepping(PA_1);
DigitalOut dir(PA_3);

Stepper stepper(dir,stepping);
Ticker checkend;

bool R_or_L = true;

void endprotocol();

int main()
{
  dc_con.period_ms(20);
  dc_con = 0;
  sw.mode(PullUp);

  printf("now waiting\n");

/*  while(sw)
  {

  }
  stepper.start(500,0);
  printf("dir = 0\n");
  while(stepper.isMoving)
  {
    printf("stepnum = %d\n",stepper._stepNum);
    wait_us(500000);
  }
  stepper.start(500,1);
  printf("dir = 1\n");
  */
  while(!can.read(msg5))
  {

  }
  checkend.attach(callback(&endprotocol),100ms);
  printf("start SHIWAKE!\n");

  while(1)
  {
    if(can.read(msg7))
    {
      if(R_or_L)
      {
        printf("activate SHIWAKE\n");
        stepper.start(140,0);
        while(stepper.isMoving)
        {
        }
        stepper.start(140,1);
        dc_con = 0.7;
        dc_dir = 1;
        wait_us(3000000);
        dc_con = 0;
      }
      else{
        printf("non-activate SHIWAKE\n");
        dc_con = 0.7;
        dc_dir = 1;
        wait_us(3000000);
        dc_con = 0;
      }
      R_or_L = !R_or_L;
      //ステッピングモータを回す
    }
    wait_us(100000);
  }
  return 0;
}

void endprotocol()
{
  if(can.read(msg8))
  {
    checkend.detach();
    dc_dir = 0;
    while(1)
    {

    }
  }
  
}