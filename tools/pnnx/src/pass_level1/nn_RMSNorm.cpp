// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2024 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "fuse_module_pass.h"

namespace pnnx {

class RMSNorm : public FuseModulePass
{
public:
    const char* match_type_str() const
    {
        return "__torch__.torch.nn.modules.normalization.RMSNorm";
    }

    const char* type_str() const
    {
        return "nn.RMSNorm";
    }

    void write(Operator* op, const TorchGraphProxy& graph, const TorchModuleProxy& mod) const
    {
        const TorchNodeProxy* rmsn = graph.find_node_by_kind("aten::rms_norm");

        op->params["normalized_shape"] = rmsn->namedInput("normalized_shape");
        op->params["eps"] = rmsn->namedInput("eps");
        op->params["elementwise_affine"] = mod.hasattr("weight");

        if (mod.hasattr("weight"))
        {
            op->attrs["weight"] = mod.attr("weight");
        }
    }
};

REGISTER_GLOBAL_PNNX_FUSE_MODULE_PASS(RMSNorm)

} // namespace pnnx
