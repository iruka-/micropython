#ifndef _ps2keyb_h_
#define _ps2keyb_h_

#include "utype.h"

void  kbd_init(void);			// : ������
uchar kbd_getchar(void);		// : ASCII �R�[�h���擾.
uchar kbd_peekcode(void);		// : FIFO�L���[��TOP������.
uchar kbd_getcode(void);		// : FIFO�L���[�̎��o������.
uchar kbd_getpress(uchar c);	// : ���A���^�C�� �C�ӃL�[�̉������ �̎擾.

void  _MIPS32 kbd_int_handler(void);

#endif	//_ps2keyb_h_
