/*
 * SPDX-License-Identifier: Apache-2.0
 */

//===----------- InitOMPasses.hpp - Init onnx-mlir passes  ----------------===//
//
// Copyright 2019-2022 The IBM Research Authors.
//
// =============================================================================
//
//===----------------------------------------------------------------------===//

#pragma once

#include "mlir/Pass/Pass.h"
#include "src/Pass/Passes.hpp"

namespace onnx_mlir {

void initOMPasses(int optLevel) {
  // All passes implemented within onnx-mlir should register within this
  // function to make themselves available as a command-line option.

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createScrubDisposablePass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createONNXOpTransformPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createDecomposeONNXToONNXPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createConvOptONNXToONNXPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createONNXHybridTransformPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    if (enableONNXHybridPass) {
      // Replaces shape inference in onnx-mlir-opt which is used in all the
      // lit tests, except the parse lit tests.
      return createONNXShapeInferenceTransformPass();
    } else {
      return createShapeInferencePass();
    }
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createConstPropONNXToONNXPass();
  });

  mlir::registerPass(
      []() -> std::unique_ptr<mlir::Pass> { return createInstrumentPass(); });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createInstrumentONNXSignaturePass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createONNXPreKrnlVerifyPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return krnl::createKrnlEnableMemoryPoolPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return krnl::createKrnlBundleMemoryPoolsPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return krnl::createKrnlOptimizeMemoryPoolsPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return krnl::createConvertKrnlToAffinePass();
  });

  mlir::registerPass([optLevel]() -> std::unique_ptr<mlir::Pass> {
    return createLowerToKrnlPass(/*enableTiling*/ optLevel >= 3,
        /*enableSIMD, should consider disableSimdOption*/ optLevel >= 3,
        /*enableParallel*/ false);
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createElideConstGlobalValuePass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return krnl::createConvertSeqToMemrefPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return krnl::createLowerKrnlRegionPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return krnl::createConvertKrnlToLLVMPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createDisconnectKrnlDimFromAllocPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createLowerKrnlShapePass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createSimplifyShapeRelatedOpsPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createONNXDimAnalysisPass();
  });

  mlir::registerPass([]() -> std::unique_ptr<mlir::Pass> {
    return createConvertONNXToTOSAPass();
  });

#ifdef ONNX_MLIR_ENABLE_MHLO
  mlir::registerPass(
      []() -> std::unique_ptr<mlir::Pass> { return createLowerToMhloPass(); });
#endif
}

} // namespace onnx_mlir
