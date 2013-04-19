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

#ifndef _DISK_H
#define _DISK_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <myscreen-stats.h>

#define PROC_DISKSTATS "/proc/diskstats"


#define NB_DISK_MAX 10
#define COLOR_DISKS_ACCESS BLUE BOLD
#define DISK_ACCESS_SIZE 3 /*Size of device 3 to sd[a-z]*/
#define DISK_ACCESS_RESULT_SIZE ( ( (5 * NB_DISK_MAX) +1) * 2 )  

char * disks_access();
char * init_disks_access(char * line);
void exit_disks_access();
#endif
