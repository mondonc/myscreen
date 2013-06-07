#ifndef  _PARSE_CONFIG_H
#define  _PARSE_CONFIG_H

#include "myscreen-stats.h"

#define CHAR_CONF_COMMENT '#'

#define HOME "HOME"

#define CONF_LOCAL_FILE "./myscreenrc"
#define CONF_HOME_FILE "/.myscreenrc"
#define CONF_ETC_FILE "/etc/myscreenrc"

void get_configuration(); /* Read configuration file, complete current_conf[] */

char * line_reader(char *line, int *module);

#endif
