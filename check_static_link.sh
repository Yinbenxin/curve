#!/bin/bash

# 清理之前的构建
rm -rf build dist pycurve.egg-info

# 使用修改后的配置重新构建
python setup.py build

# 检查生成的.so文件是否静态链接了libstdc++
echo "检查libecdh_oprf.so的依赖关系："
if [[ "$(uname)" == "Darwin" ]]; then
    # macOS使用otool
    otool -L build/lib*/pycurve/oprf/libecdh_oprf.so
else
    # Linux使用ldd
    ldd build/lib*/pycurve/oprf/libecdh_oprf.so
    
    # 检查是否包含GLIBCXX_3.4.29符号
    echo "\n检查是否包含GLIBCXX_3.4.29符号："
    strings build/lib*/pycurve/oprf/libecdh_oprf.so | grep GLIBCXX_3.4.29
    
    # 检查是否静态链接了libstdc++
    echo "\n检查是否静态链接了libstdc++："
    if ! ldd build/lib*/pycurve/oprf/libecdh_oprf.so | grep -q libstdc++.so; then
        echo "成功：libstdc++已静态链接"
    else
        echo "失败：libstdc++仍然是动态链接"
    fi
fi