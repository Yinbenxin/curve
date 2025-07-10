"""OPRF (Oblivious Pseudo-Random Function) Python bindings for Curve project.

OPRF (Oblivious Pseudo-Random Function) 是一种密码学原语，允许客户端获取函数F(k,x)的值，
其中k是服务器持有的密钥，x是客户端的输入，且服务器无法获知x的值，客户端也无法获知k的值。

EcdhOPRF是基于椭圆曲线Diffie-Hellman密钥交换的OPRF实现，主要用于隐私保护计算和安全多方计算场景。

主要功能：
- 支持多种椭圆曲线：SECP256K1、SM2、FOURQ
- 提供完整的OPRF协议实现：盲化(Blind)、评估(Evaluate)、完成(Finalize)
- 支持一步式评估(FullEvaluate)，简化使用流程

典型应用场景：
- 隐私保护集合求交(PSI)
- 密码验证
- 匿名凭证
- 安全令牌生成

使用示例见ecdh_oprf_test.py
"""

from .libecdh_oprf import EcdhOPRF as LibEcdhOPRF, CurveType

# 定义曲线类型枚举值的含义
CURVE_SECP256K1 = CurveType.CURVE_SECP256K1  # SECP256K1椭圆曲线，比特币和以太坊使用的曲线
CURVE_SM2 = CurveType.CURVE_SM2              # SM2椭圆曲线，中国国家密码标准
CURVE_FOURQ = CurveType.CURVE_FOURQ          # FourQ椭圆曲线，高性能曲线实现

__all__ = [
    'EcdhOPRF',        # ECDH-OPRF主类
    'CurveType',        # 曲线类型枚举
    'CURVE_SECP256K1',  # SECP256K1曲线类型
    'CURVE_SM2',        # SM2曲线类型
    'CURVE_FOURQ',      # FOURQ曲线类型
]

# 定义EcdhOPRF类，作为LibEcdhOPRF的包装器
class EcdhOPRF:
    """EcdhOPRF类，基于椭圆曲线Diffie-Hellman密钥交换的OPRF实现。
    
    该类提供了完整的OPRF协议实现，包括盲化(Blind)、评估(Evaluate)和完成(Finalize)，
    以及一步式评估(FullEvaluate)功能。
    
    Args:
        sk (str, optional): 16进制字符串格式的密钥，如果为空则随机生成. Defaults to "".
        curve_type (CurveType, optional): 曲线类型. Defaults to CurveType.CURVE_SECP256K1.
    """
    def __init__(self, sk="", curve_type=CurveType.CURVE_SECP256K1):
        """初始化EcdhOPRF实例
        
        Args:
            sk (str, optional): 16进制字符串格式的密钥，如果为空则随机生成. Defaults to "".
            curve_type (CurveType, optional): 曲线类型. Defaults to CurveType.CURVE_SECP256K1.
        """
        self._oprf = LibEcdhOPRF(sk, curve_type)
    
    def GetKey(self):
        """获取密钥（16进制字符串格式）
        
        Returns:
            str: 16进制字符串格式的密钥
        """
        return self._oprf.GetKey()
    
    def FullEvaluate(self, inputs):
        """全量评估
        
        Args:
            inputs (List[bytes]): 输入数据列表
            
        Returns:
            List[bytes]: 评估结果列表
        """
        return self._oprf.FullEvaluate(inputs)
    
    def Blind(self, inputs):
        """盲化
        
        Args:
            inputs (List[bytes]): 输入数据列表
            
        Returns:
            List[bytes]: 盲化后的数据列表
        """
        return self._oprf.Blind(inputs)
    
    def Evaluate(self, blinded_elements):
        """评估
        
        Args:
            blinded_elements (List[bytes]): 盲化后的数据列表
            
        Returns:
            List[bytes]: 评估结果列表
        """
        return self._oprf.Evaluate(blinded_elements)
    
    def Finalize(self, inputs, evaluated_elements):
        """完成
        
        Args:
            inputs (List[bytes]): 原始输入数据列表
            evaluated_elements (List[bytes]): 评估后的数据列表
            
        Returns:
            List[bytes]: 最终结果列表
        """
        return self._oprf.Finalize(inputs, evaluated_elements)


