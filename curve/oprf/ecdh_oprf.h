// Copyright 2022 Ant Group Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "psi/algorithm/ecdh/ub_psi/basic_ecdh_oprf.h"
#include "psi/algorithm/ecdh/ub_psi/ecdh_oprf_selector.h"

namespace psi::ecdh {

class EcdhOPRF {
 public:
  /**
   * @brief 构造函数
   * 
   * @param sk 16进制字符串格式的密钥，如果为空则随机生成
   * @param curve_type 曲线类型: 支持  FourQ/SM2/Secp256k1
   */
  EcdhOPRF(const std::string& sk = "", CurveType curve_type = CurveType::CURVE_SECP256K1);
  
  ~EcdhOPRF();

  /**
   * @brief 导出密钥（16进制字符串格式）
   * 
   * @return 16进制字符串格式的密钥
   */
  std::string GetKey() const;
  
  /**
   * @brief 全量评估
   * 
   * @param input 输入数据
   * @return 评估结果
   */
  std::vector<std::string> FullEvaluate(const std::vector<std::string>& input) const;
  
  /**
   * @brief 盲化
   * 
   * @param inputs 输入数据
   * @return 盲化后的数据
   */
  std::vector<std::string> Blind(const std::vector<std::string>& inputs) const;
  
  /**
   * @brief 评估
   * 
   * @param blinded_elements 盲化后的数据
   * @return 评估结果
   */
  std::vector<std::string> Evaluate(const std::vector<std::string>& blinded_elements) const;
  
  /**
   * @brief 完成
   * 
   * @param inputs 原始输入数据
   * @param evaluated_elements 评估后的数据
   * @return 最终结果
   */
  std::vector<std::string> Finalize(const std::vector<std::string>& inputs, 
                                  const std::vector<std::string>& evaluated_elements) const;

 private:
  std::shared_ptr<IEcdhOprfServer> dh_oprf_server_;
  std::shared_ptr<IEcdhOprfClient> dh_oprf_client_;
  std::vector<uint8_t> sk_;
  CurveType curve_type_;
};

}  // namespace psi::ecdh