/*Copyright 2009,2010,2011 Clément Mondon <clement.mondon@gmail.com>

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

#include "wifi.h"

/*
** Display wifi quality 
*/

extern char line[];
static char wifi_result[WIFI_RESULT_SIZE]; /* Returned result */

static int sscan_net_wireless(const char *line, int *power) {

  /* jump interface name */
  line = jump_separators(line);
  while (*line && *line != ':')
    line++;
  if (*line == ':')
    line++;

  /* jump 1 info */
  line = jump_separators(line);
  line = jump_info(line);

  /* get power */
  *power = atoi(line);

  return 0;
}

/* WIFI */
char * wifi() {

  /* Variables */
  FILE * f;
  int power;
  char * ptr;
  int cpt;
  
  /* Init */
  wifi_result[0]='\0';
  f = fopen(PROC_WIRELESS, "r");
  if (f == NULL) {
    IFDEBUG_PRINT("Can't open wifi file");
    return wifi_result;
  }
  
  ptr = wifi_result;
  
  cpt = 0;
  
  /* ignore two first line (header) */
  (void) (fgets(line, LINE_SIZE, f));
  (void) (fgets(line, LINE_SIZE, f));
  
  /* For each interface */
  while ((fgets(line, LINE_SIZE, f)) != NULL && cpt < NB_WIFI_INTERFACE) {
    if (strstr(line, ":") != NULL){
      sscan_net_wireless(line, &power);
      assert(ptr-wifi_result+5 < WIFI_RESULT_SIZE);
      ptr += myprint_percentage_s(ptr, power);
      cpt++;
    }
  }

  if (fclose(f) == EOF){
    perror("Closing proc Wifi file ");
  }

  return wifi_result;
}

char * init_wifi(char * UNUSED(conf_line)){

  wifi_result[0] = '\0';

  return "Wifi ";
}

void exit_wifi(){

}
