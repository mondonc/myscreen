#ifndef  _TOOLS_H
#define  _TOOLS_H

typedef void handler_t(int);
void install_signals(); /* Install sigaction function */

void strncpyclr(char * dest, const char * src, unsigned int len); /* Copy and clear src string to dest, max len char */
void strnclr(char * src, unsigned int len); /* Clear src string, max len char */

int myprint_percentage(char * result, unsigned short percentage); /*Print percentage to result, maxsize: 5*/
int myprint_percentage_s(char * result, unsigned short percentage); /*Print percentage with space to result, maxsize: 6*/

void error(const char * err, unsigned short fatal); /*Display error and exit with exit_failure if fatal is true */

#endif
