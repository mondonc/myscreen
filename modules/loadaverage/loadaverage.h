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

#ifndef _LOADAVERAGE_H
#define _LOADAVERAGE_H
#include <stdio.h>
#include <sys/sysinfo.h>
#include <myscreen-stats.h>
#define COLOR_LOADAVERAGE YELLOW BOLD
#define LOADAVERAGE_RESULT_SIZE 10 /*Loadaverage limited at 99999.99xx */ 

char * loadaverage();
char * init_loadaverage(char * line);
void exit_loadaverage();

#endif
