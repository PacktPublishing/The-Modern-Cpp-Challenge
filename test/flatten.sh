#!/bin/sh

cd `dirname $0`
for dir in `\find . -maxdepth 1 -type d`; do
	mv $dir/main.cpp a${dir:10:2}.cpp
	rm -rf $dir
done
