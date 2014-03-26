/*Copyright 2009,2010,2011,2012 Clément Mondon <clement.mondon@gmail.com>

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

#include <parse-config.h>
#include <strings.h>
#include <myscreen-stats.h>

/*********************************/
/* CONFIGURATION FILE MANAGEMENT */
/*********************************/

/* Defined in main/myscreen-stats.c */
extern char line[];
extern char * empty_string;
extern int current_conf[NB_MODULES_MAX]; /* modules indexes are ordered in this table */


/* Autogenerated in main/modules_list.c */
extern char* modules[];
extern char* modules_color[];
extern char * (*main_mod[NB_MODULES_MAX])();
extern char * (*init_mod[NB_MODULES_MAX])(const char * conf_line);
extern char * (*exit_mod[NB_MODULES_MAX])();


static FILE * f; /* To store configuration file */

/*
 * Search HOME configuration file
 */
void search_home(char ** conf_home){
	char * home;

	assert((*conf_home) == NULL);

	home = getenv(HOME);
	if (home == NULL){
		DEBUG_WARNING("I can't read HOME variable")
		(*conf_home) = malloc(1*sizeof(char));
		conf_home[0]='\0';
	} else {
		unsigned int home_len;
		unsigned int home_len_max;
		/* Build home path */
		home_len = strlen(home) + 1;
		home_len_max = (home_len + strlen(CONF_HOME_FILE)) *sizeof(char);
		(*conf_home) = malloc(home_len_max + 1 );
		strncpy( (*conf_home), home, home_len_max  );
		strncat( (*conf_home), CONF_HOME_FILE, home_len_max - home_len );
		assert(strlen( (*conf_home) ) < home_len_max);
		assert(home_len_max < FILENAME_MAX );
	}

	assert((*conf_home) != NULL);
}

/*
 * Try to read home configuration
 */
int try_read_home_configuration(){
	char * conf_home;
	short result;

	conf_home = NULL;

	search_home(&conf_home);

	assert(conf_home != NULL);

	if (( f = fopen(conf_home, "r")) == NULL){
	        DEBUG_INFO("I can not read HOME configuration file")
		result = FALSE;
	} else {
		DEBUG_INFO("I read HOME configuration file")
		result = TRUE;
	}
	free(conf_home);
	return result;
}

/*
 * Try to read ETC configuration
 */
int try_read_etc_configuration(){
	if ( (f = fopen(CONF_ETC_FILE, "r")) == NULL){
		DEBUG_INFO("I can not read ETC configuration file")
		return FALSE;
	}
	DEBUG_INFO("I read ETC configuration file")
	return TRUE;
}

/*
 * Try to read local(./) configuration
 */
int try_read_local_configuration(){
	if ( (f = fopen(CONF_LOCAL_FILE, "r")) == NULL){
		DEBUG_INFO("I can not read local configuration file")
		return FALSE;
	}
	DEBUG_INFO("I read local configuration file")
	return TRUE;
}

/*
 * Find configuration file
 */
static FILE * find_conf(){

  f = NULL;
  if (try_read_local_configuration() == FALSE){

    /* If we can't read HOME configuration file */
    if (try_read_home_configuration() == FALSE){

      /* Then trying to read ETC configuration file */
      if ( try_read_etc_configuration() == FALSE){

				/* If we can't read any configuration file, It is a problem */
				DEBUG_WARNING("Can't read any configuration file ( in /etc or in your HOME directory)")
				/* TODO : printf("/!\\  Can't read any configuration file (%s or in your HOME directory)\n",CONF_ETC_FILE); */
				(void) (void)fflush(stdout);
      }
    }
  }
  return f;
}

void init_module(int module_idx, int nb_module, char * line_conf){

	/* Save the original module index */
	current_conf[nb_module] = module_idx;

	/* Enabled module initialisation */
	DEBUG_MODULE("ENABLED with conf : «%s»",module_idx, modules[module_idx],
		     strtok(line_conf, "\n"))
	IFDEBUG((void)puts((init_mod[module_idx])(line_conf)););
	IFNDEBUG((void)fputs(modules_color[module_idx], stdout);
		 (void)fputs(init_mod[module_idx](line_conf), stdout););

}

/*
 * Read configuration file initializing current_conf[]
 */
int read_configuration_file(){
	int nb_module;
	int module_idx;
	char * line_conf; /* To define module configuration string begin (after the «=» ) */

	nb_module = 0;

	while (feof(f) == 0){

		/* For each lines */
		if (fgets(line, LINE_SIZE, f) != NULL && line[0] != CHAR_CONF_COMMENT && line[0] != '\n') {

			if ( (line_conf = line_reader(line, &module_idx)) != NULL){
				init_module(module_idx, nb_module, line_conf);
				nb_module++;
			} else {
				/* Module is disabled */
				IFDEBUG(printf("Module n°%d: disable\n",module_idx););
			}
		}
	} /*END OF WHILE*/

	assert(nb_module<=NB_MODULES_MAX);

	return nb_module;
}

/*
 * Get default configuation (all modules enabled)
 */
int get_default_configuration(){
	int nb_modules;

	IFDEBUG(printf("Default configuration : enable all modules"););
	/* Enable all modules */
	for(nb_modules=0;nb_modules < NB_MODULES_MAX;nb_modules++ ) {
		init_module(nb_modules,nb_modules,"\0");
	}

	return nb_modules;
}

/*
 * Read configuration file initializing current_conf[]
 */
int get_configuration(){

	int nb_modules = -1;
	/* Find configuration file */
	find_conf();

	/* If no configuration found */
	if ( f == NULL){

		nb_modules = get_default_configuration();

	/* Read configuration */
	} else {

		nb_modules = read_configuration_file();

		/* Close file */
		if (fclose(f) == EOF){
			perror("Closing Configuration file");
		}
		end_wait();
	}

	assert(nb_modules > -1 && nb_modules <= NB_MODULES_MAX);
	return nb_modules;
}

/*
 * Get a pointer to the start of the module's name,
 * and fill "len" with the module's name size
 */
static char *identify_module_name(const char *s, size_t *len) {
  while (*s && strchr(" \n\t", *s) != NULL)
    s++;
  *len = 0;
  while (s[*len] && strchr(" \n\t=", s[*len]) == NULL)
    (*len)++;
  return ((char *)s);
}

/*
 * Translate the module's name to this index
 */
int get_module_index(const char * s) {

 	int cpt;
	size_t len;
	char *sptr;

	sptr = identify_module_name(s, &len);

	for (cpt=0 ; cpt<NB_MODULES_MAX ; cpt++){
	  if (strlen(modules[cpt]) == len && strncmp(sptr, modules[cpt], len) == 0)
	    return cpt;
	}
	IFDEBUG(printf("PARSE-CONF: I can't understand %s \n", s););
	return -1;
}

/*
 * Read the syntax of the conf file
 */
char * line_reader(char * line, int * module_idx){

	char * adr_equal;
	if ( ((*module_idx) = get_module_index(line)) != -1){
		if ( (adr_equal = strchr(line, '=')) == NULL ){
			/* small syntax, without options */
			return empty_string;
		} else {
			/* long syntax, with options */
			return adr_equal+1;
		}
	}
	return NULL;
}

