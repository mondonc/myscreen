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

#ifndef _USER_H
#define _USER_H
#include <stdio.h>
#include <malloc.h>
#include <utmp.h>
#include <myscreen-stats.h>
#define COLOR_USERS "\005{=b dd}"
#define USER_RESULT_SIZE 7 /*limite: 9 9999 */
char * users();
void exit_users();
char * init_users(char * line);

#endif
