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

#include "battery.h"

/*
 * Display battery state and percentage of charge
 */

extern char line[]; /*Read line*/
static char battery_result[BATTERY_RESULT_SIZE]; /*Returned result */
static int time_cpt; /* To time elapsed*/

/*Read file of path "battery" and build result string*/
static void calculate_battery(char * battery_result, const char * battery){

	long int charge_full;
	long int charge_now;
	char * status; /* (POWER_DISCHARGING | POWER_CHARGING | POWER_FULL) */
	FILE * f; /*Current file*/
	char * ptr;
	unsigned int percentage;

	status = "";
	charge_full=0;
	charge_now=0;

	assert(BATTERY_RESULT_SIZE>=2);

	/*Open file*/
	f = fopen(battery, "r");
	if (f == NULL){
		IFDEBUG_PRINT("I can't read battery file");
		return ;
	}

	/*It is supposed that lines are corretly ordoned */
	while (fgets(line, LINE_SIZE, f) != NULL){

		/*Determine status (POWER_DISCHARGING | POWER_CHARGING | POWER_FULL) */
		if (!status[0] && strstr(line, POWER_SUPPLY_STATUS) != NULL){
			
			assert(strstr(line, "=") != NULL);

			status = line;
			while ( ( *(status++) ) != '=' );

			if (!strcmp(status,POWER_DISCHARGING)){
				battery_result[0]='_';				

			} else 	if (!strcmp(status,POWER_CHARGING)){
				battery_result[0]='~';				

			}	else 	if (!strcmp(status,POWER_FULL)){ /*If full, don't calculate percentage*/

				assert(BATTERY_RESULT_SIZE>=3);
				battery_result[0]='"';				
				battery_result[1]=' ';				
				battery_result[2]='\0';

				if (fclose(f) == EOF){
						perror("Closing Battery file ");
				}
				return ;
				
			} else {
				IFDEBUG_PRINT("Can't read battery status");
				battery_result[0]='?';				
			}
		}

		/*Determine charge_full*/
		if (charge_full == 0 && strstr(line, POWER_SUPPLY_CHARGE_FULL) != NULL){
			ptr = line;
			while ( ( *(ptr++) ) != '=' );
			charge_full = strtol(ptr, NULL, 10);
		}

		/*Determine charge_now*/
		if (charge_now == 0 && strstr(line, POWER_SUPPLY_CHARGE_NOW) != NULL){
			ptr = line;
			while ( ( *(ptr++) ) != '=' );
			charge_now = strtol(ptr, NULL, 10);
		}
	}

	/*Close file*/
	if (fclose(f) == EOF){
		perror("Closing Battery file ");
	}

	/*Calculate Rate*/
	if (charge_full == 0 || charge_now == 0){
		IFDEBUG_PRINT("Can't read values of battery");
		battery_result[2]='\0';
		return ;
	}

	assert(BATTERY_RESULT_SIZE>=2);
	battery_result[1]='|';
	percentage= (unsigned int) (( ((float)charge_now)  / charge_full) * 100);
	assert(BATTERY_RESULT_SIZE>=3+5);
	(void) myprint_percentage_s(&battery_result[2], percentage);
}

/*BATTERY*/
char * battery(){

	if (time_cpt == 0){
		battery_result[0]='\0';
		calculate_battery(battery_result, PROC_BATTERY0);
	} else if (time_cpt == TIME_BATTERY) {
		time_cpt=-1;
	} 
	time_cpt++;
	return battery_result;
}

char * init_battery(char * UNUSED(conf_line)){
	time_cpt=0;
	return "Battery ";
}

void exit_battery(){

}

