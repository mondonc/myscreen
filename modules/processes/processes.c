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

#include "processes.h"


extern struct sysinfo si;

/**
 * Display number of processes run at this time
 */

static char processes_result[PROCESSES_RESULT_SIZE]; /*Returned result */

/*NUMBER OF PROCESSES*/
char * processes(){

	assert(PROCESSES_RESULT_SIZE>2+4+1);
	processes_result[0]='\0';
	(void) snprintf(processes_result, PROCESSES_RESULT_SIZE, "%hu& ", si.procs);
	return processes_result;
}	

char * init_processes(char * UNUSED(conf_line)){

	return "Procs& ";
}

void exit_processes(){

}
