/* Copyright 2009-2013 Clément Mondon <clement.mondon@gmail.com>

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

extern char line[]; /* Read line*/
static char battery_result[BATTERY_RESULT_SIZE]; /* Returned result */
static int time_cpt; /* To time elapsed */

char * power_supply_charge_full[POWER_SUPPLY_NB] = POWER_SUPPLY_CHARGE_FULL;
int power_supply_charge_full_len[POWER_SUPPLY_NB] = POWER_SUPPLY_CHARGE_FULL_LEN;
char * power_supply_charge_now[POWER_SUPPLY_NB] = POWER_SUPPLY_CHARGE_NOW;
int power_supply_charge_now_len[POWER_SUPPLY_NB] = POWER_SUPPLY_CHARGE_NOW_LEN;

/*
 * Read file of path "battery" and build result string
 */
static int calculate_battery(char * battery_result, const char * battery){

	long int charge_full;
	long int charge_now;
	char * status; /* (POWER_DISCHARGING | POWER_CHARGING | POWER_FULL) */
	FILE * f; /*Current file*/
	short cpt;
	/*char * ptr;*/
	unsigned int percentage;

	status = "";
	charge_full=0;
	charge_now=0;

	assert(BATTERY_RESULT_SIZE>=2);

	/*Open file*/
	f = fopen(battery, "r");
	if (f == NULL){
		IFDEBUG_PRINT("I can't read battery file");
		battery_result[0]='\0';
		return 0;
	}

	/*It is supposed that lines are correctly ordoned */
	while (fgets(line, LINE_SIZE, f) != NULL){

		/*Determine status (POWER_DISCHARGING | POWER_CHARGING | POWER_FULL) */
		if (!status[0] && strstr(line, POWER_SUPPLY_STATUS) != NULL){
			
			assert(strstr(line, "=") != NULL);

			status = line;
			while ( ( *(status++) ) != '=' );

			assert(strlen(POWER_DISCHARGING) == POWER_DISCHARGING_LEN);
			assert(strlen(POWER_CHARGING) == POWER_CHARGING_LEN);
			assert(strlen(POWER_FULL) == POWER_FULL_LEN);

			if (!strncmp(status, POWER_DISCHARGING, POWER_DISCHARGING_LEN)){
				battery_result[0]='_';				

			} else 	if (!strncmp(status, POWER_CHARGING, POWER_CHARGING_LEN)){
				battery_result[0]='~';				

			}	else 	if (!strncmp(status, POWER_FULL, POWER_FULL_LEN)){ /*If full, don't calculate percentage*/

				assert(BATTERY_RESULT_SIZE >= 3);

				battery_result[0]='"';				
				battery_result[1]=' ';				
				battery_result[2]='\0';

				assert(strlen(battery_result) <= BATTERY_RESULT_SIZE);

				if (fclose(f) == EOF){
						perror("Closing Battery file ");
				}
				assert(strlen(battery_result) == 2);
				return 2;
				
			} else {
				IFDEBUG_PRINT("Can't read battery status");
				battery_result[0]='?';				
			}
		}

		/*Determine charge_full*/
		if (charge_full == 0){
			cpt = 0;
			while ( cpt < POWER_SUPPLY_NB ) {
				if (strncmp(line, power_supply_charge_full[cpt], power_supply_charge_full_len[cpt]) == 0){
					charge_full = strtol((line + power_supply_charge_full_len[cpt]), NULL, 10);
					break ;
				}
				cpt++;
			}
		}

		/*Determine charge_now*/
		if (charge_now == 0) {
			cpt = 0;
			while ( cpt < POWER_SUPPLY_NB ) {
				if (strncmp(line, power_supply_charge_now[cpt], power_supply_charge_now_len[cpt]) == 0){
					charge_now = strtol((line + power_supply_charge_now_len[cpt]), NULL, 10);
					break ;
				}
				cpt++;
			}
		}
	}

	/*Close file*/
	if (fclose(f) == EOF){
		perror("Closing Battery file ");
	}

	/*Calculate Rate*/
	if (charge_full == 0 || charge_now == 0){
		assert(BATTERY_RESULT_SIZE>2)
		IFDEBUG_PRINT("Can't read values of battery");
		battery_result[1]=' ';
		battery_result[2]='\0';
		return 2;
	}

	assert(BATTERY_RESULT_SIZE>1);
	battery_result[1]='|';
	percentage= (unsigned int) (( ((float)charge_now)  / charge_full) * 100);
	assert(BATTERY_RESULT_SIZE>=3+5);
	return myprint_percentage_s(&battery_result[2], percentage);
}

/*
 * BATTERY : Main function
 */
char * battery(){

	char * ptr;

	if (time_cpt == 0){
		battery_result[0]='\0';
		ptr = battery_result; 
		ptr += calculate_battery(ptr, PROC_BATTERY0);
		ptr += calculate_battery(ptr, PROC_BATTERY1);
		ptr += calculate_battery(ptr, PROC_BATTERY2);

		assert(ptr-battery_result<BATTERY_RESULT_SIZE);

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

