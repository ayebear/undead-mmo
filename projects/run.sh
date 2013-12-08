#!/bin/bash
LIBS=lib/linux/sfml2/lib
BIN=./ZombieGame
LD_LIBRARY_PATH="$LIBS":"$LD_LIBRARY_PATH" "$BIN"
