#!/bin/bash
LIBS=lib/linux/sfml2/lib
BIN=./ZombieServer
LD_LIBRARY_PATH="$LIBS":"$LD_LIBRARY_PATH" "$BIN"
