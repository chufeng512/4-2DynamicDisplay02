#include <STC89C5xRC.H>
#include <Intrins.h>
#define SW1 P42

// �����Զ������ͣ��򻯴����д
typedef unsigned char u8;
typedef unsigned int u16;

// ��ʱ����
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

// �������飬��װ����
u8 NixieTable[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// �������ʾ
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
    u8 seconds_tens;   // ʮλ��
    u8 seconds_units;  // ��λ��
    u8 seconds;        // ����
    u16 i;             // ѭ������
		bit is_running = 0;	// ��ʱ��״̬����

    while(1)
    {
			if(SW1 == 0)
			{
				Delay1ms(10);
				while(SW1 == 0);
				Delay1ms(10);
				is_running = !is_running; // ̧��->�л���ʱ��״̬
				
				if(is_running)
				{
					seconds = 99; // ��ʼ������ʱΪ99��
					while(is_running && seconds > 0)
					{
						 for(i = 0; i < 500; i++) // ����ʾ�ȶ�1��
              {
                if(SW1 == 0) // ��ⰴť״̬��ֹͣ��ʱ
                {
                 Delay1ms(10); // ����
                 if(SW1 == 0) // ȷ�ϰ�ť����
                  {
										while(SW1 == 0); // �ȴ���ť�ͷ�
										Delay1ms(10); // ����
										is_running = 0; // ֹͣ��ʱ
										break; // ����ѭ��
                   }
                 }
                     seconds_tens = seconds / 10;
                     seconds_units = seconds % 10;

                     NixieDisPlay(1, seconds_tens); // ��ʾʮλ
                     NixieDisPlay(2, seconds_units); // ��ʾ��λ
                }
							if(is_running)
							{
								seconds--; // ����ʱ��1
							}
					}
				}
				else
				{	
					P0 = 0x00; // �ر��������ʾ
				}
			}  
    }    
}
