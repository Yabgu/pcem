#ifndef _PLAT_MOUSE_H_
#define _PLAT_MOUSE_H_


void mouse_init();
void mouse_close();
extern int mouse_buttons;
void mouse_poll_host();
void mouse_get_mickeys(int *x, int *y, int *z);
extern int mousecapture;

#endif /* _PLAT_MOUSE_H_ */
