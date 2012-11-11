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

#ifndef _RAM_H
#define _RAM_H
#include <stdio.h>
#include <sys/sysinfo.h>
#include <myscreen-stats.h>
#define COLOR_RAM "\005{=b dg}"
#define RAM_RESULT_SIZE 6


char * ram();
char * init_ram(char * line);
void exit_ram();
#endif
