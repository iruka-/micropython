#ifndef _UTIL_H_
#define _UTIL_H_

#define	_MIPS32 __attribute__((nomips16))

void    led_on();
void    led_off();
void	led_flip();

void	led_test();
void	led_blink();

//	�R���\�[��(UART/NTSC)���� �����o��.
void user_putc(char c);
void user_puts(const char *s);
void user_write_console(const char *s,int len);
//	�o�̓��[�h: UART�ɂ������o�͂���(1)�����Ȃ�(0)�����w�肷��.
int  user_stdout_mode(int f);

#endif

