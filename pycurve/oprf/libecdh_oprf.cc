// Copyright 2024 Ant Group Co., Ltd.
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

#include <string>
#include <vector>

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "curve/oprf/ecdh_oprf.h"
#include "psi/proto/psi.pb.h"

namespace py = pybind11;

// 包装函数，处理字节数据转换
std::vector<py::bytes> PyFullEvaluate(const psi::ecdh::EcdhOPRF& self, const std::vector<py::bytes>& inputs) {
  std::vector<std::string> cpp_inputs;
  cpp_inputs.reserve(inputs.size());
  for (const auto& input : inputs) {
    cpp_inputs.push_back(input);
  }
  
  auto results = self.FullEvaluate(cpp_inputs);
  
  std::vector<py::bytes> py_results;
  py_results.reserve(results.size());
  for (const auto& result : results) {
    py_results.push_back(py::bytes(result));
  }
  
  return py_results;
}

std::vector<py::bytes> PyBlind(const psi::ecdh::EcdhOPRF& self, const std::vector<py::bytes>& inputs) {
  std::vector<std::string> cpp_inputs;
  cpp_inputs.reserve(inputs.size());
  for (const auto& input : inputs) {
    cpp_inputs.push_back(input);
  }
  
  auto results = self.Blind(cpp_inputs);
  
  std::vector<py::bytes> py_results;
  py_results.reserve(results.size());
  for (const auto& result : results) {
    py_results.push_back(py::bytes(result));
  }
  
  return py_results;
}

std::vector<py::bytes> PyEvaluate(const psi::ecdh::EcdhOPRF& self, const std::vector<py::bytes>& blinded_elements) {
  std::vector<std::string> cpp_blinded_elements;
  cpp_blinded_elements.reserve(blinded_elements.size());
  for (const auto& element : blinded_elements) {
    cpp_blinded_elements.push_back(element);
  }
  
  auto results = self.Evaluate(cpp_blinded_elements);
  
  std::vector<py::bytes> py_results;
  py_results.reserve(results.size());
  for (const auto& result : results) {
    py_results.push_back(py::bytes(result));
  }
  
  return py_results;
}

std::vector<py::bytes> PyFinalize(const psi::ecdh::EcdhOPRF& self, 
                                  const std::vector<py::bytes>& inputs, 
                                  const std::vector<py::bytes>& evaluated_elements) {
  std::vector<std::string> cpp_inputs;
  cpp_inputs.reserve(inputs.size());
  for (const auto& input : inputs) {
    cpp_inputs.push_back(input);
  }
  
  std::vector<std::string> cpp_evaluated_elements;
  cpp_evaluated_elements.reserve(evaluated_elements.size());
  for (const auto& element : evaluated_elements) {
    cpp_evaluated_elements.push_back(element);
  }
  
  auto results = self.Finalize(cpp_inputs, cpp_evaluated_elements);
  
  std::vector<py::bytes> py_results;
  py_results.reserve(results.size());
  for (const auto& result : results) {
    py_results.push_back(py::bytes(result));
  }
  
  return py_results;
}

PYBIND11_MODULE(libecdh_oprf, m) {
  // 设置模块属性，确保ABI兼容性
  #ifdef PYTHON_VERSION
    // 如果在编译时定义了PYTHON_VERSION，使用它
    m.attr("__compiled_for__") = PYTHON_VERSION;
  #else
    // 默认为3.11（与原始编译版本一致）
    m.attr("__compiled_for__") = "3.11";
  #endif

  // 定义枚举类型CurveType
  py::enum_<psi::CurveType>(m, "CurveType")
      .value("CURVE_SECP256K1", psi::CurveType::CURVE_SECP256K1)
      .value("CURVE_SM2", psi::CurveType::CURVE_SM2)
      .value("CURVE_FOURQ", psi::CurveType::CURVE_FOURQ)
      .export_values();

  // 绑定EcdhOPRF类
  py::class_<psi::ecdh::EcdhOPRF>(m, "EcdhOPRF")
      .def(py::init<const std::string&, psi::CurveType>(),
           py::arg("sk") = "",
           py::arg("curve_type") = psi::CurveType::CURVE_SECP256K1)
      .def("GetKey", &psi::ecdh::EcdhOPRF::GetKey)
      .def("FullEvaluate", &PyFullEvaluate)
      .def("Blind", &PyBlind)
      .def("Evaluate", &PyEvaluate)
      .def("Finalize", &PyFinalize);
}