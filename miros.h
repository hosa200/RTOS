#ifndef MIROS_H
#define MIROS_H

#include <stdint.h>

/* SYSTICK Priority address */
#define SYSTICK_HANDLER_ADD 0xE000ED20U
/* OS ISR priority address (PenSV Priority address) */
#define OS_PRIORITY_ADD 0xE000ED20U
/* OS ISR trigger address (PenSV trigger address) */
#define OS_SET_ADD 0xE000ED04U
/* NULL pointer */
#define NULL_THREAD (void*)0U


/******************************************************************************
************************* Global Types Defentations **************************
******************************************************************************/
/* Thread control block (TCB) */
typedef struct{
	void* vidptrSP;							/* thread stack pointer */
	uint32_t u8ThreadTimeOut;		/* time left for thread to be in run state */
}OSThread;

/* pointer to thread handler */
typedef void (*OSThreadHandler)(void);		

/******************************************************************************
************************* Global Function Prototypes **************************
******************************************************************************/
void OSThreadStart(OSThread* osthpMe,
									 OSThreadHandler osthhMe,
									 void* vidStk,
									 uint32_t u32StkSize);


void OSInit(void* vidpStk, uint32_t u32StkSize);
							
void OSSched(void);
									 
void OSStart(void);
									 
void OSDelay(uint32_t ticks);

void OSTick(void);
									 
extern void OSStartup(void);
									 
extern void OSIdleApplication(void);
/******************************************************************************
************************* Global Variables Defentations **************************
******************************************************************************/									 

									 
#endif /* MIROS_H */

