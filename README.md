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

## 静态链接构建

为了避免对特定版本的系统库（如libc.so.6）的依赖，可以使用静态链接方式构建：

```sh
# 在本地构建静态链接版本
./build_static.sh

# 或者在Docker容器中构建
./docker/build_in_docker.sh
```

详细信息请参考 [静态链接指南](STATIC_LINKING.md)。

## 目录结构

- `oprf/`：OPRF 相关源码及测试
- `bazel/`：Bazel 构建相关脚本

## 依赖管理

请确保 `MODULE.bazel` 配置正确，依赖可通过 `bazel mod tidy` 同步。

## 许可证

本项目遵循 Apache 2.0 License。