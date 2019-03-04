#!/bin/bash

while true;
do
	inotifywait -e modify,delete,close_write,move,create *.cc *.cpp *.h
	bash $(dirname "${0}")/build_rapid.sh
done
