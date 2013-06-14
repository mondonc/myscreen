/*Copyright 2009,2010,2011,2012,2013 Clément Mondon <clement.mondon@gmail.com>

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

#include "myscreen-stats.h"

/* Extern variables defined in main/modules_list.c (autogenerated) */
extern char * (*main_mod[NB_MODULES_MAX])(); 	/* For each module, function to display stats */
extern char * modules_color[]; 						/* For each module, display color */
extern void (*exit_mod[NB_MODULES_MAX])();    /* For each module, function to exit */

/* Current configuration read by parse-config */
char * (*current_conf[NB_MODULES_MAX])();
char * (*exit_current_conf[NB_MODULES_MAX])();

/* Multiple usages var */
char line[LINE_SIZE + 1];

/* Flags to drive loops */
int stats_loop; /* While TRUE, display stats */
int main_loop;

/* Struct at least used by uptime, processes, RAM, SWAP, and  loadaverage modules */
struct sysinfo si;

const char * empty_string = "";


/* 
 * Get sysinfo struct 
 */
void get_sysinfo() {
	if (sysinfo(&si) == -1){
		perror("Fatal error of syscall sysinfo used at least by LOADAVERAGE,USERS,PROCS,RAM,SWAP");
		exit(EXIT_FAILURE);
	}
}

/* 
 * Find module index 
 */
int find_module_index(int cpt){
	int module_idx = 0;
	while ( current_conf[cpt] != main_mod[module_idx] && ++module_idx );
	return module_idx;
}

/* 
 * Call module's function and display stat 
 */
void display_module_stats(short module_idx, short cpt){
	IFDEBUG(printf("(%d) [%s]\n", module_idx, current_conf[cpt]()););
	IFNDEBUG((void)fputs(modules_color[module_idx], stdout); (void)fputs(current_conf[cpt](), stdout);)
}

/* 
 * Display END and wait 
 */
void end_wait(){
	/* Display's end */
	(void)puts(COLOR_END);
	(void)fflush(stdout);

	/* Waiting between two stats generation */
	(void)sleep(TIME);
}

/* 
 * Exit of all modules 
 */
static void exit_modules(){

	int cpt;

	cpt = 0;
	/* For each module */
	while (exit_current_conf[cpt] != 0) {
		assert(cpt<NB_MODULES_MAX);
		exit_current_conf[cpt++]();
	}
}

/* 
 * Loop to print stats 
 */
static void loop_stat(){

	int cpt;
	int module_idx;

	/* While we have to generate stats */
	while(stats_loop) {

		cpt = 0;

		/* Get sysinfo struct (used by several modules) */
		get_sysinfo();

		/* For each module */
		while ( cpt<NB_MODULES_MAX && current_conf[cpt] != NULL ){

			/* Find module's index */
			module_idx = find_module_index(cpt);

			/* Display stats */
			(void) display_module_stats(module_idx, cpt);

			cpt++;
		}

		/* Display END and wait between two stats's generation*/
		(void) end_wait();

		IFONESHOT(stats_loop = FALSE);
	}
}


/* 
 * MAIN function 
 */
int main (/*int argc, char ** argv*/)
{

	main_loop = TRUE;

	DEBUG_INFO("Running in debug mode")

	/* Install signals */
	install_signals();   

	/* Always True, also when myscreen-stats is stopped */
	while(TRUE){

		/* Loop to read_conf and init all modules , display stats, and exit modules */
		/* main_loop become false when receiving signal */
		while (main_loop){ 

			/* stats_loop become false when receiving signal */
			stats_loop=TRUE; 

			/* current_conf initialisation and modules initiatlisation calls */
			DEBUG_INFO("Reading configuration file")
			get_configuration();

			/* MAIN LOOP (print all stats while stats_loop == TRUE ) */
			IFDEBUG_PRINT(" \n");
			DEBUG_INFO("Starting stats loop")
			loop_stat();

			/* Modules's exit calls */
			DEBUG_INFO("Exit of all modules")
			exit_modules();

			IFONESHOT(exit(EXIT_SUCCESS));

		}

		/* Stopped, waiting new signal setting main_loop at TRUE */
		(void)pause();
	}
}

