# 静态链接指南

本文档说明如何使用静态链接方式构建项目，以避免对特定版本的系统库（如libc.so.6）的依赖。

## 背景

动态链接的库文件（.so）依赖于系统中的特定版本的库，例如libc.so.6。当在不同的系统环境中运行时，如果目标系统的库版本低于编译环境的库版本，就会出现类似以下错误：

```
ImportError: /lib/aarch64-linux-gnu/libc.so.6: version `GLIBC_2.32' not found (required by /usr/local/lib/python3.8/dist-packages/pycurve/oprf/libecdh_oprf.so)
```

## 解决方案

为了解决这个问题，我们实现了完全静态链接，包括libc库。这样生成的库文件不再依赖于系统的动态库版本。

### 实现方式

1. 在Bazel构建配置中添加了`linux-static`配置，启用完全静态链接
2. 在Python扩展模块的构建脚本中，针对Linux环境使用静态链接选项
3. 在C++库的构建配置中，添加了静态链接的链接选项

## 如何构建

使用以下命令构建项目：

```bash
# 清理之前的构建
rm -rf build dist pycurve.egg-info

# 构建项目
python setup.py build
```

## 验证静态链接

使用提供的脚本检查静态链接情况：

```bash
./check_static_link.sh
```

该脚本会检查生成的.so文件是否静态链接了libstdc++和libc库。

## 注意事项

1. 静态链接会增加库文件的大小
2. 静态链接的库不会自动获得系统库的安全更新
3. 在某些环境中可能需要额外的权限才能执行静态链接的二进制文件

## 故障排除

如果遇到构建问题，可以尝试以下方法：

1. 确保已安装所有必要的开发库和工具
2. 检查Bazel版本是否兼容
3. 在Linux环境中，可能需要安装静态库版本的libc和其他系统库