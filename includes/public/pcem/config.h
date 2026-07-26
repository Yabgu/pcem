#ifndef _PCEM_CONFIG_H_
#define _PCEM_CONFIG_H_

#include <pcem/defines.h>

#define CFG_MACHINE 0
#define CFG_GLOBAL 1

extern float config_get_float(int is_global, const char *head, const char *name, float def);
extern int config_get_int(int is_global, const char *head, const char *name, int def);
extern const char *config_get_string(int is_global, const char *head, const char *name, const char *def);
extern void config_set_float(int is_global, const char *head, const char *name, float val);
extern void config_set_int(int is_global, const char *head, const char *name, int val);
extern void config_set_string(int is_global, const char *head, const char *name, const char *val);
extern int config_free_section(int is_global, const char *head);
extern void add_config_callback(void (*loadconfig)(), void (*saveconfig)(), void (*onloaded)());

#endif /* _PCEM_CONFIG_H_ */
