Tests and Continuous integration
===

Results are available on the [Continuous integration](http://myscreen.mondonc.org/commits) page.

Continuous integration is provided by a simply bash [script](http://myscreen.mondonc.org/myscreen-ci.sh).

It tests : 

1. Compilation
2. Asserts 
3. Static analysis of code
4. Autobuild debian package

Unit tests
---

Unit tests are provided by *assert*. Asserts should be wrote to ensure the state of program.

For instance, before using pointer, you can assert that it is not *NULL*.

	assert(pointer != NULL);

But asserts are only present when you compile in **debug** or **oneshot** mode. In the default mode (using make without argument), the flag *-NDEBUG* remove  all asserts ! So please, use it only to test your code and prevent bad future modifications. If you want to assert something in your algo, please use an *if* instead.

Sources overview
===

Main files
---

* *main/* : contains... the main files ! For instance, the main loop to display stats every 2 seconds. 

* *Makefile* : run make help for details.

Modules 
---

Each module have to respect a convention. For instance, for a module called "mymodule", you must find :  

* a subdirectory *modules/mymodule*

This subdirectory contains all files of your module.

* a file modules *modules/mymodule/mymodule.c*

This file have to declare some function : 

	char * init_mymodule(char * conf_line); : called when myscreen starting or reloading, the parameter is the configuration line (at the right of the '=')

	char * mymodule(); : this function is called at every loop and returns the string that will be displayed.

	void exit_mymodule(); : called when myscreen exiting 

* a file modules *modules/mymodule/mymodule.h*

This file contains at least : 

	#ifndef _MYMODULE_H
	#define _MYMODULE_H
	#define COLOR_MYMODULE YELLOW
	#include <myscreen-stats.h>

	char * mymodule();
	char * init_mymodule(char * line);
	void exit_mymodule();

	#endif

* a file *modules/mymodule/mymodule.conf*

This file is used to generate the global configuration file : myscreenrc

It should be like : 

  #Mymodule : short description for users
	mymodule = configuration line

For instance :
 
	# loadaverage : Loadaverage
	# OPTIONS : 1 or 5 or 15 (default is 1) to define time in minute (see man uptime for details)
	loadaverage = 1


Write a new module
===

To write a new module, you can use scripts/mkmodule with make new-module.
