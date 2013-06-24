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

#include "disks_access.h"

/**
 * Display disk rate
 */

static long int * disks; /* ptr to disks_static */
static long int * disks_new; /* ptr to disk_new */
static long int disks_static[NB_DISK_MAX];  /* tab of disk values*/
static long int disks_new_static[NB_DISK_MAX];  /*tab of updated disk values*/
static char disks_name[NB_DISK_MAX][DISK_ACCESS_SIZE+1];  /*tab of disk names*/
static int nb_disks; /*number of disks*/
static char disk_result[DISK_ACCESS_RESULT_SIZE];
extern char line[];


/*
** Get infos of a line from /proc/diskstats, respecting the buffers sizes
** This function is a secured alternative to sscanf
*/
static int sscan_diskstats_line(const char *line, int *type, char *part, long *_disk) {

  int i;

  /* get type info */
  line = jump_separators(line);
  *type = atoi(line);
  line = jump_info(line);

  /* jump 1 info */
  line = jump_separators(line);
  line = jump_info(line);

  /* get part info */
  line = jump_separators(line);
  line = copy_info(line, part, PROC_DISKSTATS_PART_SIZE);

  /* jump 9 infos */
  for (i = 0; i < 9; i++) {
    line = jump_separators(line);
    line = jump_info(line);
  }

  /* get disk info */
  line = jump_separators(line);
  *_disk = atol(line);

  return 0;
}


/*Update values of disks*/
static int update_disks(long * disk){

	/*Recent Values*/
	long _disk;
	int type;
	char part[DISK_ACCESS_SIZE+3];
	int cpt = 0;

	/*File "PROC_DISKSTATS"*/
	FILE * f;

	assert(disk != NULL); /*Valid disks*/

	/*Get recent values*/	
	if ( (f=fopen(PROC_DISKSTATS, "r")) != NULL ){

		int cpt2 = 0;

		while ( fgets(line, LINE_SIZE, f) != NULL && cpt < NB_DISK_MAX ){

		  /*
			if (sscanf(line, " %d %*d %s %*s %*s %*s %*s %*s %*s %*s %*s %*s %ld ", &type, part, &_disk) != 3)
			error("Read of diskstats", FALSE);
		  */
		  sscan_diskstats_line(line, &type, part, &_disk);
			if ( type == 8 && strlen(part) == DISK_ACCESS_SIZE ){

				/*New disk ?*/
				if (disks_name[cpt][0]=='\0'){
					assert(strlen("Disk [] connected ")+DISK_ACCESS_SIZE+3 < DISK_ACCESS_RESULT_SIZE);
					(void)snprintf(disk_result, DISK_ACCESS_RESULT_SIZE, "Disk [%s] connected ", part);
					strcpy(disks_name[cpt],part);

				} else {

					/*Loop to "down" all disk_names until it is "part" */
					while (strncmp(part,disks_name[cpt],DISK_ACCESS_SIZE) != 0){

						/*disks_name[cpt] disconnected */
						if ( cpt < NB_DISK_MAX-1 && disks_name[cpt][0] != '\0'){

							cpt2 = cpt;

							assert(strlen("Disk [] disconnected ")+DISK_ACCESS_SIZE+3 < DISK_ACCESS_RESULT_SIZE);
							(void)snprintf(disk_result, DISK_ACCESS_RESULT_SIZE, "Disk [%s] disconnected ", disks_name[cpt]);


							while (cpt2 < NB_DISK_MAX-1){
								strcpy(disks_name[cpt2],disks_name[cpt2+1]); 
								cpt2++;
							}
						} else {
							break;
						}
					}
				}	

				/*Save recent values*/
				disk[cpt] = _disk;
				cpt++;
			}
		}

		/*Clear end of disks_name if disk removed*/
		cpt2=cpt;
		while (cpt2 < NB_DISK_MAX && disks_name[cpt2][0]!='\0'){
			assert(strlen("Disk [] disconnected ")+DISK_ACCESS_SIZE+3 < DISK_ACCESS_RESULT_SIZE);
			(void)snprintf(disk_result, DISK_ACCESS_RESULT_SIZE, "Disk [%s] disconnected ", disks_name[cpt2]);
			disks_name[cpt2++][0]='\0';
		}


		if (fclose(f) == EOF){
			perror("Closing diskstat file ");
		}
	}
	else {
	  IFDEBUG(error("Error : disks_access: ", FALSE); perror(PROC_DISKSTATS));
	}

	return cpt;
}


/*Build result string*/
static void calculate_disks(){

	int percentage;
	int cpt,i;
	char * ptr;

	ptr = disk_result;

	assert(TIME!=0);
	assert( (NB_DISK_MAX * 5) * 2 < DISK_ACCESS_RESULT_SIZE);
	assert(disks != NULL); /*Valid disks*/
	assert(disks_new != NULL); /*Valid disks_new*/
	/*assert(disk_result);*/ /*Valid result*/

	percentage=0;

	for(cpt=0;cpt<nb_disks;cpt++){


		/*Calculate Rate*/
		percentage= (int) (( (float)( (disks_new[cpt] - disks[cpt] )) ) / (TIME * 10) );

		/*Display percentage or device if it is null*/
		if (percentage != 0 ){
			ptr+=myprint_percentage_s(ptr, percentage);
		} else {
			/*Copy device name to disk_result*/
			for (i=0;i<DISK_ACCESS_SIZE;i++){ 
				(*(ptr++)) = disks_name[cpt][i];
			}
			(*(ptr++))=' ';
			(*ptr)='\0';
		}
	}
}

/*DISK*/
char * disks_access(){

	long * disks_temp;
	assert(disks != NULL); /*Valid disks*/
	assert(disks_new != NULL); /*Valid disks_new*/
	/*assert(disk_result); *//*Valid result*/

	disk_result[0]='\0';

	nb_disks = update_disks(disks_new);

	if (disk_result[0] == '\0' ){
		calculate_disks();
	}

	disks_temp=disks;
	disks=disks_new;
	disks_new=disks_temp;

	return disk_result;
}

char * init_disks_access(char * UNUSED(conf_line)){

	int cpt;
	disks = disks_static;
	disks_new = disks_new_static;

	for (cpt=0;cpt< NB_DISK_MAX;cpt++) {
		disks_name[cpt][0]='\0';
	}

	nb_disks = update_disks(disks);

	return "Disk access ";
}



void exit_disks_access(){

}

