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

#include "cpu.h"

/**
 * Display cpu rates 
 */

static int nb_cpu; /*Number of cpu*/
static int unsigned long * cpus; /* Pointer to tab of cpu values */
static int unsigned long * cpus_news; /* Pointer to tab of new cpu values */
static int unsigned long cpus_static[NB_CPU_MAX+1]; /* Tab of cpu values */
static int unsigned long cpus_news_static[NB_CPU_MAX+1]; /* Tab of new cpu values */
static long int proc_stat_unit; /* to sysconf(_SC_CLK_TCK) result in function of architecture */
extern char line[]; 
static char cpu_result[CPU_RESULT_SIZE];

/* Update cpu values */
static void update_cpus(){

	/*Recent Values*/
	unsigned long _cpu_u; /*User time*/
	unsigned long _cpu_s; /*System time*/

	int unsigned long * tmp;
	FILE * f;
	tmp = cpus;
	cpus = cpus_news;
	cpus_news = tmp;

	/*Get recent values*/	
	if ( (f = fopen(PROC_STAT, "r")) != NULL){
		if (fgets(line, LINE_SIZE, f) != NULL) {

			/*Global cpu*/
			if (strstr(line, "cpu") != NULL){

				if (sscanf(line, " %*100s %100lu %*100s %100lu", &_cpu_u, &_cpu_s) != 2 ){
					DEBUG_WARNING("Can't read cpu values");
					return ;
				}

				/*Save recent values*/
				cpus_news[0] = _cpu_u + _cpu_s;
			} else {
				cpus_news[0] = 0;
			}

			/*Detailled cpu*/
			if (nb_cpu > 1){
				int cpt;
				cpt = 1;
				while(fgets(line, LINE_SIZE, f) != NULL && cpt <= NB_CPU_MAX){ /* (<=) because cpt begin at 1*/
					if (strstr(line, "cpu") != NULL){

						if (sscanf(line, " %*100s %100lu %*100s %100lu", &_cpu_u, &_cpu_s) != 2 ){
							DEBUG_INFO("Reading cpu values to cpu rate");	
						} else {
							/*Save recent values*/
							cpus_news[cpt] = _cpu_u + _cpu_s;
							cpt++;
						}
					}
				}
			}
		} else {
			DEBUG_WARNING("Reading proc stats file to cpu rate : Failed!");
			nb_cpu = -1;
		}
		if (fclose(f) == EOF){
			perror("Closing cpu (PROC_STAT) file ");
		}

	} else {
		DEBUG_WARNING("Opening proc stats file to cpu rate : Failed!");
		nb_cpu = -1;
	}
}

/* Display cpu rates */
char * cpu(){

	char * ptr = cpu_result;


	assert(TIME != 0);

	cpu_result[0] = '\0';
	ptr = cpu_result;

	update_cpus();

	assert(CPU_RESULT_SIZE >= 6 + 5 * nb_cpu);

	if(nb_cpu > 0){

		unsigned int percentage;

		/*Calculate Rate*/
		percentage = (unsigned int) proc_stat_unit * ( ( (float)( (cpus_news[0] - cpus[0] )) / (TIME*100) ) / nb_cpu);
		assert(CPU_RESULT_SIZE >= 5);
		ptr += myprint_percentage(ptr, percentage);
		assert(ptr - cpu_result < CPU_RESULT_SIZE);

		/*Detailled cpu*/
		if (nb_cpu > 1){

			int cpt;

			assert(CPU_RESULT_SIZE >= 6);
			(*(ptr++)) = '(';

			for(cpt = 1; cpt <= nb_cpu; cpt++){

				percentage = (unsigned int) (( (float)( (cpus_news[cpt] - cpus[cpt] )) / TIME) ); 
				assert(CPU_RESULT_SIZE >= 6 + 5 * cpt);
				ptr += myprint_percentage(ptr, percentage);
				assert(ptr - cpu_result < CPU_RESULT_SIZE);

			}
			assert(ptr - cpu_result + 3 <= CPU_RESULT_SIZE);
			(*(ptr++)) = ')';
			(*(ptr++)) = ' ';
			(*ptr) = '\0';
		} else {
			assert(ptr - cpu_result + 2 <= CPU_RESULT_SIZE);
			(*(ptr++)) = ' ';
			(*ptr) = '\0';
		}
	}
	return cpu_result;
}

char * init_cpu(char * UNUSED(conf_line)){

	FILE * f;
	nb_cpu = -1;
	/*printf("CONF : %s\n", conf_line);*/

	proc_stat_unit = sysconf(_SC_CLK_TCK);

	assert(proc_stat_unit != 0);
	assert(CPU_RESULT_SIZE >= 4 * (NB_CPU_MAX + 1) + 3 + 1);

	/*Determine number of cpu*/
	if ((f = fopen(PROC_STAT, "r")) != NULL){

		while ((fgets(line, LINE_SIZE, f)) != NULL){
			if (strstr(line, "cpu") != NULL){
				nb_cpu++;
			}
		}
		if (fclose(f) == EOF){
			perror("Closing cpu file ");
		}

	} else {
		DEBUG_WARNING("Can't open cpu file");
	}

	cpus = cpus_static;
	cpus_news = cpus_news_static;

	if (nb_cpu > 0){
		update_cpus();
	}
	return "cpu%(cpu0%,...) ";
}



void exit_cpu(){
/* no code */
}
