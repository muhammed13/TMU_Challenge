/*
 ============================================================================
 Name        : TMU.h
 Author      : Muhammed Gamal
 Description : 
 ============================================================================
 */

#ifndef TMU_H_
#define TMU_H_

/*********************************includes**********************************/
#include "std_types.h"
#include "registers.h"
#include "interrupt.h"
#include "Timer.h"
#include "TMU_Cfg.h"
/***************************************************************************/


/*******************************definitions********************************/
#define TMU_INITIALIZED                (1U)
#define TMU_NOT_INITIALIZED            (0U)

#define ONE_SHOOT                      0
#define INFINITE                       1

/************************************************************************/
/*			          Structures Definitions	                        */
/************************************************************************/



typedef struct
{
	uint8_t Timer_CH_NO;
	uint8_t Resolution;
	uint8_t Repetition;
} TMU_ConfigChannel;

typedef struct
{
	TMU_ConfigChannel Channels[NUMBER_OF_CHANNELS];
} TMU_ConfigType;

/***************************************************************************/


/******************************global variables*****************************/
/* Extern structures to be used by TMU and other modules */
extern const TMU_ConfigType TMU_Configuration;
/***************************************************************************/


/***************************functions prototypes****************************/
ERROR_STATUS TMU_Init(TMU_ConfigType* ConfigPtr);
ERROR_STATUS TMU_Start(void (*ptr)(void), uint8_t channel_id, uint8_t repetition, uint16_t delay);
void TMU_Dispatch(void);
ERROR_STATUS TMU_DeInit(void);
ERROR_STATUS TMU_Stop(uint8_t Channel_ID);
/***************************************************************************/

#endif /* TMU_H_ */
