# Curve 项目

本项目基于 Bazel 构建，包含 OPRF（Oblivious Pseudorandom Function）相关实现。

## 快速开始

1. 安装 Bazel（推荐使用 bazelisk）：
   https://bazel.build/install

2. 克隆本仓库并进入目录：
   ```sh
   git clone https://github.com/Yinbenxin/curve.git
   cd curve
   ```

3. 运行基本测试：
   ```sh
   bazel run //oprf:basic_ecdh_oprf_test
   ```

## 目录结构

- `oprf/`：OPRF 相关源码及测试
- `bazel/`：Bazel 构建相关脚本

## 依赖管理

请确保 `MODULE.bazel` 配置正确，依赖可通过 `bazel mod tidy` 同步。

## 许可证

本项目遵循 Apache 2.0 License。