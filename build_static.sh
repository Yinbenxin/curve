#!/bin/bash

# 构建静态链接版本的库

set -e

# 清理之前的构建
rm -rf build dist pycurve.egg-info

# 检查操作系统
if [[ "$(uname)" == "Linux" ]]; then
    echo "在Linux环境下构建，使用静态链接..."
    
    # 确保安装了必要的静态库
    if [ -x "$(command -v apt-get)" ]; then
        echo "检测到Debian/Ubuntu系统，安装必要的静态库..."
        sudo apt-get update
        sudo apt-get install -y libc6-dev glibc-source
    elif [ -x "$(command -v yum)" ]; then
        echo "检测到RHEL/CentOS系统，安装必要的静态库..."
        sudo yum install -y glibc-static
    fi
    
    # 使用Bazel构建
    echo "使用Bazel构建静态链接库..."
    bazel build --config=linux-static --config=linux-release //pycurve/oprf:libecdh_oprf_alias
    
    # 使用Python setup.py构建
    echo "使用Python setup.py构建..."
    python setup.py build
else
    echo "在非Linux环境下构建，使用默认链接方式..."
    python setup.py build
fi

# 运行检查脚本
echo "运行检查脚本..."
./check_static_link.sh

echo "构建完成！"