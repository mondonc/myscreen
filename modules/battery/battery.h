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

#ifndef _BATTERY_H
#define _BATTERY_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <myscreen-stats.h>

#define PROC_BATTERY0 "/sys/class/power_supply/BAT0/uevent" 
#define PROC_BATTERY1 "/sys/class/power_supply/BAT1/uevent"
#define PROC_BATTERY2 "/sys/class/power_supply/BAT2/uevent"

#define POWER_SUPPLY_STATUS "POWER_SUPPLY_STATUS="
#define POWER_SUPPLY_STATUS_LEN 20
#define POWER_DISCHARGING "Discharging"
#define POWER_DISCHARGING_LEN 11
#define POWER_CHARGING "Charging"
#define POWER_CHARGING_LEN 8
#define POWER_FULL "Full"
#define POWER_FULL_LEN 4
#define POWER_SUPPLY_CHARGE_FULL { "POWER_SUPPLY_CHARGE_FULL=", "POWER_SUPPLY_ENERGY_FULL=" }
#define POWER_SUPPLY_CHARGE_FULL_LEN { 25, 25 }
#define POWER_SUPPLY_CHARGE_NOW { "POWER_SUPPLY_CHARGE_NOW=", "POWER_SUPPLY_ENERGY_NOW=" }
#define POWER_SUPPLY_CHARGE_NOW_LEN { 24, 24 }
#define POWER_SUPPLY_NB 2
#define POWER_SUPPLY_SIZE 20 /*Size of fields */

#define TIME_BATTERY 15 /* Multiplied by global loop TIME*/
#define BATTERY_RESULT_SIZE 8 /* "S[999% 0"*/
#define STATUS_LEN 30
#define COLOR_BATTERY GREEN BOLD
#define COLOR_WARNING_BATTERY RED BOLD


char * battery();
char * init_battery(char * line);
void exit_battery();

#endif
