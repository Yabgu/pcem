#ifndef __LPT_EPSONLX810_H__
#define __LPT_EPSONLX810_H__

#include <pcem/devices.h>

extern lpt_device_t lpt_epsonprinter_device;
extern char printer_path[512];

int wx_image_save_fullpath(const char *fullpath, const char *format, unsigned char *rgba, int width, int height, int alpha);

#define IMAGE_JPG "jpg"
#define IMAGE_PNG "png"
#define IMAGE_BMP "bmp"
#define IMAGE_TIFF "tiff"

#endif /* __LPT_EPSONLX810_H__ */