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

#ifndef _NETWORK_H
#define _NETWORK_H
#include <stdio.h>
#include <string.h>
#include <myscreen-stats.h>

#define GW_SIZE 9
#define INTERFACE_SIZE 5
#define REFRESH_TIME 20 /*Refresh interface when no_activity > REFRESH_TIME (realtime is REFRESH_TIME * TIME) */
#define PROC_NET_DEV "/proc/net/dev"
#define COLOR_NETWORK MAGENTA BOLD
#define NETWORK_RESULT_SIZE 30 
#define PROC_NET_ROUTE "/proc/net/route"

#define OPT_AUTO "auto"

char * network();
char * init_network(char * line);
void exit_network();

#endif
