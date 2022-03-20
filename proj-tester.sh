#!/bin/bash

make -s -C src
for infile in ./tests-public/*.in; do
    basename=${infile%.*}
    ./src/$1 < $infile > $basename.result
    cmp --silent $basename.result $basename.out || echo "$basename TEST FAILED"
    if [ "$2" == "-v" ]; then
        if ! command -v colordiff &> /dev/null; then
            diff $basename.result $basename.out
        else
            colordiff $basename.result $basename.out
        fi
    fi
done
rm -rf src/proj1

if [ "$2" == "-v" ]; then
    if ! command -v colordiff &> /dev/null
    then
        echo ""
        echo "NOTA: Instala colordiff para ver as diferenças a cores"
        exit
    fi
fi
