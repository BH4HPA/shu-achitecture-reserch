#!/bin/bash

./memory_test "$@" &
PID1=$!

./memory_test "$@"  # 前台运行，输出自然会显示

wait $PID1
