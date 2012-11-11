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

#include "loadaverage.h"

extern struct sysinfo si;

/**
 * Display loadaverage 
 */

static int config;

static char loadaverage_result[LOADAVERAGE_RESULT_SIZE]; /*Returned result */


/*LOADAVERAGE*/
char * loadaverage(){
	loadaverage_result[0]='\0';
	(void)snprintf(loadaverage_result, LOADAVERAGE_RESULT_SIZE, "%1.2f ", ((float)si.loads[0])/(float)(1<<SI_LOAD_SHIFT));
	return loadaverage_result;
}

char * init_loadaverage(char * conf_line){
	
	int int_conf_line;
	
	int_conf_line = 0;
	config=0;
	
	sscanf(conf_line, "%d", &int_conf_line);

	if (int_conf_line == 5){
		int_conf_line = 1;
		return "Loadaverage 5mn ";
	} else if (int_conf_line == 15){
		int_conf_line = 2;
		return "Loadaverage 15mn ";
	} else {
		int_conf_line = 0;
		return "Loadaverage 1mn ";
	}
	
	return "Loadaverage 1mn ";
}

void exit_loadaverage(){

}
