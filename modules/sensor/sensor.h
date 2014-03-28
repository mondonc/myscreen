/*Copyright 2014 Barthélémy Delemotte <barthelemy.delemotte@gmail.com>

  This file is part of project myscreen.

  myscreen is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  myscreen is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with myscreen.  If not, see http://www.gnu.org/licenses/.

*/

#ifndef _SENSOR_H
# define _SENSOR_H

# include <stdbool.h>
# include <string.h>

# include <myscreen-stats.h>

# define COLOR_SENSOR MAGENTA BOLD
# define CORETEMP_PATH "/sys/class/hwmon/hwmon1/device/"
# define TEMP_FILE_PREFIX "temp"
# define SENSOR_ERROR "could not read temperature"
# define MAX_LABEL_SIZE 256
# define PATHNAME_BUFFER_SIZE (sizeof(CORETEMP_PATH) + sizeof(TEMP_FILE_PREFIX)	\
			       + 1 + sizeof("_crit_alarm") + 1)

typedef	float	temp_t;

typedef		struct
{
  /* fixed values */
  char		label[MAX_LABEL_SIZE];
  unsigned int	sensor_num;
  temp_t	critical;

  /* variables */
  bool		is_init;
  temp_t	current;
  temp_t	high;
  bool		alarm;
}		temp_info_t;

char *sensor();
char *init_sensor(char *line);
void exit_sensor();

#endif /* !_SENSOR_H */
