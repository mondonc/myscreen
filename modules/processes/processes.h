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

#ifndef _PROCESSES_H
#define _PROCESSES_H
#define COLOR_PROCESSES YELLOW
#define PROCESSES_RESULT_SIZE 9 /*nb processes limited at 999 999 */
#include <stdio.h>
#include <sys/sysinfo.h>
#include <myscreen-stats.h>

char * processes();
char * init_processes(char * line);
void exit_processes();

#endif
