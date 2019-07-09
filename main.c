#include <stdint.h>
#include "bsp.h"
#include "miros.h"

uint32_t blinky1Stack[40];
OSThread blinky1SP;


uint32_t blinky2Stack[40];
OSThread blinky2SP;

uint32_t blinky3Stack[40];
OSThread blinky3SP;

uint32_t idleStack[40];

void OSIdleApplication(void)
{
	__wfi(); /* sleep */
}

void main_blinky1() 
{
    while (1) {
			  BSP_ledGreenOn();
        OSDelay(BSP_TICKS_PER_SEC / 4U);
			  BSP_ledGreenOff();
        OSDelay(BSP_TICKS_PER_SEC * 3U / 4U);
    }
}


void main_blinky2() {
    while (1) {
			  BSP_ledBlueOn();
        OSDelay(BSP_TICKS_PER_SEC / 2U);
			  BSP_ledBlueOff();
        OSDelay(BSP_TICKS_PER_SEC / 3U);
    }
}

void main_blinky3() {
    while (1) {
			  BSP_ledRedOn();
        OSDelay(BSP_TICKS_PER_SEC / 3U);
			  BSP_ledRedOff();
        OSDelay(BSP_TICKS_PER_SEC *3U / 5U);
    }
}

/* background code: sequential with blocking version */
int main() {
    BSP_init();
	  OSInit(idleStack,sizeof(idleStack));
  
		OSThreadStart(&blinky1SP,
									main_blinky1,
									blinky1Stack,
									sizeof(blinky1Stack));

		OSThreadStart(&blinky2SP,
									main_blinky2,
									blinky2Stack,
									sizeof(blinky2Stack));
	
			OSThreadStart(&blinky3SP,
									main_blinky3,
									blinky3Stack,
									sizeof(blinky3Stack));
	OSStart();
}
