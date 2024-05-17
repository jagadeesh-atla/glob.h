#!/bin/sh

set -xe

gcc -Wall -Wextra -Wswitch-enum -ggdb -o test test.c
./test
