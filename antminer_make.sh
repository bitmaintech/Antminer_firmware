#!/bin/sh

set -x
bitbake -c compile monitor-recobtn -f -D
bitbake -c compile initc-bin -f -D
bitbake -c compile lighttpd -f -D
bitbake -c compile initscripts -f -D
bitbake S3-MM -f -D

