#!/bin/bash

# 默认参数：array size（单位 MB）
ARRAY_SIZE=${1:-512}  # 如果没传就默认 512

# 构造命令并执行，同时保存日志
run_test() {
  local mem_limit=$1
  local swap_limit=$2
  local tag=$3

  echo ">>> Running test: memory=${mem_limit}m, swap=${swap_limit}m, array-size=${ARRAY_SIZE}MB"
  docker run --rm \
    --memory="${mem_limit}m" \
    --memory-swap="${swap_limit}m" \
    memory-bench \
    --array-size "${ARRAY_SIZE}" \
    --num-accesses 100000000 \
    --verbose \
  | tee "logs/mem${mem_limit}_array${ARRAY_SIZE}.log"
  echo ""
}

# 创建日志目录
mkdir -p logs

docker build -t memory-bench .

# 运行不同的配置
run_test 256 2048  mem256
run_test 512 2048  mem512
run_test 1024 2048 mem1024
run_test 2048 2048 mem2048