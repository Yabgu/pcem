#ifndef CDROM_ISO_H
#define CDROM_ISO_H

#include <filesystem>
#include <string>
#include <stdint.h>

extern std::string image_path;

int image_open(const std::filesystem::path &fn);
void image_reset();
void image_close();

void image_audio_callback(int16_t *output, int len);
void image_audio_stop();

#endif /* ! CDROM_ISO_H */
