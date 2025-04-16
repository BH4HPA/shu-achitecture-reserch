#!/bin/bash

./memory_test "$@" &
PID1=$!

./memory_test "$@"

wait $PID1
