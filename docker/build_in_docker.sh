#!/bin/bash

# 在Docker容器中构建静态链接版本

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# 构建Docker镜像
echo "构建Docker镜像..."
docker build -t curve-static-build "$SCRIPT_DIR"

# 在Docker容器中运行构建
echo "在Docker容器中运行构建..."
docker run --rm -v "$PROJECT_ROOT:/home/admin/curve" curve-static-build bash -c "cd /home/admin/curve && ./build_static.sh"

echo "Docker构建完成！"