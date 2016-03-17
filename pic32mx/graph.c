/********************************************************************
 *	GRAPHIC SUBROUTINES
 ********************************************************************
 */
#include <stdlib.h>
#include <string.h>
#include <p32xxxx.h>			// always in first place to avoid conflict with const.h ON
//#include <plib.h>

#include "utype.h"
#include "spi2.h"
#include "font8.h"

#define	M_PI	3.14159
#define SWAP(x,y)  {int t; t=(x); (x)=(y); (y)=t; }

/********************************************************************
 *	�錾
 ********************************************************************
 */
#define	GR_LSPACE	(LSPACE*8)				// ���]��dot 5*8=40
#define	GR_RSPACE	(RSPACE*8)				// ���]��dot 5*8=40
#define	GR_WIDTH	(WIDTH - GR_LSPACE-GR_RSPACE)	// ��ʕ�dot 320-5*8*2=240
#define	GR_HEIGHT	(HEIGHT)				// ��ʍ���dot 208

// ���]����32�̔{���ƁA���̗]��ɕ�������.
#define	GR_LSPACE_1	32
#define	GR_LSPACE_2	(GR_LSPACE-GR_LSPACE_1)

//	word size
#define	WSTRIDE		(WIDTH_DMA / 4)			// DMA 1���C������32bit WORD��.

//	bit address
#define	BITMASK(x)	( ((uint)0x80000000) >> (x & 31) )

#define	CWIDTH		(GR_WIDTH/8) //30
#define	CHEIGHT		(HEIGHT/8)	 //26
#define	CSTRIDE		(WIDTH_DMA)	 //40


extern uint	txferTxBuff[];
// �X�N���[���J�[�\�����W
static int sx=0;
static int sy=0;

/********************************************************************
 *	
 ********************************************************************
 */
//	====================================================
//	(x,y)���W����s�N�Z���o�b�t�@�̃|�C���^*p�𓾂�.
//	====================================================
uint *p_adr(int x,int y)
{
	if( ((uint)x < GR_WIDTH) && ((uint)y < GR_HEIGHT) ) {
		x+=GR_LSPACE;
		return &txferTxBuff[ y * WSTRIDE + (x>>5) ];	// (x/32)
	}
	return 0;
}
//	====================================================
//	(x,y)���W�Ƀh�b�g��ł�.
//	====================================================
void pset(int x,int y)
{
	uint *p=p_adr(x,y);
	x+=GR_LSPACE_2;
	if(p) *p |= BITMASK(x);
}
//	====================================================
//	(x,y)���W�̃h�b�g������.
//	====================================================
void preset(int x,int y)
{
	uint *p=p_adr(x,y);
	x+=GR_LSPACE_2;
	if(p) *p &= ~BITMASK(x);
}
//	====================================================
//	(x,y)���W�̃h�b�g�𔽓].
//	====================================================
void prev(int x,int y)
{
	uint *p=p_adr(x,y);
	x+=GR_LSPACE_2;
	if(p) *p ^= BITMASK(x);
}

#define	gr_pset(x,y,c)	pset(x,y)
//	====================================================
//	������`�悷��.
//	====================================================
void gr_line(int x1,int y1,int x2,int y2,int c)
{
	int px,py;		/* �łׂ��_ */
	int r;			/* �덷���W�X�^ */
	int dx,dy,dir,count;

	if(x1 > x2) {SWAP(x1,x2);SWAP(y1,y2);}

	px=x1;py=y1;	/* �J�n�_ */
	dx=x2-x1;	/* �f���^ */
	dy=y2-y1;dir=1;
	if(dy<0) {dy=-dy;dir=-1;} /* ���̌X�� */

	if(dx<dy) {	/* �f���^���̕����傫���ꍇ */
		count=dy+1;
		r=dy/2;
		do {
			gr_pset(px,py,c);py+=dir;
			r+=dx;if(r>=dy) {r-=dy;px++;}
		} while(--count);
	} else {	/* �f���^���̕����傫���ꍇ */
		count=dx+1;
		r=dx/2;
		do {
			gr_pset(px,py,c);px++;
			r+=dy;if(r>=dx) {r-=dx;py+=dir;}
		} while(--count);
	}
}

//	====================================================
//	�~(cx,cy),���ar,�Fc ��`��
//	====================================================
void gr_circle( int cx,int cy,int r,int c)
{
	int  x,y;
	int  xr,yr;

	if(r==0) return;
	x=r * r;y=0;
	do {
		xr= x / r;
		yr= y / r;
		gr_pset(cx+xr,cy+yr,c);
		gr_pset(cx-xr,cy+yr,c);
		gr_pset(cx-xr,cy-yr,c);
		gr_pset(cx+xr,cy-yr,c);

		gr_pset(cx+yr,cy+xr,c);
		gr_pset(cx-yr,cy+xr,c);
		gr_pset(cx-yr,cy-xr,c);
		gr_pset(cx+yr,cy-xr,c);

		x += yr;
		y -= xr;
	}while( x>= (-y) );
}
#if	0
//	���������_���C�u�����������N�����̂ŁA����
//	====================================================
//	�~��(cx,cy),���ar,�Fc,�p�x(begin,end) ��`��
//	====================================================
void gr_circle_arc( int cx,int cy,int rx,int ry,int c,int begin,int end)
{
	float x,y,rad,t,td;
	int xr,yr;
	if(rx>ry) td = rx;
	else	  td = ry;

	td = (360/6.28) / (td * 1.2);

	for(t=begin;t<end;t=t+td) {
		rad = (t * M_PI *2 ) / 360.0;
		x =  cos(rad);
		y =  sin(rad);
		xr = x * (float)rx;
		yr = y * (float)ry;
		gr_pset(cx-xr,cy-yr,c);
	}
}
#endif


//	====================================================
//	(cx,cy)�������W����s�N�Z���o�b�t�@��byte�|�C���^*p�𓾂�.
//	====================================================
uchar *ch_adr(int cx,int cy)
{
	uchar *t = (uchar*)txferTxBuff;
	if( ((uint)cx < CWIDTH) && ((uint)cy < CHEIGHT) ) {
		cx+=LSPACE;
		t += (cy*(8*CSTRIDE)+(cx & 0xfffc)); // 4�����P�ʂ� 1234 --> 4321 �ɂȂ� (DMA Endian���)
		t += (3 -            (cx & 0x0003));
		return t;
	}
	return 0;
}

void gr_putch_xy(int ch,int cx,int cy);

//	====================================================
//	ASCII����(ch) ��8x8�t�H���g�f�[�^(8byte)�𓾂�.
//	====================================================
const uchar *get_font_adr(int ch)
{
	return &font8[ (ch & 0xff) * 8 ];
}

void gr_locate(int cx,int cy)
{
	sx=cx;
	sy=cy;
}

void gr_scrollup(int y)
{
	y *= 8;

	uint *t = txferTxBuff;
	uint *s = t + (y * WSTRIDE);
	int i;
	int m = (HEIGHT - y) * WSTRIDE;
	int n = (HEIGHT    ) * WSTRIDE;

	// 1�s�X�N���[��.
	for(i=0;i<m;i++) *t++ = *s++;
	// �X�N���[���Ō�̍s���[���N���A.
	for(i=m;i<n;i++) *t++ = 0;
}

void gr_cls()
{
	// �X�N���[���s=��ʑS�̂̍s���ɂ��āA��ʃN���A�̑�p�i�ɂ���.
	gr_scrollup(CHEIGHT);
	// �J�[�\���������.
	gr_locate(0,0);
}

void gr_crlf(void)
{
	sx=0;
	sy++;
	if(sy>=CHEIGHT) {
		sy=CHEIGHT-1;
		gr_scrollup(1);
	}
}

void gr_curadv(void)
{
	sx++;
	if(sx>=CWIDTH) {
		gr_crlf();
	}
}

void gr_left(void)
{
	if(	sx == 0) {
		if( sy == 0) {
			return;
		}else{
			sy--;
		}
		sx = CWIDTH-1;
	}else{
		sx--;
	}
}

void gr_bs(void)
{
	gr_left();
	gr_putch_xy(' ', sx,sy);
}

void gr_del(void)
{
	gr_bs();
}

//	====================================================
//	�������W(cx,cy)��ASCII����(ch)���O���t�B�b�N�`��
//	====================================================
void gr_putch_xy(int ch,int cx,int cy)
{
	uchar *t = ch_adr(cx,cy);
	const uchar *font = get_font_adr(ch);

	if(t) {
		int i;
		for(i=0;i<8;i++) {
			*t = *font++;
			t += CSTRIDE;
		}
	}
}

void gr_tab(void)
{
	while(1) {
		int ch = ' ';
		gr_putch_xy(ch , sx,sy);
		gr_curadv();
		if((sx & 7)==0) break;
	}
}

//	====================================================
//	ASCII����(ch)���J�[�\���ʒu�ɏ����A�J�[�\����i�߂�
//	====================================================
void gr_putch(int ch)
{
	if( ch>=' ' ) {
		gr_putch_xy(ch , sx,sy);
		gr_curadv();
	}else{
		switch(ch){
		 case '\n':	gr_crlf();break;	//CRLF
		 case 0x0c:	gr_cls() ;break;	//CLS
		 case 0x08:	gr_bs()  ;break;	//BS
		 case 0x09:	gr_tab() ;break;	//TAB
		 case 0x1c:	gr_curadv();break;	//��
		 case 0x1d:	gr_left();break;	//��
		 case 0x7f:	gr_del() ;break;	//DEL
		 default:
			break;
		}
	}
}

//	====================================================
//	�������W(cx,cy)��ASCII����(ch)���O���t�B�b�N�`��
//	====================================================
void gr_puts(char *str)
{
	while(1) {
		int ch = *str++;if( ch==0 ) break;
		gr_putch(ch);
	}
}

//	====================================================
//	�O���t�B�b�N�e�X�g.
//	====================================================
void gr_test()
{
	int x;
	//
	// Text
	int i;
	gr_locate(0,0);
	for(i=0;i<(CHEIGHT);i++) {
		gr_puts("Hello,World.\n");
	}
	gr_locate(0,0);
	gr_puts("0123456789012345678901234567890123");
	gr_locate(CHEIGHT-1,0);
	//
	// Graphic
	for(x=16;x<HEIGHT;x+=16) {
		gr_line(x,0,GR_WIDTH,x,1);
	}
	gr_circle(GR_WIDTH/2,HEIGHT/2,HEIGHT/2-2,1);
}

