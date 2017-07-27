#!/bin/bash

INFO=`sysctl -n machdep.cpu.brand_string`
echo $INFO
if [[ $INFO == *"Intel"* ]]
then
	echo "#define CPU INTEL" > config.h
fi
INFO=`uname -a`
echo $INFO
if [[ $INFO == *"64"* ]]
then
	echo "#define WORD_LEN 64" >> config.h
elif [[ $INFO == *"32"* ]]
then
	echo "#define WORD_LEN 32" >> config.h
fi