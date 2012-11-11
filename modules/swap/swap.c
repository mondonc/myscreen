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

#include "swap.h"

extern struct sysinfo si;

static char swap_result[SWAP_RESULT_SIZE]; /*Returned result */

/**
 * Display swap percentage of occupancy 
 */

static int calculate_swap(){

	if (si.totalswap == 0)
		return -1;

	return (int)  ( (float)( (si.totalswap - si.freeswap)) / ( ((float)si.totalswap) /100) );

}

/*STATS OF SWAP*/
char * swap(){

	int swap_used;
	char * ptr;

	ptr=swap_result;
	swap_used = calculate_swap();

	assert(SWAP_RESULT_SIZE>=1+4+1+1);
	if (swap_used != -1){
		(*(ptr++))='<';
		ptr+=myprint_percentage(ptr, swap_used);
		(*(ptr++))='>';
		(*(ptr++))=' ';
		(*(ptr))='\0';
	} else {
		swap_result[0]='\0';
	}
	return swap_result;
}

char * init_swap(char * UNUSED(conf_line)){

	return "Swap ";
}

void exit_swap(){

}
