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

#ifndef _DISKS_USAGE_H
#define _DISKS_USAGE_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/vfs.h>
#include <sys/statvfs.h>
#include <myscreen-stats.h>

#define POWER_SUPPLY_SIZE 20 /*Size of fields */

#define DISKS_USAGE_RESULT_SIZE 8 /* "S[999% 0"*/
#define COLOR_DISKS_USAGE BLUE
#define COLOR_ALERT_DISKS_USAGE "\005{= rk}"
#define NB_DISK_MAX 10
#define DISK_USAGE_SIZE ( 30 ) 
#define DISK_LEN ( 12 ) 
#define DISK_USAGE_RESULT_SIZE (((DISK_USAGE_SIZE* NB_DISK_MAX) +1) * 2 ) 
#define PROC_MOUNTS "/proc/mounts"
#define FILESYSTEM_TYPES "ext1 ext2 ext3 ext4 vfat"
#define VALUE_MO  1048576 /* 2^20 == 1024*1024*/


char * disks_usage();
char * init_disks_usage(char * line);
void exit_disks_usage();

#endif
