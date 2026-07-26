#ifndef _PLAT_KEYBOARD_H_
#define _PLAT_KEYBOARD_H_

#include <stdint.h>

void keyboard_init();
void keyboard_close();
void keyboard_poll_host();
extern int rawinputkey[272];

#ifndef __unix
#define KEY_LCONTROL 0x1d
#define KEY_RCONTROL (0x1d | 0x80)
#define KEY_END (0x4f | 0x80)
#endif

#endif /* _PLAT_KEYBOARD_H_ */
