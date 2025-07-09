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

#include "curve/oprf/ecdh_oprf.h"

#include <future>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "absl/strings/escaping.h"
#include "spdlog/spdlog.h"
#include "yacl/base/exception.h"
#include "yacl/crypto/rand/rand.h"
#include "yacl/crypto/tools/prg.h"

namespace psi::ecdh {

EcdhOPRF::EcdhOPRF(const std::string& sk, CurveType curve_type) {
  // 初始化服务端
  curve_type_ = curve_type;
  if (sk.empty()) {
    SPDLOG_WARN("oprf sk is empty");
    std::vector<uint8_t> new_sk(kEccKeySize);
    yacl::crypto::Prg<uint64_t> prg(yacl::crypto::SecureRandU64());
    prg.Fill(absl::MakeSpan(new_sk));
    dh_oprf_server_ = CreateEcdhOprfServer(new_sk, OprfType::Basic, curve_type);
    dh_oprf_client_ = CreateEcdhOprfClient(new_sk, OprfType::Basic, curve_type);
    sk_ = new_sk;
  } else {
    // 如果sk不为空，则将16进制字符串转换为二进制格式
    std::string binary_sk;
    std::vector<uint8_t> sk_vec;
    if (absl::HexStringToBytes(sk, &binary_sk)) {
      sk_vec.assign(binary_sk.begin(), binary_sk.end());
    } else {
      SPDLOG_ERROR("Failed to convert hex string to bytes");
    }
    dh_oprf_server_ = CreateEcdhOprfServer(sk_vec, OprfType::Basic, curve_type);
    dh_oprf_client_ = CreateEcdhOprfClient(sk_vec, OprfType::Basic, curve_type);
    sk_ = sk_vec;
  }
}

EcdhOPRF::~EcdhOPRF() {}

std::string EcdhOPRF::GetKey() const {
  return absl::BytesToHexString(absl::string_view(reinterpret_cast<const char*>(sk_.data()), sk_.size()));
}

std::vector<std::string> EcdhOPRF::FullEvaluate(const std::vector<std::string>& input) const {
  SPDLOG_INFO("oprf full evaluate, input size: {}", input.size());
  return dh_oprf_server_->FullEvaluate(input);
}

std::vector<std::string> EcdhOPRF::Blind(const std::vector<std::string>& inputs) const {
  SPDLOG_INFO("oprf blind, input size: {}", inputs.size());
  return dh_oprf_client_->Blind(inputs);
}

std::vector<std::string> EcdhOPRF::Evaluate(const std::vector<std::string>& blinded_elements) const {
  SPDLOG_INFO("oprf evaluate, input size: {}", blinded_elements.size());
  return dh_oprf_server_->Evaluate(blinded_elements);
}

std::vector<std::string> EcdhOPRF::Finalize(const std::vector<std::string>& inputs, 
                                      const std::vector<std::string>& evaluated_elements) const {
  SPDLOG_INFO("oprf finalize, input size: {}, evaluated_elements size: {}", inputs.size(), evaluated_elements.size());
  return dh_oprf_client_->Finalize(inputs, evaluated_elements);
}

}  // namespace psi::ecdh