// Copyright 2023 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/compiler/turboshaft/optimize-phase.h"

#include "src/compiler/js-heap-broker.h"
#include "src/compiler/turboshaft/late-escape-analysis-reducer.h"
#include "src/compiler/turboshaft/machine-optimization-reducer.h"
#include "src/compiler/turboshaft/memory-optimization-reducer.h"
#include "src/compiler/turboshaft/structural-optimization-reducer.h"
#include "src/compiler/turboshaft/value-numbering-reducer.h"
#include "src/compiler/turboshaft/variable-reducer.h"
#include "src/numbers/conversions-inl.h"

namespace v8::internal::compiler::turboshaft {

void OptimizePhase::Run(PipelineData* data, Zone* temp_zone) {
  UnparkedScopeIfNeeded scope(data->broker(),
                              v8_flags.turboshaft_trace_reduction);
  turboshaft::OptimizationPhase<
      turboshaft::StructuralOptimizationReducer,
      turboshaft::LateEscapeAnalysisReducer,
      turboshaft::MemoryOptimizationReducer, turboshaft::VariableReducer,
      turboshaft::MachineOptimizationReducerSignallingNanImpossible,
      turboshaft::ValueNumberingReducer>::
      Run(data->isolate(), &data->graph(), temp_zone, data->node_origins(),
          std::tuple{
              turboshaft::MemoryOptimizationReducerArgs{data->isolate()}});
}

}  // namespace v8::internal::compiler::turboshaft
