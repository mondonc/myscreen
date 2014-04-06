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

#ifndef  _MYSCREEN_STATS_H
#define  _MYSCREEN_STATS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <utmp.h>
#include <unistd.h>
#include <signal.h>
#include <sys/sysinfo.h>
#include <linux/kernel.h>
#include <linux/limits.h>
#include <sys/time.h>
#include <sys/types.h>

/* DEBUG MACROS ?*/
#ifdef DEBUG
#define IFDEBUG(instruction); instruction
#define IFDEBUG_PRINT(chaine); fputs(chaine, stdout);
#define DEBUG_INFO(chaine) fputs("#INFO: ", stdout); printf(chaine); fputs("\n", stdout);
#define DEBUG_WARNING(chaine) fputs("#WARNING: ", stdout); printf(chaine); fputs("\n", stdout);
#define DEBUG_ERROR(chaine) fputs("#ERROR: ", stdout); printf(chaine); fputs("\n", stdout);
#define DEBUG_MODULE(chaine,index,name,conf) printf("#MODULE %d (%s):", index, name); printf(chaine,conf); fputs("\n", stdout);
#define IFNDEBUG(instruction); /*no code*/
#else
#define IFDEBUG(instruction); /*no code*/
#define IFDEBUG_PRINT(chaine); /*no code*/
#define IFDEBUG_PRINF(chaine); /*no code*/
#define DEBUG_INFO(chaine) /* no code */
#define DEBUG_WARNING(chaine) /* no code */
#define DEBUG_ERROR(chaine) /* no code */
#define DEBUG_MODULE(chaine,index,name,conf) /* no code */
#define IFNDEBUG(instruction); instruction
#endif

/* ONESHOT MACROS (used by continuous intergration) */
#ifdef ONESHOT
#define IFONESHOT(instruction); instruction
#else
#define IFONESHOT(instruction); /*no code*/
#endif


/* To make unused-parameter enjoy */
#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#ifdef __GNUC__
#  define UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_ ## x
#else
#  define UNUSED_FUNCTION(x) UNUSED_ ## x
#endif

#include <tools.h>
#include <parse-config.h>
#include <proc_tools.h>

/* include the message queue interface for main and modules */
#include "msg_queue.h"

/***********/
/* MODULES */
/***********/
#include "modules_list.h"
#include "colors.h"

#define RESULT_SIZE_MAX 800 /* Max size of result string (per module) */
#define LINE_SIZE 200 /* Read Line size of proc files and configuration file */
#define MODULE_NAME_SIZE 50 /* Max size of module name */

#define TIME 2 /* Main loop time ; Time between 2 stats generation */

#define COLOR_END "\005{-}"

#define TRUE 1
#define FALSE 0

int main(/*int argc, char ** argv*/);
void end_wait();

#endif
