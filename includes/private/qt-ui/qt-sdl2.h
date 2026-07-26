#ifndef _QT_SDL2_H_
#define _QT_SDL2_H_

void leave_fullscreen();
int getfile(void *hwnd, const char *f, const char *fn);
int getsfile(void *hwnd, const char *f, const char *fn, const char *dir, const char *ext);
int getfilewithcaption(void *hwnd, const char *f, const char *fn, const char *caption);
void screenshot_taken(unsigned char *rgb, int width, int height);


extern char openfilestring[260];

extern volatile int pause;

extern int take_screenshot;

#endif /* _QT_SDL2_H_ */
