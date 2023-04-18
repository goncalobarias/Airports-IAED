#! /bin/bash

if [ $# != 1 ]; then
	echo "usage: $0 <path to tests-directory>"
	exit
fi

if [ -d "extracted-tests" ]; then
	echo "Please rename or delete your "extracted-tests" folder"
	exit
fi

mkdir extracted-tests

for dir in "$1"/*
do
	for test in $(ls $dir)
	do
		if [ ${#test} -gt 5 ]; then
			cp $dir/$test extracted-tests/$(basename "$dir").out
		else
			cp $dir/$test extracted-tests/$(basename "$dir").in
		fi
	done
done
