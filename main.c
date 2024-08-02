#include <STC89C5xRC.H>
#include <Intrins.h>
#define SW1 P42

// 增加自定义类型，简化代码编写
typedef unsigned char u8;
typedef unsigned int u16;

// 延时函数
void Delay1ms(u16 ms)    //@11.0592MHz
{
    unsigned char i, j;
    while(ms--)
    {
        _nop_();
        i = 2;
        j = 199;
        do
        {
            while (--j);
        } while (--i);
    }
}

// 定义数组，封装数字
u8 NixieTable[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// 数码管显示
void NixieDisPlay(u8 Location, u8 DisplayNum)
{
    switch(Location)
    {
        case 1: P15 = 0; P14 = 0; P13 = 0; break;
        case 2: P15 = 0; P14 = 0; P13 = 1; break;
        case 3: P15 = 0; P14 = 1; P13 = 0; break;
        case 4: P15 = 0; P14 = 1; P13 = 1; break;
        case 5: P15 = 1; P14 = 0; P13 = 0; break;
        case 6: P15 = 1; P14 = 0; P13 = 1; break;
        case 7: P15 = 1; P14 = 1; P13 = 0; break;
        case 8: P15 = 1; P14 = 1; P13 = 1; break;
    }
    P0 = NixieTable[DisplayNum];
    Delay1ms(1);
    P0 = 0x00;
}

void main()
{
    u8 seconds_tens;   // 十位数
    u8 seconds_units;  // 个位数
    u8 seconds;        // 秒数
    u16 i;             // 循环变量
		bit is_running = 0;	// 计时器状态变量

    while(1)
    {
			if(SW1 == 0)
			{
				Delay1ms(10);
				while(SW1 == 0);
				Delay1ms(10);
				is_running = !is_running; // 抬手->切换计时器状态
				
				if(is_running)
				{
					seconds = 99; // 初始化倒计时为99秒
					while(is_running && seconds > 0)
					{
						 for(i = 0; i < 500; i++) // 让显示稳定1秒
              {
                if(SW1 == 0) // 检测按钮状态以停止计时
                {
                 Delay1ms(10); // 消抖
                 if(SW1 == 0) // 确认按钮按下
                  {
										while(SW1 == 0); // 等待按钮释放
										Delay1ms(10); // 消抖
										is_running = 0; // 停止计时
										break; // 跳出循环
                   }
                 }
                     seconds_tens = seconds / 10;
                     seconds_units = seconds % 10;

                     NixieDisPlay(1, seconds_tens); // 显示十位
                     NixieDisPlay(2, seconds_units); // 显示个位
                }
							if(is_running)
							{
								seconds--; // 倒计时减1
							}
					}
				}
				else
				{	
					P0 = 0x00; // 关闭数码管显示
				}
			}  
    }    
}
