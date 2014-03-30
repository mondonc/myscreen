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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "sensor.h"

extern char line[];
static temp_info_t temp_info;
static bool verbose = false;

/*
** build a coretemp file pathnmame.
** the string returned is stored in a data section and will be
** overwritten by the next call to this function
*/
static const char* make_pathname(unsigned int sensor_num, const char *suffix)
{
  static char pathname_buffer[PATHNAME_BUFFER_SIZE];
  snprintf(pathname_buffer, PATHNAME_BUFFER_SIZE, CORETEMP_PATH"/"TEMP_FILE_PREFIX"%d_%s",
	   sensor_num, suffix);
  return pathname_buffer;
}

/*
** Read an integer in the temp file identified with parameters
*/
static int read_value(unsigned sensor_num, const char *suffix)
{
  const char *pathname = make_pathname(sensor_num, suffix);
  FILE *f = fopen(pathname, "r");
  int value;

  if (!f)
    {
      IFDEBUG(perror(pathname));
      value = -1;
    }
  else
    {
      if (fscanf(f, "%8d", &value) == EOF && ferror(f))
	{
	  IFDEBUG(perror(pathname));
	  value = -1;
	}
      fclose(f);
    }
  return value;
}

/*
** Fill the field 'label' in temp_info
*/
static void get_label(void)
{
  const char *pathname = make_pathname(temp_info.sensor_num, "label");
  FILE *f = fopen(pathname, "r");

  if (!f)
    {
      IFDEBUG(perror(pathname));
      strncpy(temp_info.label, "[error]", MAX_LABEL_SIZE);
    }
  else
    {
      size_t ret = fread(temp_info.label, 1, MAX_LABEL_SIZE - 1, f);
      temp_info.label[ret] = '\0';
      if (ret > 0 && temp_info.label[ret - 1] == '\n')
	temp_info.label[ret - 1] = '\0';
      fclose(f);
    }
}

/*
** Read the temperature in a temp file and store it in 'ptr'
*/
static void get_temperature(temp_t *ptr, const char *suffix)
{
  int value = read_value(temp_info.sensor_num, suffix);
  *ptr = (temp_t)value / (temp_t)1000;
}

/*
** Check files accesses
*/
static int try_access_files(unsigned int sensor_num)
{
  if (access(make_pathname(sensor_num, "label"), R_OK) == -1 ||
      access(make_pathname(sensor_num, "crit"), R_OK) == -1 ||
      access(make_pathname(sensor_num, "input"), R_OK) == -1 ||
      access(make_pathname(sensor_num, "max"), R_OK) == -1 ||
      access(make_pathname(sensor_num, "crit_alarm"), R_OK) == -1)
     return -1;
  return 0;
}

/*
** update stats
*/
static void update(void)
{
  get_temperature(&temp_info.current, "input");
  get_temperature(&temp_info.high, "max");
  if (read_value(temp_info.sensor_num, "crit_alarm") == 1)
    temp_info.alarm = true;
  else
    temp_info.alarm = false;
}

char *sensor()
{
  if (!temp_info.is_init)
    return "";
  update();
  if (verbose)
    snprintf(line, LINE_SIZE, "%s%.1f°C (high=%.1f, crit=%.1f) ",
	     temp_info.alarm ? RED : "", temp_info.current,
	     temp_info.high, temp_info.critical);
  else
    snprintf(line, LINE_SIZE, "%s%.1f°C ",
	     temp_info.alarm ? RED : "", temp_info.current);
  return line;
}

char *init_sensor(char *confline)
{
  if (strstr(confline, "verbose"))
    verbose = true;
  memset(&temp_info, 0, sizeof(temp_info_t));
  if (try_access_files(1) == -1)
    {
      msg_queue_vpush("%ssensor: %scould not read temperature: %s",
		      WHITE BOLD, COLOR_SENSOR, strerror(errno));
      return "";
    }
  temp_info.sensor_num = 1;
  get_label();
  get_temperature(&temp_info.critical, "crit");
  temp_info.is_init = true;
  snprintf(line, LINE_SIZE, "%s ", temp_info.label);
  return line;
}

void exit_sensor()
{
  temp_info.is_init = false;
}
