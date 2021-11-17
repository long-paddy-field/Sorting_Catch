asm(".global _printf_float");

#include"mbed.h"
#include"stepper.h"

CAN can(PA_11,PA_12,50000);
CANMessage msg5(0x5,CANStandard);
CANMessage msg7(0x7,CANStandard);
CANMessage msg8(0x8,CANStandard);

PwmOut dc_con(PB_1);
DigitalOut dc_dir(PB_7);

DigitalOut stepping(PA_2);
DigitalOut dir(PA_1);

Stepper stepper(dir,stepping);
Ticker checkend;

bool R_or_L = true;

void endprotocol();

int main()
{
  dc_con.period_ms(20);
  dc_con = 0;
  while(!can.read(msg5))
  {

  }
  checkend.attach(callback(&endprotocol),100ms);

  while(1)
  {
    if(can.read(msg7))
    {
      if(R_or_L)
      {
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
        dc_con = 0.7;
        dc_dir = 1;
        wait_us(3000000);
        dc_con = 0;
      }
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