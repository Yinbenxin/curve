#!/usr/bin/env python3
# Copyright 2024 Ant Group Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import unittest
import os
import sys
import random
import string
import binascii

# 导入EcdhOPRF模块
from libecdh_oprf import EcdhOPRF, CurveType

class TestEcdhOPRF(unittest.TestCase):

    def sample(self, curve_type, ele_num, ele_size):
        # 创建EcdhOPRF实例，使用默认参数（空密钥和SECP256K1曲线）
        oprf = EcdhOPRF("", curve_type)
        
        # 获取生成的密钥
        key = oprf.GetKey()
        self.assertTrue(len(key) > 0)
        
        # 使用相同密钥创建另一个实例
        oprf2 = EcdhOPRF(key)
        self.assertEqual(oprf2.GetKey(), key)
        
        # 测试OPRF功能
        inputs = [os.urandom(ele_size) for _ in range(ele_num)]
        
        # 全量评估
        results = oprf.FullEvaluate(inputs)
        self.assertEqual(len(results), len(inputs))
        
        # 测试盲化-评估-完成流程
        blinded = oprf.Blind(inputs)
        evaluated = oprf.Evaluate(blinded)
        finalized = oprf.Finalize(inputs, evaluated)
        
        # 验证结果一致性
        self.assertEqual(results, finalized)

    def test_different_curve_types(self):
        # 测试不同曲线类型
        ele_num = 10
        ele_size =100
        for curve_type in [CurveType.CURVE_SECP256K1, CurveType.CURVE_SM2, CurveType.CURVE_FOURQ]:
            self.sample(curve_type, ele_num, ele_size)

if __name__ == "__main__":
    unittest.main()