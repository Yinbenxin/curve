"""OPRF (Oblivious Pseudo-Random Function) Python bindings for Curve project.
"""
from typing import List
# 导入EcdhOPRF相关类
try:
    from .libecdh_oprf import EcdhOPRF, CurveType
except ImportError:
    pass  # 如果模块未编译，优雅地处理导入错误
import logging
