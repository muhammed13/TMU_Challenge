/*
 ============================================================================
 Name        : main.c
 Author      : Muhammed Gamal
 Description : 
 ============================================================================
 */

#include "TMU.h"
#include "DIO.h"


void func0()
{
	DIO_Cfg_s DIO_stru_init=
	{
			DIO_stru_init.GPIO=GPIOA,
			DIO_stru_init.pins=BIT0,
			DIO_stru_init.dir=OUTPUT,
	};
	DIO_init (&DIO_stru_init);

	DIO_Toggle(GPIOA, BIT0);
}

void func1()
{
	DIO_Cfg_s DIO_stru_init=
	{
			DIO_stru_init.GPIO=GPIOA,
			DIO_stru_init.pins=BIT1,
			DIO_stru_init.dir=OUTPUT,
	};
	DIO_init (&DIO_stru_init);

	DIO_Toggle (GPIOA, BIT1);
}

void func2()
{
	DIO_Cfg_s DIO_stru_init=
	{
			DIO_stru_init.GPIO=GPIOA,
			DIO_stru_init.pins=BIT2,
			DIO_stru_init.dir=OUTPUT,
	};
	DIO_init (&DIO_stru_init);

	DIO_Toggle (GPIOA, BIT2);
}

void func3()
{
	DIO_Cfg_s DIO_stru_init=
	{
			DIO_stru_init.GPIO=GPIOA,
			DIO_stru_init.pins=BIT3,
			DIO_stru_init.dir=OUTPUT,
	};
	DIO_init (&DIO_stru_init);

	DIO_Toggle (GPIOA, BIT3);
}

void func4()
{
	DIO_Cfg_s DIO_stru_init=
	{
			DIO_stru_init.GPIO=GPIOA,
			DIO_stru_init.pins=BIT4,
			DIO_stru_init.dir=OUTPUT,
	};
	DIO_init (&DIO_stru_init);

	DIO_Toggle (GPIOA, BIT4);
}

int main(void)
{
	sei();
	TMU_Init(&TMU_Configuration);
	TMU_Start(func0,0,1,1000);
	TMU_Start(func1,1,1,500);
	//TMU_DeInit();
	TMU_Start(func2,2,0,1000);
	TMU_Stop(1);
	TMU_Start(func3,3,1,1000);
	while(1)
	{
		TMU_Dispatch();
	}

	return 0;
}
