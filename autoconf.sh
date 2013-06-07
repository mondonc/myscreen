#!/bin/bash

cat `cat modules.conf |  sed 's/^[ \t]*//;s/[ \t]*$//' | sed '/^$/d' | sed 's/\([a-z][a-z_]*\)/\1\/\1/g' | sed 's/^/modules\//g' |  sed 's/$/.conf/g'`
