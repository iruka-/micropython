#ifndef _spi2_h_
#define _spi2_h_

#include "util.h"
//#define	_ISR    __attribute__((interrupt,nomips16,noinline))

//================================================================
//	SPI Config:
#define	DMA_INTERRUPT			(0)	// DMA���荞�݂��g�p����.

//	DMA���g�p����1LINE���̏�������.
#define	USE_SPI2_INTERRUPT		(0)	// SPI2 ���M���荞�݂��g�p����.
#define	USE_DMA					(1)	// DMA  ���g�p����. (0)= SPI_write�ő�p.


//================================================================
#include "utype.h"

//================================================================
void	SPI2_mode(uchar mode);
void	SPI2_clock(uint speed);
void	SPI2_close();
void	SPI2_init();

uint 	SPI2_write(uint data_out);
uint 	SPI2_read(void);
//================================================================

void 	NTSC_init(void);
void 	_MIPS32 wait_ms(int ms);
void 	draw_screen(void);
void	putch_cls();

//================================================================
//	��ʃT�C�Y
#define	WIDTH_DMA	40				// 4�̔{���Ɍ���.
#define	WIDTH		(WIDTH_DMA*8)	// 320dot
#define	HEIGHT		200				// 26�s

//	���E�]���T�C�Y
#define LSPACE		5		// ������
#define RSPACE		1		// ������

#endif

