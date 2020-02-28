/*
 ============================================================================
 Name        : TMU.c
 Author      : Muhammed Gamal
 Description : 
 ============================================================================
 */


/*********************************includes**********************************/
#include "TMU.h"
/***************************************************************************/


/*********************************definitions*******************************/
#define NULL_PTR ((void*)0)
#define CLEAR 0
#define COUNTER 0
#define DELAY 1
#define START 0
#define FLAG_HIGH 1
#define FLAG_LOW 0
#define NUM_OF_ARR_ELEMENTS 2
#define TIMER0_COMPARE_VALUE 250
#define ONE 1
#define FIRST_CHANNEL 0
/***************************************************************************/


/******************************global variables*****************************/
static TMU_ConfigChannel * TMU_Channels = NULL_PTR;
static uint8_t TMU_Status = TMU_NOT_INITIALIZED;
void (*g8_functionHoler[NUMBER_OF_CHANNELS])(void);
uint32_t gu32_Counter_Delay_Arr[NUMBER_OF_CHANNELS][NUM_OF_ARR_ELEMENTS];
uint8_t gu8_Active_Channels[NUMBER_OF_CHANNELS];
uint8_t Loop_Counter;
/***************************************************************************/




/************************************************************************************
* Function Name: TMU_Init
* Parameters (in):
* Parameters (inout): None
* Parameters (out): None
* Return value: u8_error- variable that describe the error status
* Description: Initiates the module
************************************************************************************/
ERROR_STATUS TMU_Init(TMU_ConfigType* ConfigPtr)
{
	uint8_t u8_status=E_ok;
	if(ConfigPtr == NULL_PTR || TMU_Status == TMU_INITIALIZED)
	{
		u8_status |= E_NOk;
	}
	else
	{
		TMU_Channels = ConfigPtr->Channels;

		St_TimerCfg timer0_stru_init=
		{
			timer0_stru_init.Timer_CH_NO=Timer_0,
			timer0_stru_init.Timer_Mode=T0_COMP_MODE,
			timer0_stru_init.Timer_Polling_Or_Interrupt=T0_INTERRUPT_CMP,
			timer0_stru_init.Timer_Prescaler=T0_PRESCALER_64,
		};
		Timer_Init(&timer0_stru_init);
		TMU_Status = TMU_INITIALIZED;
	}

	if(u8_status == E_ok)
	{
		u8_status = E_ok;
	}
	else
	{
		u8_status = E_NOk;
	}
	return u8_status;
}




/************************************************************************************
* Function Name: TMU_Start
* Parameters (in):
* Parameters (inout): None
* Parameters (out): None
* Return value: u8_error- variable that describe the error status
* Description:
************************************************************************************/
ERROR_STATUS TMU_Start(void (*ptr)(void), uint8_t channel_id, uint8_t repetition, uint16_t delay)
{
	uint8_t u8_status=E_ok;
	if(ptr == NULL_PTR ||   /*pointer to function is null*/
		TMU_Status == TMU_NOT_INITIALIZED ||  /*TMU module is not initialized*/
		((gu8_Active_Channels[channel_id]==channel_id) && (channel_id!=FIRST_CHANNEL)) ||  /*restart a channel*/
		channel_id >= NUMBER_OF_CHANNELS ||  /*the TMU buffer is full*/
		repetition>INFINITE  /*unexpected repetition input*/
	  )
	{
		u8_status |= E_NOk;
	}

	else
	{
		switch(TMU_Channels[channel_id].Timer_CH_NO)
		{
			case Timer_0:
				Timer_Start(Timer_0,TIMER0_COMPARE_VALUE);
				break;
			case Timer_1:

				u8_status |= E_NOk;
				break;
			case Timer_2:

				u8_status |= E_NOk;
				break;
			default:
				u8_status |= E_NOk;
		}

		g8_functionHoler[channel_id]=ptr;
		gu32_Counter_Delay_Arr[channel_id][COUNTER]=CLEAR;
		gu32_Counter_Delay_Arr[channel_id][DELAY]=delay;
		TMU_Channels[channel_id].Repetition=repetition;
		gu8_Active_Channels[channel_id]=channel_id;
	}

	if(u8_status == E_ok)
	{
		u8_status = E_ok;
	}
	else
	{
		u8_status = E_NOk;
	}
	return u8_status;
}




/************************************************************************************
* Function Name: TMU_Dispatcher
* Parameters (in):
* Parameters (inout): None
* Parameters (out): None
* Return value:
* Description:
************************************************************************************/
void TMU_Dispatch(void)
{
	if(gu8_Timer0CompFlag==FLAG_HIGH)
	{
		gu8_Timer0CompFlag=FLAG_LOW;
		for(Loop_Counter=START;Loop_Counter<NUMBER_OF_CHANNELS;Loop_Counter++)
		{
			if(gu32_Counter_Delay_Arr[Loop_Counter][COUNTER]==gu32_Counter_Delay_Arr[Loop_Counter][DELAY] && gu32_Counter_Delay_Arr[Loop_Counter][DELAY]!=0)
			{
				(*g8_functionHoler[Loop_Counter])();
				if(TMU_Channels[Loop_Counter].Repetition==INFINITE)
				{
					gu32_Counter_Delay_Arr[Loop_Counter][COUNTER]=CLEAR;
				}
				else
				{
					gu32_Counter_Delay_Arr[Loop_Counter][COUNTER]=gu32_Counter_Delay_Arr[Loop_Counter][DELAY]+ONE;
				}
			}
			gu32_Counter_Delay_Arr[Loop_Counter][COUNTER]++;
		}
	}
}



/************************************************************************************
* Function Name: TMU_DeInit
* Parameters (in):
* Parameters (inout): None
* Parameters (out): None
* Return value:
* Description:
************************************************************************************/
ERROR_STATUS TMU_DeInit(void)
{
	uint8_t u8_status=E_ok;
	if(TMU_Status == TMU_NOT_INITIALIZED)
	{
		u8_status |= E_NOk;
	}
	else
	{
		Timer_DeInit(Timer_0);
		TMU_Status = TMU_NOT_INITIALIZED;

		for(int i=0;i<NUMBER_OF_CHANNELS;i++)
		{
			TMU_Stop(i);
		}
	}

	if(u8_status == E_ok)
	{
		u8_status = E_ok;
	}

	else
	{
		u8_status = E_NOk;
	}

	return u8_status;
}



/************************************************************************************
* Function Name: TMU_DeInit
* Parameters (in):
* Parameters (inout): None
* Parameters (out): None
* Return value:
* Description:
************************************************************************************/
ERROR_STATUS TMU_Stop(uint8_t Channel_ID)
{
	uint8_t u8_status=E_ok;

	/* check if the channel has been started or not
	 * no need to check if the channel is initialized or not since no channel
	 * will start without initializing the module
	 *  */
	if(gu8_Active_Channels[Channel_ID]!=Channel_ID)
	{
		u8_status |= E_NOk;
	}
	else
	{
		g8_functionHoler[Channel_ID]=NULL_PTR;
		gu32_Counter_Delay_Arr[Channel_ID][COUNTER]=CLEAR;
		gu32_Counter_Delay_Arr[Channel_ID][DELAY]=CLEAR;
		TMU_Channels[Channel_ID].Repetition=ONE_SHOOT;
		gu8_Active_Channels[Channel_ID]=CLEAR;
	}

	if(u8_status == E_ok)
	{
		u8_status = E_ok;
	}

	else
	{
		u8_status = E_NOk;
	}

	return u8_status;
}





