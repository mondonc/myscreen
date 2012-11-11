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

#include "reboot.h"

/**
 * Indicates if reboot is requiredi with a @
 */

char * reboot(){

	FILE * f;

/*Test if file REBOOT_REQUIRED exists*/
	if ( (f=fopen(REBOOT_REQUIRED, "r")) ){
		if (fclose(f) == EOF){
			perror("Closing REBOOT_REQUIRED file ");
		}
		return "@ ";
	}
	return "";
}

char * init_reboot(char * UNUSED(conf_line)){

	return "Reboot ";
}

void exit_reboot(){

}


