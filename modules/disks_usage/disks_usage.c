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

#include "disks_usage.h"

/*
 * Display disks_usage 
 */

static char disk_result[DISK_USAGE_RESULT_SIZE];
extern char line[];

/* Get usage values of disks*/
static int get_disks_usage(char disks_usage[NB_DISK_MAX][DISK_USAGE_RESULT_SIZE]){

	char dev[DISK_USAGE_RESULT_SIZE];
	char mnt[DISK_USAGE_RESULT_SIZE*2];
	char type[DISK_USAGE_SIZE*2];
	char * found;
	int cpt;
	struct statvfs buf;


	char * unit;
	/*File "PROC_MOUNTS"*/
	FILE * f;


	cpt=0;

	/*Get recent values*/	
	if ( (f=fopen(PROC_MOUNTS, "r")) != NULL ){

		while ( fgets(line, LINE_SIZE, f) != NULL && cpt < NB_DISK_MAX){

			if (sscanf(line, " %s %s %s %*s %*d %*d", dev, mnt, type) != 3)
				error("Read of PROC_MOUNTS", FALSE);

			if (strstr(FILESYSTEM_TYPES, type) != NULL ) {

				int percentage;
				unsigned long blksize, blocks, freeblks, disk_size, used;
				double free_res;

				/* Get fs infos */
				if (statvfs(mnt, &buf) == -1) {
					error("Read statvfs", FALSE);
					perror("statvfs");
				}

				if ( (found = strstr(mnt, "/media/")) != NULL){
					found += 7; /* strlen("/media/") == 7 */
				} else {
					found = mnt;
				}
				/* Mount point should be short*/
				found[DISK_LEN-1] = '.';
				found[DISK_LEN] = '\0';



				blksize = buf.f_bsize;
				blocks = buf.f_blocks;
				freeblks = buf.f_bavail;
				free_res = (((double) freeblks) / (double)VALUE_MO ) * blksize;

				disk_size = blocks;
				used = disk_size - freeblks;

				unit= "Mo";
				if ( free_res > 1024){
					unit = "Go";
					free_res = (double) free_res / (double) 1024;
				}
				if ( free_res > 1024){
					unit = "To";
					free_res = (double) free_res / (double) 1024;
				}

				percentage = (int) ( (((double)used)/((double)disk_size)) * 100) ;


				if (percentage > 95)
					snprintf(disks_usage[cpt], DISK_USAGE_RESULT_SIZE/NB_DISK_MAX, "%s%s:%d%%(%.1f%s)%s",COLOR_ALERT_DISKS_USAGE,found,percentage, free_res,unit,COLOR_DISKS_USAGE);
				else
					snprintf(disks_usage[cpt], DISK_USAGE_RESULT_SIZE/NB_DISK_MAX, "%s:%d%%(%.1f%s)",found,percentage, free_res,unit);

				cpt++;

			}

		}	

		if (fclose(f) == EOF){
			perror("Closing diskstat file ");
		}

	}
	return cpt;
}


/*DISK*/
char * disks_usage(){

	int cpt;
	int nb_disks;
	char disks_usage[NB_DISK_MAX][DISK_USAGE_RESULT_SIZE];

	nb_disks = get_disks_usage(disks_usage);

	disk_result[0]='\0';

	for (cpt=0;cpt<nb_disks;cpt++) {
		strcat(disk_result,disks_usage[cpt]);
		strcat(disk_result," ");
	}
	return disk_result;
}

char * init_disks_usage(char * UNUSED(conf_line)){

	return "Disk usages ";
}



void exit_disks_usage(){

}
