// Copyright (c) 2021 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "lite/kernels/nnadapter/converter/converter.h"

namespace paddle {
namespace lite {
namespace kernels {
namespace nnadapter {

int ConvertReshape(Converter* converter, OpInfo* op, Scope* scope) {
  // Input operand
  auto x_name = op->Input("X").front();
  auto input_operand = converter->GetMappedOperand(x_name);

  // Shape operand
  // "ShapeTensor"(input) > "Shape"(input) > "shape"(attr)
  NNAdapterOperand* shape_operand = nullptr;
  if (HasInput(op, scope, "ShapeTensor")) {
    // TODO(zhupengyang): apply concat
    LOG(WARNING) << "Not support ShapeTensor!";
    return UNSUPPORTED_FEATURE;
  } else if (HasInput(op, scope, "Shape")) {
    auto shape_name = op->Input("Shape").front();
    shape_operand = converter->GetMappedOperand(shape_name);
  } else {
    std::vector<int> shape = op->GetAttr<std::vector<int>>("shape");
    shape_operand = converter->AddConstantOperand(shape);
  }

  // Output operand
  auto out_name = op->Output("Out").front();
  // Copy scales from input in PrepareReshape
  auto output_operand = converter->AddOutputOperand(out_name);

  // Reshape operation
  converter->AddOperation(
      NNADAPTER_RESHAPE, {input_operand, shape_operand}, {output_operand});
  return NO_ERROR;
}

}  // namespace nnadapter
}  // namespace kernels
}  // namespace lite
}  // namespace paddle
