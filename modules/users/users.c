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

#include "users.h"

/**
 * Displays the number of users connected at this time on the machine
 */

static char user_result[USER_RESULT_SIZE]; /*Returned result */

/*NUMBER OF USERS*/
char * users(){

	/*Variables*/
	struct utmp * user;
	int nb_users;

	/*Init*/
	setutent();
	nb_users=0;
	user_result[0]='\0';

	/*Count number of user*/
	while ((user=getutent()) != NULL) {
		if ((user->ut_type) == 7)
			nb_users++;
	}
	(void) snprintf(user_result, USER_RESULT_SIZE, "%d# ", nb_users);
	endutent();
	return user_result;
	/*free(user);*/
}

char * init_users(char * UNUSED(conf_line)){

	return "Users ";
}

void exit_users(){

}
