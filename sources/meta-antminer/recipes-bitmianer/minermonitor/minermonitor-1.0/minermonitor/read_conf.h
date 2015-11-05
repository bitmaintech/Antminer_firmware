#ifndef _READCONF_FILE_H
#define _READCONF_FILE_H
#include <stdint.h>
#include <stdio.h>
#include <syslog.h>
#define    MAX_PATH_LEN         (512)
#define    MAX_FILE_NAME_LEN    (128)
char *conf_directory;
char *luci_conf_directory;
char *luci_cgminer_dir;
//////////////////////////////////////////////////transfer the items of configration by the following two functions
/////////////
/* After the configuration items in the configuration file parsing deposit into a global array */
int parse_luciconf_file(char *path_to_config_file);
int parse_myconf_file(char *path_to_config_file);
/* From the global reading a configuration items in the array。for example  get_config_var("var_name1");  */
char * get_config_var(char *var_name);
char *read_luci_conf(char *item);
char *read_conf(char *item);
char *read_cgminer_conf(char *item);
#endif
