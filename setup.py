from setuptools import setup, find_packages, Extension
from setuptools.command.build_ext import build_ext
import os
import subprocess
import sys

# 安装开发版本
# python setup.py develop

# 构建分发包
# python setup.py sdist bdist_wheel

# 安装
# pip install .

class BazelExtension(Extension):
    """用于从Bazel构建的Python扩展"""
    def __init__(self, name, bazel_target):
        super().__init__(name, sources=[])
        self.bazel_target = bazel_target

class BazelBuildExt(build_ext):
    """使用Bazel构建扩展的自定义构建命令"""
    def run(self):
        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        if not isinstance(ext, BazelExtension):
            super().build_extension(ext)
            return

        # 确保输出目录存在
        os.makedirs(self.build_temp, exist_ok=True)
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        os.makedirs(extdir, exist_ok=True)

        # 调用Bazel构建目标
        subprocess.check_call(
            ['bazel', 'build', ext.bazel_target],
            cwd=os.path.abspath(os.path.dirname(__file__))
        )

        # 找到构建的.so文件并复制到正确的位置
        bazel_bin_dir = os.path.join(
            os.path.abspath(os.path.dirname(__file__)),
            'bazel-bin', 'pycurve', 'oprf'
        )
        
        # 根据操作系统确定扩展名
        if sys.platform == 'darwin':
            ext_suffix = '.so'
        elif sys.platform == 'win32':
            ext_suffix = '.pyd'
        else:  # Linux和其他类Unix系统
            ext_suffix = '.so'
            
        # 复制libvolepsi.so到目标目录
        src_file = os.path.join(bazel_bin_dir, 'libecdh_oprf' + ext_suffix)
        dst_file = os.path.join(extdir, 'libecdh_oprf' + ext_suffix)
        
        if os.path.exists(src_file):
            self.copy_file(src_file, dst_file)
        else:
            raise RuntimeError(f"找不到构建的扩展文件: {src_file}")
setup(
    name="pycurve",
    version="0.1.0",
    author="Ant Group",
    description="Curve project with OPRF (Oblivious Pseudo-Random Function) implementation",
    long_description=open("README.md", "r").read() if os.path.exists("README.md") else "",
    long_description_content_type="text/markdown",
    url="https://github.com/Yinbenxin/curve",
    packages=find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: Apache Software License",
        "Operating System :: OS Independent",
    ],
    python_requires=">=3.6",
    # 使用正确的目标名称
    ext_modules=[BazelExtension("pycurve.oprf.libecdh_oprf", "//pycurve/oprf:libecdh_oprf_alias")],
    cmdclass={
        'build_ext': BazelBuildExt,
    },
    install_requires=[
        # 添加依赖项
    ],
)