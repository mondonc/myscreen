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

#include "network.h"

/**
 * Display upload and download rate 
 */

/* Gateway */
static char gw[GW_SIZE];

/*To memorize old values of up/down rate */
static unsigned long up;
static unsigned long down;

static char conf_line[LINE_SIZE];

/*Network interface name*/
static char interface[INTERFACE_SIZE+1];/*Name of current interface*/
static char no_activity[INTERFACE_SIZE+17+1+1]; /*Name of current interface with " No activity on " and a final space */

/*Refresh interface when no_activity > REFRESH_TIME */
static short no_activity_count;

extern char line[];
static char network_result[NETWORK_RESULT_SIZE]; /*Returned result */

/*static short first_init = TRUE;*/


/*
** move 'line' pointer to the next word of the line, returning it
*/
static const char *jump_separators(const char *line) {

  while (*line && strchr(PROC_FILES_SEPARATORS_CHARS, *line))
    line++;
  return line;
}

/*
** move 'line' pointer to the next sepatator_char of the line, returning it
*/
static const char *jump_info(const char *line) {

  while (strchr(PROC_FILES_SEPARATORS_CHARS, *line) == NULL)
    line++;
  return line;
}

/*
** copy at most 'size' characters of the line in buffer.
** including null byte at the end of 'buffer'
** return the moved 'line' pointer, at the first separator character 
*/
static const char *copy_info(const char *line, char *buffer, size_t size) {

  size_t i;

  /* if 'line' is pointing to the end */
  IFDEBUG(if (*line == '\0')
	    fprintf(stderr, "Error : network: missing informations\n"));
  

  /* copy the word-info */
  for (i = 0; i < size - 1 && strchr(PROC_FILES_SEPARATORS_CHARS, line[i]) == NULL; i++)
    buffer[i] = line[i];
  buffer[i] = '\0';
  line += i;
  
  /* if buffer is too small to copy the entire word */
  if (i >= size - 1 && strchr(PROC_FILES_SEPARATORS_CHARS, line[size - i - 1]) == NULL) {
    IFDEBUG(fprintf(stderr, "Error : network: too long word size for '%s[...]'\n", buffer));
    /* move 'line' to the end of the word */
    while (strchr(PROC_FILES_SEPARATORS_CHARS, *line) == NULL)
      line++;
  }

  return line;
}


static int sscan_net_dev(const char *line, unsigned long *down, unsigned long *up) {

  int i;

  /* jump interface name */
  line = jump_separators(line);
  while (*line && *line != ':')
    line++;
  if (*line == ':')
    line++;

  /* get down */
  line = jump_separators(line);
  *down = strtoul(line, NULL, 10);
  line = jump_info(line);

  /* jump 7 infos */
  for (i = 0; i < 7; i++) {
    line = jump_separators(line);
    line = jump_info(line);
  }

  /* get up */
  line = jump_separators(line);
  *up = strtoul(line, NULL, 10);

  return 0;
}

static int sscan_net_route(const char *line, char *interface, char *gw) {

  /* get interface */
  line = jump_separators(line);
  line = copy_info(line, interface, PROC_NET_INTERFACE_SIZE);

  /* jump 1 info */
  line = jump_separators(line);
  line = jump_info(line);

  /* get gw */
  line = jump_separators(line);
  line = copy_info(line, gw, GW_SIZE);

  return 0;
}

/*Upload up and down values*/
static int update_up_down(unsigned long * up, unsigned long * down){

	FILE * f;
	short flag;

	flag=FALSE;

	/*Get recent values*/	
	if ( (f=fopen(PROC_NET_DEV, "r")) ){

	  /* jump two lines (header) */
	  (void)(fgets(line, LINE_SIZE, f));
	  (void)(fgets(line, LINE_SIZE, f));
	  
		while ((fgets(line, LINE_SIZE, f)) != NULL){
			if (strstr(line, interface) != NULL){
			  sscan_net_dev(line, down, up);
			  flag=TRUE;
			  break;
			}
		}
		if (fclose(f) == EOF){
			perror("Closing proc network file ");
		}
		return flag;
	}
	IFDEBUG_PRINT("Can't open network file");
	return flag;
}


/*NETWORK UP/DOWN RATE*/
char * network(){

	/*Recent Values*/
	unsigned long  _up;
	unsigned long _down;

	/*Units*/
	char * u_unit;
	char * d_unit;

	/*Init*/
	u_unit=" o/s";
	d_unit=" o/s";
	network_result[0]='\0';

	if (*interface && update_up_down(&_up, &_down) == TRUE ){

		/*Rates*/
		float u_rate;
		float d_rate;

		/* If rate == zero*/
		if ( up==_up && down==_down ){

			/* Update interface */
			if (no_activity_count > REFRESH_TIME){
				return init_network(conf_line);	
			} else {
				no_activity_count++;
				return no_activity;
			}
		} else {
			no_activity_count=0;
		}

		/*Calculate Rate*/
		u_rate= (float)( _up - up ) / TIME;
		d_rate= (float)( _down - down ) / TIME;

		/*Save recent values*/
		up=_up;
		down=_down;

		/*Ajust units*/
		if (d_rate>999) {
			d_rate/=1024;
			if (d_rate>999) {
				d_rate/=1024;
				d_unit="Mo/s";
			} else {
				d_unit="Ko/s";
			}
		}
		if (u_rate>999) {
			u_rate/=1024;
			if (u_rate>999) {
				u_rate/=1024;
				u_unit="Mo/s";
			} else {
				u_unit="Ko/s";
			}
		}

		assert(NETWORK_RESULT_SIZE >= 4+ 5*2 +1);
		(void)snprintf(network_result, NETWORK_RESULT_SIZE, "v%5.1f%s ^%5.1f%s ", d_rate, d_unit, u_rate, u_unit);
	} else {
		/*Unkown values, updating interface*/
		/* Update interface */
	        IFDEBUG_PRINT(*interface ? "Can't read values of network file" : "no network found");
		if (no_activity_count > REFRESH_TIME){
			return init_network(conf_line);	
		} else {
			no_activity_count++;
			return "no network found ";
		}

	}
	return network_result;
}

char * init_network(char * confline){

	FILE * f;
	short flag;
	/*INIT OLD RATES*/
	up=0;
	down=0;
	flag=FALSE;

	IFDEBUG_PRINT("Init network interface: ");
	/* Save conf_line */
	strncpyclr(conf_line, confline, LINE_SIZE);

	/* If AUTO option */
	if ( strstr(conf_line, OPT_AUTO) != NULL || *conf_line == '\0' ) {

		/*Get interface*/
		if ( (f=fopen(PROC_NET_ROUTE, "r")) != NULL){
			/*First line ignored*/
			(void)fgets(line, LINE_SIZE, f);
			while ( flag == FALSE && (fgets(line, LINE_SIZE, f)) != NULL){
			  sscan_net_route(line, interface, gw);
			  if (strcmp(gw, "00000000") != 0){
			    flag=TRUE;
			  }	
			}
			if (fclose(f) == EOF){
				perror("Closing proc network file ");
			}
		}

		/* Build result string */
		if (*interface != '\0') {
			strcpy(network_result,"selected device: ");
		} else {
			strcpy(network_result,"no network found ");
		}

	} else {
		strncpy(interface, conf_line, INTERFACE_SIZE); 
		/* Build result string */
		strcpy(network_result," configured to : ");
	}
	no_activity_count=0;
	/* Build "no activity" string */
	assert(strlen(" no activity on ")+INTERFACE_SIZE+2 <= NETWORK_RESULT_SIZE);
	strcpy(no_activity," no activity on ");
	strcat(no_activity,interface);
	strcat(no_activity, " ");

	assert(strlen("selected device: ")+INTERFACE_SIZE+2 <= NETWORK_RESULT_SIZE);
	assert(strlen(" configured to : ")+INTERFACE_SIZE+2 <= NETWORK_RESULT_SIZE);
	assert(strlen("no network found ")+INTERFACE_SIZE+2 <= NETWORK_RESULT_SIZE);

	/* Build result string */
	strcat(network_result,interface);
	strcat(network_result, " ");


	/*if (first_init){*/
		(void)update_up_down(&up, &down);
		/*first_init = FALSE;
	}*/
	return network_result;
}

void exit_network(){

}
