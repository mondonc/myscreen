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

#include "ram.h"

extern struct sysinfo si;

/**
 * Display ram percentage of occupancy 
 */

static char ram_result[RAM_RESULT_SIZE]; /*Returned result */

/*STATS OF RAM*/
char * ram(){

	unsigned long int ram_used;

	ram_result[0]='\0';
	ram_used= si.totalram - si.freeram - si.sharedram - si.bufferram;
	assert(RAM_RESULT_SIZE>=5+1);
	(void)myprint_percentage_s(ram_result, (int) ( ram_used / ( ((float)si.totalram) /100 )));
	
	return ram_result;
}

char * init_ram(char * UNUSED(conf_line)){

	return "Ram ";
}

void exit_ram(){

}
