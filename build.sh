#!/bin/sh

set -xe

mkdir -p build
cd build

cc -Wall -Wextra -pedantic ../src/main.c -o gucker -lraylib
