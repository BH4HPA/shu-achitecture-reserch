#!/bin/bash

ARRAY_SIZE=${1:-512}
NUM_ACCESSES=100000000
LOG_DIR="logs"
mkdir -p "$LOG_DIR"

run_test() {
  local mem_limit=$1

  echo ">>> Running test: memory=${mem_limit}m, array-size=${ARRAY_SIZE}MB"

  local log_file="${LOG_DIR}/mem${mem_limit}_array${ARRAY_SIZE}.log"

  docker run --rm \
    --memory="${mem_limit}m" \
    --memory-swap="-1" \
    memory-bench \
    --array-size "${ARRAY_SIZE}" \
    --num-accesses "${NUM_ACCESSES}" \
    --verbose \
  | tee "$log_file"

  local code=${PIPESTATUS[0]}

  if [[ $code -ne 0 ]]; then
    echo -e "\033[0;31m[Error] Test failed: mem=${mem_limit}m array=${ARRAY_SIZE}MB (exit code: $code)\033[0m"
  else
    echo -e "\033[0;32m[OK] Test passed: mem=${mem_limit}m array=${ARRAY_SIZE}MB\033[0m"
  fi
  echo ""
}

docker build -t memory-bench .

run_test 256
run_test 512
run_test 1024
run_test 2048
