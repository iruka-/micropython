#include <p32xxxx.h>			// always in first place to avoid conflict with const.h ON
#include <stdlib.h>			// always in first place to avoid conflict with const.h ON


#include "util.h"

typedef	unsigned int uint;
void delay_us(int us);
void wait_ms(int ms);

/********************************************************************
 *		
 ********************************************************************
 */
void led_flip()
{
	LATBINV=0x8000;	// LATB.bit15を反転
}

/********************************************************************
 *		
 ********************************************************************
 */
void led_test()
{
	while(1) {
		led_flip();
		wait_ms(500);
	}
}

/********************************************************************
 *		
 ********************************************************************
 */
#define	CNTMAX		400000
//#define	CNTMAX		100000
void led_blink()
{
	static int cnt=0;
	cnt++;
	if(	cnt >= CNTMAX) {
		cnt=0;
		led_flip();
	}
}


void wait_nloop(int n);
/********************************************************************
 *	( ms ) * 1mS 待つ.
 ********************************************************************
 */
void wait_ms(int ms)
{
	int i;
	for(i=0; i<ms; i++) {
		delay_us(1000);
	}
}

/********************************************************************
 *	( us ) * 1uS 待つ.
 ********************************************************************
 */
void delay_us(int us)
{
	wait_nloop(us*10);
}
/********************************************************************
 *	( nloop * 1/10 ) uS 待つ. clock=40MHzと仮定.
 ********************************************************************

9fc00110 <wait_nloop>:
9fc00110:	6500      	nop
9fc00112:	4cff      	addiu	a0,-1
9fc00114:	2cfd      	bnez	a0,9fc00110 <wait_nloop>
9fc00116:	e8a0      	jrc	ra

 */
//clock=40MHzと仮定.	最適化オプションやコンパイラが変わったら再計測.
void wait_nloop(int n)
{
	do {
		asm("nop");
		n--;
	}while( n != 0);
}


#if	0
/********************************************************************
 *	code領域節約 INT系関数.
 ********************************************************************
 */
void __attribute__((nomips16,section(".bootrom"))) INTDisableInterrupts(void)
{
	//asm("di $2");	// $2 = v0
	asm("di");
}


void __attribute__((nomips16,section(".bootrom")))  INTRestoreInterrupts(unsigned int status)
{
	if((status & 1)==0) {
		asm("di");
	}else{
		asm("ei");
	}
}
//void _BOOTROM_ INTClearFlag(INT_SOURCE source)
//{
	// FIXME!
//}
#endif


/********************************************************************
 *	
 ********************************************************************
 */
