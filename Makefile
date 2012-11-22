#!/bin/make -f
# Clément Mondon

PACKAGE=myscreen

#External tools
RM=rm -f
DOC=doc
LOG_STATIC := log-static-analysis

#Compilation 
CC=gcc
INCLUDES = -I main/ -I modules/
CFLAGS = $(INCLUDES) -Wall -Werror -Wextra -ansi -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
LDFLAGS=
CPPFLAGS=

# Modules support
MODULES_SRC := $(foreach dir, $(wildcard modules/*) , $(wildcard $(dir)/*.c) )   
MODULES_OBJ := $(MODULES_SRC:%.c=%.o)
MAIN_SRC = main/myscreen-stats.c main/tools.c main/parse-config.c    
MAIN_OBJ = $(MAIN_SRC:%.c=%.o)
MODULE_LIST_H := main/modules_list.h 
MODULE_LIST_C := $(MODULE_LIST_H:%.h=%.c) 
MODULE_LIST_OBJ := $(MODULE_LIST_H:%.h=%.o) 

# Final Target
TARGET := myscreen-stats

#Configuration files
SCREEN_CONF=myscreen-screenrc
MYSCREEN_CONF=myscreenrc

#Install 
BIN_STATS=$(DESTDIR)/usr/lib/myscreen
BIN_MYSCREEN=$(DESTDIR)/usr/bin/
EXEC=myscreen

#*#*#*#*#*#*#
#* Targets *#
#*#*#*#*#*#*#

#Build Target
default: CFLAGS += -DNDEBUG #To remove all asserts in final code
default: compile ## Compile to packaging ( with NDEBUG flag == without assert() )

#Build in DEBUG mode (with assert)
debug: CFLAGS += -DDEBUG -g ## Compile in debug mode
debug: compile 

#Compile target
compile: $(TARGET)
	
$(TARGET):	$(MODULE_LIST_H) $(MODULE_LIST_OBJ) $(MODULES_OBJ) $(MAIN_OBJ) 
$(CC): $(MODULES_OBJ) $(MODULE_LIST_OBJ) $(MAIN_OBJ) -o $(TARGET) $(LDFLAGS)

help : dump ## Display help 
		@grep '##' Makefile | grep -v "grep " | sed s/'##'/'\n    '/ | sed s/$$/'\n'/

build-local-install: DESTDIR = $(HOME)/.myscreen
build-local-install: CFLAGS += -DLOCAL
build-local-install: pre-build-local-install realclean $(TARGET) install ## Compile and install in home directory  
	sed  -e 's&CONF_FILE=.*&CONF_FILE=$(DESTDIR)/etc/$(SCREEN_CONF)&' -i $(BIN_MYSCREEN)/$(EXEC)
	sed  -e 's&/usr/lib/myscreen/myscreen-stats&$(BIN_STATS)/$(TARGET)&' -i $(HOME)/.myscreen/etc/$(SCREEN_CONF)
	@if grep  'PATH=$${PATH}:$${HOME}/.myscreen/usr/bin' $${HOME}/.bashrc ; then \
	echo ".bashrc is updated"  ;\
	else \
	echo "update"  ;\
	echo 'PATH=$${PATH}:$${HOME}/.myscreen/usr/bin' >> $${HOME}/.bashrc ;\
	fi
	export PATH=$${PATH}:$${HOME}/.myscreen/usr/bin


pre-build-local-install: 
	mkdir -p $(DESTDIR)

install: $(TARGET) ##Install myscreen
		install -d $(DESTDIR)/etc/
		cp $(SCREEN_CONF) $(DESTDIR)/etc/
		cp $(MYSCREEN_CONF) $(DESTDIR)/etc/
		install -d $(BIN_STATS)
		install $(TARGET) $(BIN_STATS)
		install -d $(BIN_MYSCREEN)
		install $(EXEC) $(BIN_MYSCREEN)
	
clean: ## Remove *.o
	@-$(RM) -f $(LOG_STATIC)
	@-$(RM) -f `find . -name *.o` $(MODULE_LIST_H) $(MODULE_LIST_C)

realclean : clean ## Clean target, doc also
	@-$(RM) $(TARGET) 
	@-$(RM) -r $(DOC) 

archive : realclean ## Create archive tar.gz in ../
	#@REP=$$(basename $$PWD) ; cd ../ ; tar cvfz $(TARGET)-`date +%Y-%m-%d-%H-%M`.tar.gz  $${REP}/*
	@REP=$$(basename $$PWD) ; tar cvfz ../$(PACKAGE).tar.gz  *


$(MODULE_LIST_H): $(MODULES_SRC:%.c=%.h)
	@echo -n "Generating $(MODULE_LIST_H)..."
	@echo '\n/* This file is auto-generated */\n\n#ifndef _MODULES_H\n#define _MODULES_H\n\n#include "myscreen-stats.h"\n' >$(MODULE_LIST_H)
	@for m in `ls modules` ; do echo "#include \"$${m}/$${m}.h\" " >> $(MODULE_LIST_H)   ; done 
	@NB_MOD=`ls modules/ | wc -w` ; echo '\n#define NB_MODULES ' `expr 1 + $${NB_MOD}` '\n' >> $(MODULE_LIST_H)
	@echo '\n#endif\n' >> $(MODULE_LIST_H)
	@echo " [OK]"

$(MODULE_LIST_C): $(MODULES_SRC:%.c=%.h)
	@echo -n "Generating $(MODULE_LIST_C)..."
	@echo '\n/* This file is auto-generated */\n#include "$(notdir $(MODULE_LIST_H))"\n' >$(MODULE_LIST_C)
	@echo 'char * modules[] = {' `ls -mQ modules` '};' >> $(MODULE_LIST_C)
	@echo 'char * modules_color[] = {' `ls -m modules | sed -e 's/\([a-z][a-z_]*\)/COLOR_\U\1/g'`  '};' >> $(MODULE_LIST_C)
	@echo 'char * (*main_mod[NB_MODULES])() = {' `ls -m modules` '};' >> $(MODULE_LIST_C)
	@echo 'char * (*init_mod[NB_MODULES])(char * conf_line) = {' `ls -m modules | sed -e 's/\([a-z][a-z_]*\)/init_\1/g'`  '};' >> $(MODULE_LIST_C)
	@echo 'void (*exit_mod[NB_MODULES])(const char * conf_line) = {' `ls -m modules | sed -e 's/\([a-z][a-z_]*\)/exit_\1/g'`  '};' >> $(MODULE_LIST_C)
	@echo " [OK]"

%.o : %.c %.h
	$(CC) -o $@ -c $< $(CFLAGS) 


BUILD_DEPENDENCIES = libmpc2 libgmp10 sed libc6-dev cpp-4.7 gcc-4.7 libselinux1 libmpfr4 make libacl1 locales libc6-i686 linux-libc-dev libattr1 binutils zlib1g libc6 coreutils gcc dash

install-depencies:
	apt-get -y install ${BUILD_DEPENDENCIES}

#*#*#*#*#*#*#*#*#*#*#*#*#*#*#
# Static analysis and proof #
#*#*#*#*#*#*#*#*#*#*#*#*#*#*#



# Packages : path_to_binary.package_name
PACKAGES := /usr/bin/cppcheck.cppcheck /usr/bin/splint.splint /usr/bin/git.git /usr/bin/dch.devscripts /usr/bin/lftp.lftp
PKG_BIN := $(basename $(PACKAGES))
PKG_NAME_WITH_DOT := $(suffix $(PACKAGES))
PKG_ALL := $(subst ., .pkg_, $(PKG_NAME_WITH_DOT))
PKG_NAME := $(subst ., , $(PKG_NAME_WITH_DOT))

# TODO : CLANG, splint, sparse
static-analysis : cppcheck ## Static analysis of source code with cppcheck, splint
	@echo "\nStandard output writed in $(LOG_STATIC)\n"

cppcheck: .pkg_cppcheck
	@echo `date -I` > $(LOG_STATIC)
	cppcheck $(INCLUDES) --enable=all `find . -name *.c` >> $(LOG_STATIC) 

$(PKG_ALL): 
	make -s $(basename $(filter %$(strip $(subst .pkg_, , $@)), $(PACKAGES) ) )

$(PKG_BIN) : 
	@echo "/!\ I can't find $@, trying to install it."
	sudo apt-get -y install $(strip $(subst ., , $(suffix $(filter $@%, $(PACKAGES) ) ) ) )


.PHONY: dump clean realclean help new-module cppcheck