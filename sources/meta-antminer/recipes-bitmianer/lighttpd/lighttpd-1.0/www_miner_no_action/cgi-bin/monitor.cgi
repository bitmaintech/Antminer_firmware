#!/bin/sh

top -b -n 1 | while read LINE
do
	if [ -z ${LINE} ]; then
		break
	fi
	echo "${LINE}"
done