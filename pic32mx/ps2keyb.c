/**********************************************************************
 *	PS2 Keyboard Library
 **********************************************************************
 *
void  kbd_init(void);			// : ������
uchar kbd_getchar(void);		// : ASCII �R�[�h���擾.
uchar kbd_peekcode(void);		// : FIFO�L���[��TOP������.
uchar kbd_getcode(void);		// : FIFO�L���[�̎��o������.
uchar kbd_getpress(uchar c);	// : ���A���^�C�� �C�ӃL�[�̉������ �̎擾.

 */

#include <p32xxxx.h>			// always in first place to avoid conflict with const.h ON
#include <stdio.h>
#include "config.h"
#include "util.h"

//	PS2 KEYBOARD PORT ASSIGN ============
#define PS2KBD_PIN    PORTB
#define PS2KBD_DDR    TRISB
#define PS2KBD_PORT   LATB


//	PS2 KEYBOARD BIT ASSIGN =============
#define PS2KBD_CLOCK  9
#define PS2KBD_DATA   8

//	MASK BIT ============================
#define PS2KBD_CLOCK_MASK  	(1<<PS2KBD_CLOCK)
#define PS2KBD_DATA_MASK   	(1<<PS2KBD_DATA)

#include "ps2keyb.h"
#if	PS2KBD_USE_GETCHAR
#include "keycode.h"
#endif
#include "keyname.h"


#if	PS2KBD_USE_INTERRUPT
//  use PCINT8 hardhandler
#define	INTERRUPT_SERVICE(vect)		ISR(vect)
#else
//	use TIMER1 softhandler
#define	INTERRUPT_SERVICE(vect)		void 	kbd_handler(void)
#endif

/**********************************************************************
 *	FIFO�̃T�C�Y.
 **********************************************************************
 */
#define	BUFF_SIZE 		32			//�Q�ׂ̂�.
#define	BUFF_SIZE_MASK	(BUFF_SIZE-1)

#define	PREFIX_E0		0x1
#define	PREFIX_E1		0x2			// [Pause]=E1 14,77,E1 14,F0 77
#define	PREFIX_F0		0x4

#define	KBD_SHIFT		0x1
#define	KBD_CTRL		0x2



/**********************************************************************
 *	���[�N�G���A.
 **********************************************************************
 */
static	uchar bitcount;				//	0..10 : start=0,data[8],oddparity,stop=1
static	uchar inptr, outptr;		// FIFO �̎��o���A�������݃|�C���^.
static	uchar kbd_buffer[BUFF_SIZE];// FIFO.
static  uchar kbd_data;				// Holds the received	scan code
static  uchar kbd_shift;			// Holds shift , ctrl , alt status
static  uchar prefix;				// Holds last scan code prefix

#if	PS2KBD_USE_PRESSTABLE	
static	uchar kbd_stat[32];			// 128�̃L�[�̉������ (bit table).
#endif
/**********************************************************************
 *	ps2kbd : ������
 **********************************************************************
 */
void kbd_init(void)
{
#if	PS2KBD_USE_INTERRUPT		// use PCINT8
	//�|�[�g�̏�����.
	PCICR  = 1<<1;	// PCINT1 (8..14) Enable.
	PCMSK1 = 1<<0;	// PCINT8 Mask off.
#endif

	//�����ϐ��̏�����.
	inptr  =	0;
	outptr =	0;
	bitcount = 11;
	prefix =	0;
	kbd_shift =	0;
}

#if	PS2KBD_USE_PRESSTABLE	
/**********************************************************************
 *	ps2kbd : 128�̃L�[�̉�������mask bit����.
 **********************************************************************
 */
static uchar get_press_mask(uchar c)
{
	return 1 << (c & 7);
}
static uchar get_press_idx(uchar c)
{
	return (c >> 3);
}

/**********************************************************************
 *	ps2kbd : 128�̃L�[�̉������ �̃N���A.
 **********************************************************************
 */
void clr_press_table(void)
{
	uchar i;
	for(i=0;i<16;i++) {
		kbd_stat[i] = 0;
	}
}
/**********************************************************************
 *	ps2kbd : 128�̃L�[�̉������ �̍X�V.
 **********************************************************************
 */
static void set_press_table(uchar c,uchar pre)
{
	uchar idx  = get_press_idx(c);
	uchar mask = get_press_mask(c);
	if(pre & PREFIX_F0) {	//�����ꂽ���,�g���V�[�P���X�͖�������.
		kbd_stat[idx] &= ~mask;
	}else{
		kbd_stat[idx] |=  mask;
	}
}

/**********************************************************************
 *	ps2kbd : 128�̃L�[�̉������ �̎擾.
 **********************************************************************
 *	����  c : �X�L�����R�[�h.
 *	�߂�l  : ��[���̏ꍇ�A���̃X�L�����R�[�h�̃L�[�́u�����ꂽ��ԁv�ɂ���.
 *			  �[���̏ꍇ�A���̃X�L�����R�[�h�̃L�[�́u�����ꂽ��ԁv�ɂ���.
 */
uchar kbd_getpress(uchar c)
{
	uchar idx  = get_press_idx(c);
	uchar mask = get_press_mask(c);
	return kbd_stat[idx] & mask;
}
#else

#endif
/**********************************************************************
 *	ps2kbd : FIFO�L���[�̑���.
 **********************************************************************
 */
static	void _MIPS32 put_que(uchar c)
{
	uchar p = inptr;

	if(c==0) return;			// 0 ��FIFO�ɏ������܂Ȃ�.

	kbd_buffer[p]=c;
	p++; p &= BUFF_SIZE_MASK;	// ���� p ��i�߂Ă݂�.

	if(p != outptr) {	// ���� p �����o���|�C���^�ɒǂ����Ă��Ȃ��ꍇ�̂�,
		inptr = p;		// �|�C���^���X�V����.
	}
}

/**********************************************************************
 *	ps2kbd : FIFO�L���[��TOP������.
 **********************************************************************
 */
uchar kbd_peekcode(void)
{
	uchar p = outptr;
	if(p == inptr) {			//�L���[����Ɣ��f����.
		return 0;				//���͂Ȃ�.
	}
	return kbd_buffer[p];		//�L���[TOP�����邾��.
}

/**********************************************************************
 *	ps2kbd : FIFO�L���[�̎��o������.
 **********************************************************************
 *	�L�[���͂��Ȃ��Ƃ��́A0��Ԃ�.
 *	�L�[���͂��������Ƃ��́A�X�L�����R�[�h(��[��)��1�o�C�g�Ԃ�.
 *���ӁF
 *	ASCII�R�[�h�݂̂��擾����ꍇ�́Akbd_getcode()����،Ă΂��ɁA
 *	kbd_getchar()�̂ق����g�p����B
 *	���݂��ČĂяo�����ꍇ�Akbd_getchar()�̓��삪���������Ȃ�B
 */
uchar kbd_getcode(void)
{
	uchar c;
	uchar p = outptr;
	if(p == inptr) {			//�L���[����Ɣ��f����.
		return 0;				//���͂Ȃ�.
	}
	c = kbd_buffer[p];			//�L���[������o��.
	p++; p &= BUFF_SIZE_MASK;	// ���� p ��i�߂Ă݂�.
	outptr = p;
	return c;
}

#if	PS2KBD_USE_GETCHAR
/**********************************************************************
 *	ps2kbd : getchar ASCII �R�[�h���擾.
 **********************************************************************
 *	�L�[���͂��Ȃ��Ƃ��́A0��Ԃ�.
 *	�L�[�u���[�N(�����ꂽ)�O�u�R�[�h(0xF0)��g���V�[�P���X(0xE0)
 *	���擾�����ꍇ�́A prefix�Ƃ����X�^�e�B�b�N�ϐ��ɂ��̃t���O��
 *	�c�����A�߂�l�� 0 �ɂȂ�.�i���̃R�[�h�擾���Ƀt���O���Q�Ƃ����j
 */
uchar kbd_getchar(void)
{
	int pre = prefix;			//��O�̃R�[�h���g���R�[�h�������Ă���?
	int c = kbd_getcode();
	int idx;

	if(c==0) return c;			//���͖���.

	// �Q�o�C�g�ȏ�̃V�[�P���X������ꍇ�́A�O�u�R�[�h�������ŏ���:
	if(c == 0xe0) {				//�g���V�[�P���X E0.
		prefix |= PREFIX_E0;
		return 0;
	}else if(c == 0xe1) {		//�g���V�[�P���X E1.
		prefix |= PREFIX_E1;
		return 0;
	}else if(c == 0xf0) {		//�����ꂽ��񂪌㑱.
		prefix |= PREFIX_F0;
		return 0;
	}

	// �Q�o�C�g�ȏ�̃V�[�P���X�Ȃ�A�Ō�̂P�o�C�g������:
	// �������͒P���L�[�̏ꍇ�������ɗ���:

	prefix = 0;
	// [SHIFT] ��Ԃ̃t���O�X�V.
	if((c == SKEY_LSHIFT)) {	//||(c == SKEY_RSHIFT)) {
		if(pre & PREFIX_F0) {
			kbd_shift &= ~KBD_SHIFT;
		}else{
			kbd_shift |=  KBD_SHIFT;
		}
	}
	// [CTRL] ��Ԃ̃t���O�X�V.
	if((c == SKEY_CTRL)){
		if(pre & PREFIX_F0) {
			kbd_shift &= ~KBD_CTRL;
		}else{
			kbd_shift |=  KBD_CTRL;
		}
	}
	set_press_table(c,pre);

	if(pre & PREFIX_E0) {	//�g���V�[�P���X��SCAN CODE��MSB�𗧂Ăċ�ʂ���.
		c += SKEY_EXTEND;
	}

	if(pre & PREFIX_F0) {	//�����ꂽ���͖�������.
		return 0;
	}

	idx = c*2;
	if(kbd_shift & KBD_SHIFT) idx++;	// [SHIFT]��������Ă���?

	// SCAN CODE --> ASCII CODE
	c = keycode_table[idx];
	if(kbd_shift & KBD_CTRL) {
		if((c>='A')&&(c<='z')) {
			c = c & 0x1f;			// Ctrl+A �` Z��.
		}
	}
	return c;
}
#endif
/**********************************************************************
 *	ps2kbd : �n���h���[.
 **********************************************************************
 */
void _MIPS32 kbd_handler(void)
{
	ushort edge = PS2KBD_PIN & PS2KBD_CLOCK_MASK;
											//	start and stop bits	are	ignored.
	if (!edge) {							// Routine entered at	falling	edge
		if(bitcount	< 11 &&	bitcount > 2) {	//	Bit	3 to 10	is data. Parity	bit,
			kbd_data >>= 1;
			if(PS2KBD_PIN & PS2KBD_DATA_MASK) {
				kbd_data |= 0x80;			// Store	a '1'
			}
		}
	} else {								// Routine entered at rising edge
		if(--bitcount == 0)	{				// All bits received
			put_que(kbd_data);
			bitcount = 11;
		}
	}
}

/**********************************************************************
 *	PCINT8 ��TIMER1 ���荞�݂� �G�~�����[�g�i��p�j����.
 **********************************************************************
 */
void _MIPS32 kbd_int_handler(void)
{
	static ushort clk_last = PS2KBD_CLOCK_MASK;	// �����l = H

	ushort clk_edge = PS2KBD_PIN & PS2KBD_CLOCK_MASK;	//���ݒl KBD_CLK

	// KBD_CLK�̓d�ʂɕω�����������?.(�O��ƈقȂ��Ă��邩)
	if(	clk_last != clk_edge ) {
		clk_last  = clk_edge;
		kbd_handler();					// PCINT8 �̃G���g���[���R�[������.
	}
}
/**********************************************************************
 *	
 **********************************************************************
 */
