#include "miros.h"

/* pointer to the next thread to run */
OSThread * volatile OSThreadNxt;
/* pointer to the current running thread */
OSThread * volatile OSThreadCurr;

/* OS threads array to hold the addresses of the system threads  */
/* TODO: why 32+1 ? +1 for idel thread but why 32 threads ? */
static OSThread* OSThreads[32+1];
/* counter for threads executed */
static uint8_t u8ThreadCtr = 0;
/* number of threads created by user */
static uint8_t u8ThreadNum = 0;
/* State of Threads in terms of ready or not ready to run  */
static uint32_t u32ThreadsState = 0;

/*******************************************************************************
 * Idle Thread section
 ******************************************************************************/
OSThread osthIdleThreadStP;

void vidIdleHandler() 
{
    while (1) 
		{
			OSIdleApplication();
    }
}

/*******************************************************************************
 * OS initialization function
 * Set OS ISR priority to lowest ISR priority 
 * Fabricate Idle Thread
 ******************************************************************************/
void OSInit(void* vidpStk, uint32_t u32StkSize)
{								 
	*(uint32_t volatile *)(OS_PRIORITY_ADD) |= (0xFFU << 16);

		OSThreadStart(&osthIdleThreadStP,
									vidIdleHandler,
									vidpStk,
									u32StkSize);
}

/*******************************************************************************
 * start the OS behaviour *
 ******************************************************************************/
void OSStart(void)
{	
	OSStartup();
	__disable_irq();
	OSSched();
	__enable_irq();
}

/*******************************************************************************
 * Creates the thread 
 * fabricate the thread stack
 * add the thread to the thread array
 ******************************************************************************/
void OSThreadStart(OSThread* osthpMe,
									 OSThreadHandler osthhMe,
									 void* vidpStk,
									 uint32_t u32StkSize)
{
	uint32_t* u32pSP = (uint32_t*)((((uint32_t)vidpStk + u32StkSize)/8)*8);
	uint32_t* u32pStkLmt ;
	
	/* fabricate ThreadStack from top to bottom of stack*/
	*(--u32pSP) = ( 1U << 24 ); 		  /* XPSR */
	*(--u32pSP) = (uint32_t)osthhMe;  /* PC */
	*(--u32pSP) = 0x0000000EU;			  /* LR */
	*(--u32pSP) = 0x0000000CU;			  /* R12 */
	*(--u32pSP) = 0x00000003U;			  /* R3 */
	*(--u32pSP) = 0x00000002U;			  /* R2 */
	*(--u32pSP) = 0x00000001U;			  /* R1 */
	*(--u32pSP) = 0x00000000U;			  /* R0 */
	*(--u32pSP) = 0x0000000BU;			  /* R11 */
	*(--u32pSP) = 0x0000000AU;			  /* R10 */
	*(--u32pSP) = 0x00000009U;			  /* R9 */
	*(--u32pSP) = 0x00000008U;			  /* R8 */
	*(--u32pSP) = 0x00000007U;			  /* R7 */
	*(--u32pSP) = 0x00000006U;			  /* R6 */
	*(--u32pSP) = 0x00000005U;			  /* R5 */
	*(--u32pSP) = 0x00000004U;			  /* R4 */	
	
	/* save the start of the stack to thread TCB */
	osthpMe->vidptrSP = u32pSP;
	osthpMe->u8ThreadTimeOut = 0;
	
	/* Fill rest of stack with 0xAABBCCDD to ease the debugging */
	u32pStkLmt = (uint32_t*)vidpStk;
	while(u32pStkLmt != u32pSP )
	{
		*u32pStkLmt = 0xAABBCCDD;
		u32pStkLmt++;
	}
	
	/* increment the number of threads created */
	if(u8ThreadNum < 32)
	{
		OSThreads[u8ThreadNum] = osthpMe;
		/* set thread state to ready but not idle thread */
		if(u8ThreadNum > 0)
		{
			u32ThreadsState |= (1<< u8ThreadNum );
		}
		else
		{
			/* Do nothing */
		}
		u8ThreadNum++;
	}
	else
	{
		/* TODO:return error */
	}
	
}

/*******************************************************************************
 * schedule thread to be called by application on system ISR timer 
 * the scheduler supports round robin algorthim
 * the scheduler selects next thread then trigger an OS reserved ISR
 ******************************************************************************/
void OSSched()
{
//	if(u8ThreadCtr < u8ThreadNum)
//	{
		if ( (u32ThreadsState) == 0U )
		{
			u8ThreadCtr = 0U;
		}	
		else
		{	
		do
		{
			u8ThreadCtr++;
			if(u8ThreadCtr > u8ThreadNum)
			{
				u8ThreadCtr = 1U;
			}
			else
			{
				/* Do no thing */
			}
		}while( !(u32ThreadsState & (1 << (u8ThreadCtr) )) );/* nxt thread is blocked */

		}
		OSThreadNxt = OSThreads[u8ThreadCtr];
	//}
	//else
	//{
	//	OSThreadNxt = OSThreads[u8ThreadCtr];		
	//}

	if(OSThreadNxt != OSThreadCurr)
	{
		/* Trigger OS reserved ISR */
		*(uint32_t volatile*)(OS_SET_ADD) |= (1U << 28);
	}
}

/*******************************************************************************
 * 
 ******************************************************************************/
void OSDelay(uint32_t ticks)
{
	__disable_irq();
	OSThreadCurr->u8ThreadTimeOut = ticks;
	u32ThreadsState &= ~(1 << (u8ThreadCtr) );
	OSSched();
	__enable_irq();
}

/*******************************************************************************
*
 ******************************************************************************/
void OSTick(void)
{
	uint32_t n = 0U;
	for( n=1U ; n<u8ThreadNum ; ++n )
	{
		if(OSThreads[n]->u8ThreadTimeOut > 0U)
		{
			--OSThreads[n]->u8ThreadTimeOut;
			if( OSThreads[n]->u8ThreadTimeOut == 0U )
			{
				/* thread is ready to run */
				u32ThreadsState |= (1 << (n) );
			}
			else
			{
				/* Do Nothing */
			}
		}
		else
		{
			/* Do Nothing */
		}
	}
}


__asm
/*******************************************************************************
 * OS ISR handler
 * responsable for context switch between current and next thread
 ******************************************************************************/
void PendSV_Handler(void)
{
	IMPORT OSThreadCurr
	IMPORT OSThreadNxt
	/*void* sp;
	 * __disable_irq();
	 *	if (OSThreadCurr != NULL_THREAD)
	 *	{
	 *	 push R4-R11 registers on current stack 
	 *		OSThreadCurr->vidptrSP = sp;
	 *	}
	 *	sp = OSThreadNxt->vidptrSP;	
	 *	OSThreadCurr = OSThreadNxt; /why/
	 *	 pop R4-R11 registers 
   * __enable_irq();*/
			
			
			
		/* __disable_irq(); */ 
		CPSID         I

	
	/* if (OSThreadCurr != NULL_THREAD) */
		LDR           r1,=OSThreadCurr
		LDR           r1,[r1,#0x00]
		CBZ           r1,PendSV_Restore
		/* push R4-R11 registers on current stack */
		PUSH 					{r4-r11}
	/* end of if condition */ 
	
		
	/* OSThreadCurr->vidptrSP = sp; */
		LDR           r1,=OSThreadCurr
		LDR           r1,[r1,#0x00]
		STR           sp,[r1,#0x00]

		
	/*if condition is ture-> sp = OSThreadNxt->vidptrSP; */   
PendSV_Restore
		LDR           r1,=OSThreadNxt
		LDR           r1,[r1,#0x00]
		LDR           sp,[r1,#0x00]

		
	/* OSThreadCurr = OSThreadNxt; /why */ 
		LDR           r1,=OSThreadNxt
		LDR           r1,[r1,#0x00]
		LDR           r2,=OSThreadCurr
		STR           r1,[r2,#0x00]

		/* pop R4-R11 registers */  
		POP						{r4-r11}

		/* __enable_irq(); */
		CPSIE         I

		/* return from isr */
		BX            lr
}
