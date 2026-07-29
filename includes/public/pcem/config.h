#ifndef _PCEM_CONFIG_H_
#define _PCEM_CONFIG_H_

#include <pcem/defines.h>
#include <string_view>

#define CFG_MACHINE 0
#define CFG_GLOBAL 1

extern float config_get_float(int is_global, const char *head, std::string_view name, float def);
extern int config_get_int(int is_global, const char *head, std::string_view name, int def);
extern const char *config_get_string(int is_global, const char *head, std::string_view name, const char *def);
extern void config_set_float(int is_global, const char *head, std::string_view name, float val);
extern void config_set_int(int is_global, const char *head, std::string_view name, int val);
extern void config_set_string(int is_global, const char *head, std::string_view name, std::string_view val);
extern int config_free_section(int is_global, const char *head);
extern void add_config_callback(void (*loadconfig)(), void (*saveconfig)(), void (*onloaded)());

#endif /* _PCEM_CONFIG_H_ */
