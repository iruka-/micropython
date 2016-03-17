//
//	�X�L�����R�[�h�̒�`.
//

#define SKEY_EXTEND        	0x80	/* �g���L�[�V�[�P���X(E0,xx) �� MSB On �ŋ�ʂ��� */
									/* MSB On�ŕW���V�[�P���X�ȃL�[([F7])�����݂��邪,*/
									/* (E0,03)�Ƃ����g���V�[�P���X�͑��݂��Ȃ��̂ŋ�ʂ͉\ */
//�ŉ��i.
#define SKEY_CTRL 	     	0x14	/* [CTRL] */
#define SKEY_LWIN	 	   	(0x1f|SKEY_EXTEND)	/* Left Windows[���L�[] */
#define SKEY_ALT 	     	0x11	/* [ALT] */
#define SKEY_NFER        	0x67	/* ���ϊ� */
#define SKEY_SPACE        	0x29	/* SPACE */
#define SKEY_XFER        	0x64	/* �ϊ� */
#define SKEY_KANA        	0x13	/* �Ђ炪��/�J�^�J�i */
// Right Control/Alt.
#define SKEY_RALT 	     	(0x11|SKEY_EXTEND)	/* [ALT] */
#define SKEY_RWIN 		   	(0x27|SKEY_EXTEND)	/* Right Windows [���L�[] */
#define SKEY_APPS	 	   	(0x2f|SKEY_EXTEND)	/* AppMenu [��START�L�[] */
#define SKEY_RCTRL 	     	(0x14|SKEY_EXTEND)	/* [CTRL] */

//LARGE KEY
#define SKEY_BS            	0x66	/* BACKSPACE */
#define SKEY_TAB          	0x0d	/* TAB */
#define SKEY_CAPS 	     	0x58	/* [CAPS] */
#define SKEY_RETURN       	0x5a	/* ENTER */
//Shift
#define SKEY_LSHIFT      	0x12	/* [SHIFT] */
#define SKEY_RSHIFT      	0x59	/* [SHIFT] */

#ifndef	SKEY_AT101			//SKEY_AT101:	�p��=defined  ���{��=not defined
//
// ========= 106-JP ==========
//���i0
#define SKEY_COMMA	     	0x41	/* , < */
#define SKEY_PERIOD        	0x49	/* . > */
#define SKEY_SLASH          0x4a	/* / ? */
#define SKEY_UNDERLINE      0x59	/* �__�� */

//���i1
#define SKEY_SEMICOLON     	0x4c	/* ; + */
#define SKEY_COLON  	   	0x52	/* : * */
#define SKEY_RBRACKET      	0x5b	/* ] } */

//���i2
#define SKEY_ATMARK		   	0x54	/* @ ' */
#define SKEY_LBRACKET     	0x5d	/* [ { */

//��i3
#define SKEY_GRAVE		   	0x0e	/* ~ (���p/�S�p) ��ԍ��[�̃L�[ */
#define SKEY_MINUS         	0x4e	/* - = */
#define SKEY_PREVTRACK	   	0x55	/* ^ ~ (CurcumFlex) ���{��L�[�̂� */
#define SKEY_YEN	    	0x6a	/* ��| */

#else
//
// ========= 101-AT ==========
//���i0
#define SKEY_COMMA	     	0x41	/* , < */
#define SKEY_PERIOD        	0x49	/* . > */
#define SKEY_SLASH          0x4a	/* / ? */

//���i1
#define SKEY_SEMICOLON     	0x4c	/* ; : */
#define SKEY_APOSTROPHE  	0x52	/* ' " */

//���i2
#define SKEY_LBRACKET	   	0x54	/* [ { */
#define SKEY_RBRACKET     	0x5b	/* ] } */

//��i3
#define SKEY_GRAVE		   	0x0e	/* ~ (hankaku/zenkaku) */
#define SKEY_MINUS         	0x4e	/* - _ */
#define SKEY_EQUAL		   	0x55	/* = + */
#define SKEY_BACKSLASH   	0x5d	/* \ | (BackSlash�̏ꏊ���قȂ鐻�i�����݂���)*/

#endif

//Cursor
#define SKEY_UP	 	 	  	(0x75|SKEY_EXTEND)	/* �� */
#define SKEY_LEFT	    	(0x6b|SKEY_EXTEND)	/* �� */
#define SKEY_RIGHT	    	(0x74|SKEY_EXTEND)	/* �� */
#define SKEY_DOWN	    	(0x72|SKEY_EXTEND)	/* �� */
//RollUp/Down
#define SKEY_PGUP	 	 	(0x7d|SKEY_EXTEND)	/* PGUP	 */
#define SKEY_PGDOWN	    	(0x7a|SKEY_EXTEND)	/* PGDOWN*/
#define SKEY_INS	 	 	(0x70|SKEY_EXTEND)	/* INS	 */
#define SKEY_DEL	    	(0x71|SKEY_EXTEND)	/* DEL	 */
#define SKEY_HOME	 	 	(0x6c|SKEY_EXTEND)	/* HOME	 */
#define SKEY_END	    	(0x69|SKEY_EXTEND)	/* END	 */

//PRINT SCREEN/SCROLL LOCK/BREAK
#define SKEY_SYSRQ		  	(0x7c|SKEY_EXTEND)	/* PrintScreen/SySRq	 */
#define SKEY_SCROLLLOCK  	0x7e	/* Scroll Lock */
//	PAUSE/BREAK�̓V�[�P���X�������̂ŁA��`���Ă��Ȃ�.
//		SKEY_BREAK			E1 14 77 E1 14 F0 77	// 77��NUMLOCK�ƌ��Ȃ����.
													// 14��CTRL�ƌ��Ȃ���邩������Ȃ�.
//	PAUSE/BREAK�L�[�͗����ꂽ�Ƃ��R�[�h���o�͂��Ȃ�.

// POWER CONTROL
#define SKEY_POWER	    	(0x37|SKEY_EXTEND)	/* [POWER] */
#define SKEY_SLEEP	    	(0x3f|SKEY_EXTEND)	/* [SLEEP] */
#define SKEY_WAKEUP	    	(0x5e|SKEY_EXTEND)	/* [WAKE] (0x78�̂��̂�����)*/



#define SKEY_ESCAPE       	0x76	/* ESCAPE */
#define SKEY_F1 	     	0x05	/* [F1 ] */
#define SKEY_F2 	     	0x06	/* [F2 ] */
#define SKEY_F3 	     	0x04	/* [F3 ] */
#define SKEY_F4 	     	0x0c	/* [F4 ] */
#define SKEY_F5 	     	0x03	/* [F5 ] */
#define SKEY_F6 	     	0x0b	/* [F6 ] */
#define SKEY_F7 	     	0x83	/* [F7 ] */
#define SKEY_F8 	     	0x0a	/* [F8 ] */
#define SKEY_F9 	     	0x01	/* [F9 ] */
#define SKEY_F10 	     	0x09	/* [F10] */
#define SKEY_F11 	     	0x78	/* [F11] */
#define SKEY_F12 	     	0x07	/* [F12] */

#define SKEY_NUMPAD0        0x70	/* 0 */
#define SKEY_NUMPAD1        0x69	/* 1 */
#define SKEY_NUMPAD2        0x72	/* 2 */
#define SKEY_NUMPAD3        0x7a	/* 3 */
#define SKEY_NUMPAD4        0x6b	/* 4 */
#define SKEY_NUMPAD5        0x73	/* 5 */
#define SKEY_NUMPAD6        0x74	/* 6 */
#define SKEY_NUMPAD7        0x6c	/* 7 */
#define SKEY_NUMPAD8        0x75	/* 8 */
#define SKEY_NUMPAD9        0x7d	/* 9 */
#define SKEY_NUMPADMINUS    0x7b	/* - */
#define SKEY_NUMPADCOMMA    0x71	/* , */
#define SKEY_NUMPADPLUS     0x79	/* + */
#define SKEY_NUMPADSTAR    	0x7c	/* * */
#define SKEY_NUMPADENTER    (0x5a|SKEY_EXTEND)	/* ENTER */
#define SKEY_NUMPADSLASH    (0x4a|SKEY_EXTEND)	/* '/' */
#define SKEY_NUMLOCK    	0x77	/* NumLock */

#define SKEY_0             	0x45	/* 0 */
#define SKEY_1             	0x16	/* 1 */
#define SKEY_2             	0x1e	/* 2 */
#define SKEY_3             	0x26	/* 3 */
#define SKEY_4             	0x25	/* 4 */
#define SKEY_5             	0x2e	/* 5 */
#define SKEY_6             	0x36	/* 6 */
#define SKEY_7             	0x3d	/* 7 */
#define SKEY_8             	0x3e	/* 8 */
#define SKEY_9             	0x46	/* 9 */

#define SKEY_A             	0x1c	/* a */
#define SKEY_B             	0x32	/* b */
#define SKEY_C             	0x21	/* c */
#define SKEY_D             	0x23	/* d */
#define SKEY_E             	0x24	/* e */
#define SKEY_F             	0x2b	/* f */
#define SKEY_G             	0x34	/* g */
#define SKEY_H             	0x33	/* h */
#define SKEY_I             	0x43	/* i */
#define SKEY_J             	0x3b	/* j */
#define SKEY_K             	0x42	/* k */
#define SKEY_L             	0x4b	/* l */
#define SKEY_M             	0x3a	/* m */
#define SKEY_N             	0x31	/* n */
#define SKEY_O             	0x44	/* o */
#define SKEY_P             	0x4d	/* p */
#define SKEY_Q             	0x15	/* q */
#define SKEY_R             	0x2d	/* r */
#define SKEY_S             	0x1b	/* s */
#define SKEY_T             	0x2c	/* t */
#define SKEY_U             	0x3c	/* u */
#define SKEY_V             	0x2a	/* v */
#define SKEY_W             	0x1d	/* w */
#define SKEY_X             	0x22	/* x */
#define SKEY_Y             	0x35	/* y */
#define SKEY_Z             	0x1a	/* z */

