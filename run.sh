#!/bin/bash

./memory_test "$@" > >(tee /dev/stdout) &
PID1=$!

./memory_test "$@"  # 前台运行，输出自然会显示

wait $PID1
