/*Copyright 2009-2014 Clément Mondon <clement.mondon@gmail.com>

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

#ifndef _CPU_H
#define _CPU_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <myscreen-stats.h>

#define PROC_STAT "/proc/stat"
#define COLOR_CPU RED BOLD
#define NB_CPU_MAX 8
#define CPU_RESULT_SIZE (4 + (NB_CPU_MAX * 4 ) + 4)
char * cpu();
char * init_cpu(char * line);
void exit_cpu();

#endif
