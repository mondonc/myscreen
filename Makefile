#!/bin/make -f
# This Makefile is part of myscreen project, under GPLv3
# Clément Mondon
#

#> Makefile to compile myscreen-stats binary used in myscreen package 
#> To disable module's compilation, please edit the Makefile and find the MODULES var 
PACKAGE=myscreen
TARGET := myscreen-stats

# External tools
RM=rm -f
DOC=doc
LOG_STATIC := log-static-analysis

# Compilation 
CC=gcc
INCLUDES = -I main/ -I modules/
CFLAGS = $(INCLUDES) -Wall -Werror -Wextra -ansi -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
LDFLAGS=
CPPFLAGS=

THIS_MAKEFILE := Makefile 

# MultiArch support (32/64)
ifdef DEB_HOST_ARCH_BITS
	ifeq ($(DEB_HOST_ARCH_BITS),32)
		MARCH=-m$(DEB_HOST_ARCH_BITS)
	endif
	ifeq ($(DEB_HOST_ARCH_BITS),66)
		MARCH=-m$(DEB_HOST_ARCH_BITS)
	endif
endif

# Modules support

# Used to disable compilation of specific modules, and define module's order
#MODULES :=  users uptime network battery wifi reboot loadaverage processes cpu disks_access disks_usage ram swap
MODULES :=  users uptime network battery wifi reboot loadaverage processes cpu disks_access disks_usage ram swap

MODULES_SRC := $(foreach dir, $(MODULES) , $(wildcard modules/$(dir)/*.c) )   
MODULES_OBJ := $(MODULES_SRC:%.c=%.o)
MAIN_SRC = main/myscreen-stats.c main/tools.c main/parse-config.c main/proc_tools.c
MAIN_OBJ = $(MAIN_SRC:%.c=%.o)
MODULE_LIST_H := main/modules_list.h 
MODULE_LIST_C := $(MODULE_LIST_H:%.h=%.c) 
MODULE_LIST_OBJ := $(MODULE_LIST_H:%.h=%.o) 
MODULES_COMMA_R := $(filter-out $(firstword $(MODULES)), $(MODULES))
MODULES_COMMA_QR := $(foreach mod,$(MODULES_COMMA_R),,"$(mod)")
MODULES_COMMA_RR := $(foreach mod,$(MODULES_COMMA_R),,$(mod))
MODULES_M := $(firstword $(MODULES)) $(MODULES_COMMA_RR)
MODULES_QM := "$(firstword $(MODULES))" $(MODULES_COMMA_QR)



# Configuration files
SCREEN_CONF=myscreen-screenrc
MYSCREEN_CONF=myscreenrc

# Install 
BIN_STATS=$(DESTDIR)/usr/lib/myscreen
BIN_MYSCREEN=$(DESTDIR)/usr/bin/
EXEC=myscreen

#
# TARGETS
#

# Build Target
default: CFLAGS += -DNDEBUG #To remove all asserts in final code
default: compile ## Compile to packaging ( with NDEBUG flag == without assert() )

# Build in DEBUG mode (with assert)
debug: CFLAGS += -DDEBUG -g ## Compile in debug mode
debug: compile 

# Build in ONESHOT mode (used to report bug or CI)
oneshot: CFLAGS += -DONESHOT ## Compile in oneshot mode
oneshot: debug

# Compile target
compile: $(TARGET)

$(TARGET): $(MYSCREEN_CONF) $(MODULE_LIST_H) $(MODULE_LIST_OBJ) $(MODULES_OBJ) $(MAIN_OBJ) 
	$(CC) $(MARCH) $(MODULES_OBJ) $(MODULE_LIST_OBJ) $(MAIN_OBJ) -o $(TARGET) $(LDFLAGS)

# Help
help : ## Display this help 
	@echo "---"
	@grep '^#>' Makefile | sed s/'#>'/' '/ 
	@echo "---"
	@grep '##' Makefile | grep -v "grep " | sed s/'^\(.*\):.*##\(.*\)'/'* \1 : \2'/ 

install: $(TARGET) ## Install myscreen
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

distclean : realclean
realclean : clean ## Clean target
	@-$(RM) $(TARGET) 
	@-$(RM) -f $(MYSCREEN_CONF)

archive : realclean ## Create archive tar.gz in ../
	#@REP=$$(basename $$PWD) ; cd ../ ; tar cvfz $(TARGET)-`date +%Y-%m-%d-%H-%M`.tar.gz  $${REP}/*
	@REP=$$(basename $$PWD) ; tar cvfz ../$(PACKAGE).tar.gz  *

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

# Dependencies
$(MODULE_LIST_H): $(THIS_MAKEFILE)
	@echo -n "Generating $(MODULE_LIST_H)..."
	@echo '\n/* This file is auto-generated */\n\n#ifndef _MODULES_H\n#define _MODULES_H\n\n#include "myscreen-stats.h"\n' >$(MODULE_LIST_H)
	@for m in $(MODULES) ; do echo "#include \"$${m}/$${m}.h\" " >> $(MODULE_LIST_H)   ; done 
	@echo '\n#define NB_MODULES_MAX $(words $(MODULES))\n' >> $(MODULE_LIST_H)
	@echo '\n#endif\n' >> $(MODULE_LIST_H)
	@echo " [OK]"

$(MODULE_LIST_C): $(THIS_MAKEFILE)
	@echo -n "Generating $(MODULE_LIST_C)..."
	@echo '\n/* This file is auto-generated */\n#include "$(notdir $(MODULE_LIST_H))"\n' >$(MODULE_LIST_C)
	@echo 'char * modules[] = { $(MODULES_QM) };' >> $(MODULE_LIST_C)
	@echo 'char * modules_color[] = {' `echo '$(MODULES_M)' | sed -e 's/\([a-zA-Z][a-z_A-Z]*\)/COLOR_\U\1/g'`  '};' >> $(MODULE_LIST_C)
	@echo 'char * (*main_mod[NB_MODULES_MAX])() = { $(MODULES_M) };' >> $(MODULE_LIST_C)
	@echo 'char * (*init_mod[NB_MODULES_MAX])(char * conf_line) = {' `echo '$(MODULES_M)' | sed -e 's/\([A-Za-z][a-z_A-Z]*\)/init_\1/g'`  '};' >> $(MODULE_LIST_C)
	@echo 'void (*exit_mod[NB_MODULES_MAX])(const char * conf_line) = {' `echo '$(MODULES_M)' | sed -e 's/\([A-Za-z][A-Za-z_]*\)/exit_\1/g'`  '};' >> $(MODULE_LIST_C)
	@echo " [OK]"

$(MYSCREEN_CONF): $(THIS_MAKEFILE)
	@echo -n "Generating $(MYSCREEN_CONF)...          "
# Header
	@echo "# Version 0.9" > $(MYSCREEN_CONF)
	@echo "# Auto-generated configuration file of myscreen" >> $(MYSCREEN_CONF)
	@echo "#" >> $(MYSCREEN_CONF)
	@echo "# SYNTAX:" >> $(MYSCREEN_CONF)
	@echo "#" >> $(MYSCREEN_CONF)
	@echo "# to activate a module      ->   module_name   OR   module_name = OPTION" >> $(MYSCREEN_CONF)
	@echo "# to desactivate a module   ->   comment it !" >> $(MYSCREEN_CONF)
	@echo "#" >> $(MYSCREEN_CONF)
	@echo "# If a module isn't mentioned, it's considerated as disable\n\n" >> $(MYSCREEN_CONF)
# Body
	@cat $(foreach mod,$(MODULES),modules/$(mod)/$(mod).conf) >> $(MYSCREEN_CONF)
	@echo " [OK]"


%.o : %.c %.h
	$(CC) $(MARCH) -o $@ -c $< $(CFLAGS)

manpage: myscreen.1
myscreen.1 : doc/manpage.md
	pandoc -s -t man doc/manpage.md -o myscreen.2

BUILD_DEPENDENCIES = sed libc6-dev make linux-libc-dev binutils libc6 coreutils gcc dash
install-dependencies:
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

cppcheck: .pkg_cppcheck
	@echo -n "" > $(LOG_STATIC)
	cppcheck $(INCLUDES) --enable=all `find . -name *.c` 1>/dev/null 2>>$(LOG_STATIC) 
	@if ! [ "`cat $(LOG_STATIC) | wc -l`" = "0" ] ; then cat $(LOG_STATIC) ; return 1 ; fi

$(PKG_ALL): 
	make -s $(basename $(filter %$(strip $(subst .pkg_, , $@)), $(PACKAGES) ) )

$(PKG_BIN) : 
	@echo "I can't find $@, trying to install it."
	apt-get -y install $(strip $(subst ., , $(suffix $(filter $@%, $(PACKAGES) ) ) ) )


.PHONY: clean realclean help new-module cppcheck
