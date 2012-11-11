#include <myscreen-stats.h> 
#include <parse-config.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <utmp.h>
#include <sys/sysinfo.h>
#include <linux/kernel.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

extern int stats_loop;
extern int main_loop;
extern char line[];

/* Struct to signal management */
static struct  sigaction usr1, usr2, cont;


/**********************/
/* ERROR MANAGEMENT */
/**********************/

/* 
 * Display error and exit if fatal == TRUE 
 */
void error(const char * err, unsigned short fatal){

  fprintf(stderr,"Error : %s",err);

  if (fatal == TRUE)
    exit(EXIT_FAILURE);
}


/**********************/
/* STRINGS MANAGEMENT */
/**********************/

/* 
 * Like strncpy, but clear string also (without space, tab, and \n )
 */
void strncpyclr(char * dest, const char * src, unsigned int len){
	
	unsigned int s, d;
	d = 0;
	for (s = 0 ; s < strlen(src) && s < len ; s++){
		if (src[s] != ' ' && src[s] != '\t' && src[s] != '\n'){
			dest[d++] = src[s];
		}
	}

}

/* 
 * Clear given string (without space, tab, and \n ) 
 */
void strnclr(char * src, unsigned int len){
	strncpyclr(src, (const char *) src, len);
}


/*************************/
/* DISPLAY OPTIMISATIONS */
/*************************/

/*
 * Print percentage to result, maxsize: 5
 */
int myprint_percentage(char * result, unsigned short percentage){

  int cpt=0;
  if (percentage>999){

    error("Percentage",FALSE);

  } else {
    
    if (percentage>99){
      result[cpt++]= '0' +percentage/100; 
      percentage=percentage%100;
    }
    result[cpt++]='0' +percentage/10;
    result[cpt++]='0' +percentage%10;
    result[cpt++]='%';
    result[cpt]='\0';
  }
  return cpt;

}

/*
 * Print percentage with space to result, maxsize: 6
 */
int myprint_percentage_s(char * result, unsigned short percentage){

  int cpt;
  cpt=myprint_percentage(result, percentage);
  result[cpt++]=' ';
  result[cpt]='\0';
  return cpt;

}

/*
 * Print str1 then str2 to standard output
 */
/*void myprint_ss(const char * st1, const char * st2){
  while ((*st1) != '\0')
    putchar(*(st1++));
  while ((*st2) != '\0')
    putchar(*(st2++));
}*/

/*
 * Print str1 to standard output
 */
void myprint_s(const char * st1){
  while ((*st1) != '\0')
    putchar(*(st1++));
}

/*
 * Print str1 with a '\n' to standard output
 */
/*void myprint_sn(const char * st1){
  myprint_s(st1);
  putchar('\n');
}*/


/*********************/
/* SIGNAL MANAGEMENT */
/*********************/


/* 
 * Install sigaction function 
 */
void signal_install(struct  sigaction * sa, int signum, handler_t * handler) {
  assert(sa!=NULL);
  assert(handler!=NULL);

  sa->sa_handler = handler;
  sa->sa_flags=SA_RESTART;
  sigemptyset(&(sa->sa_mask));
  sigaction(signum, sa, (struct sigaction *)0);
}

/* sigusr1_handler : restart myscreen-stats */
void sigusr1_handler(int UNUSED(sig)){   
  stats_loop=FALSE;
  main_loop=TRUE;
  printf("Restarting ...\n");
  (void) (void)fflush(stdout);
  return;
}

/* sigusr2_handler : stop / restart  myscreen-stats */
void sigusr2_handler(int UNUSED(sig)){   

  stats_loop=FALSE;
  main_loop=FALSE;
  printf("Stopped : run \"myscreen restart\" to resume\n");
  (void) (void)fflush(stdout);
  return;
}

void install_signals(){

	/* Install signals */
	signal_install(&usr1, SIGUSR1, sigusr1_handler);
	signal_install(&usr2, SIGUSR2, sigusr2_handler);
	signal_install(&cont, SIGCONT, sigusr1_handler);
}
