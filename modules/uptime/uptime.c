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

#include "uptime.h"


extern struct sysinfo si;

/**
 * Display uptime
 */

static char uptime_result[UPTIME_RESULT_SIZE]; /*Returned result */

/*UPTIME*/
char * uptime(){

	assert(UPTIME_RESULT_SIZE >= 3+1+2+1+1+1);

	/*Change units*/
	if (si.uptime > 86400){ 

		/*Day*/
		(void) snprintf(uptime_result, UPTIME_RESULT_SIZE, "%ldd%02ldh ", (si.uptime/86400), ((si.uptime%86400)/3600)); 

	} else if (si.uptime > 3600) { 

		/*Hour*/
		(void) snprintf(uptime_result, UPTIME_RESULT_SIZE, "%02ldh%02ldm ", (si.uptime/3600), ((si.uptime%3600)/60)); 

	} else if (si.uptime > 60) { 

		/*Minute*/
		(void) snprintf(uptime_result, UPTIME_RESULT_SIZE, "%02ldm%02lds ", (si.uptime/60), (si.uptime%60)); 		

	} else {
		uptime_result[0]='\0';
	}

	return uptime_result;
}

char * init_uptime(char * UNUSED(conf_line)){

	return "Uptime ";
}

void exit_uptime(){

}
