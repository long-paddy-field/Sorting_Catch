asm(".global _printf_float");

#include"mbed.h"
#include"stepper.h"
#include"controller.h"

CAN can(PA_11,PA_12,50000);
CANMessage msg1(0x1,CANStandard);
CANMessage msg3(0x3,CANStandard);
CANMessage msg4(0x4,CANStandard);
Controller controller(can,0x334);
DigitalIn sw(PA_10);
PwmOut dc_con(PA_7);
DigitalOut dc_dir(PA_6);

DigitalOut stepping(PA_1);
DigitalOut dir(PA_3);


int main()
{
  dc_con.period_ms(20);
  dc_con = 0;
  sw.mode(PullUp);

  printf("now waiting\n");

/*  while(sw)
  {

  }
  while(1)
  {
    if(controller.buttons)
    wait_us(100000);
  }
  */
  while(!can.read(msg1))
  {

  }
  printf("start SHIWAKE!\n");
  while(!can.read(msg4))
  {
    if(can.read(msg3) && msg3.data[0] == 1)
    {
      if(controller.buttons[0]==1)
      {
        //ステッパー左
        dir = 0;
        stepping = !stepping;
      }else if(controller.buttons[2]==1)
      {
        //ステッパー右
        dir = 1;
        stepping = !stepping;
      }
      if(controller.buttons[1]==1)
      {
        dc_dir = 1;
        dc_con = 0.3;
        //コンベア正転
      }else if(controller.buttons[3]==1)
      {
        dc_dir = 0;
        dc_con = 0.3;
        //コンベア逆転
      }
    }
    wait_us(4000);
  }
  dc_con = 0;
  return 0; 
}