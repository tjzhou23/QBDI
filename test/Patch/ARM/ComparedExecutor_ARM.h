/*
 * This file is part of QBDI.
 *
 * Copyright 2017 - 2024 Quarkslab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef COMPAREDEXECUTOR_ARM_H
#define COMPAREDEXECUTOR_ARM_H

#include <sstream>
#include <string.h>
#include <string>

#include "TestSetup/InMemoryAssembler.h"
#include "TestSetup/ShellcodeTester.h"

#define CPU_CPU "cortex-a57"
#define CPU_MATTRS "neon"

extern const char *GPRSave_s;
extern const char *GPRShuffle_s;
extern const char *RelativeAddressing_s;
extern const char *ConditionalBranching_s;
extern const char *FibonacciRecursion_s;
extern const char *StackTricks_s;
extern const char *STLDMIA_s;
extern const char *STLDMIB_s;
extern const char *STLDMDA_s;
extern const char *STLDMDB_s;
extern const char *STMDB_LDMIA_post_s;
extern const char *STMDA_LDMIB_post_s;
extern const char *STMIB_LDMDA_post_s;
extern const char *STMIA_LDMDB_post_s;
extern const char *LDREXTest_s;

class ComparedExecutor_ARM : public ShellcodeTester {

public:
  ComparedExecutor_ARM() : ShellcodeTester(CPU_CPU, {CPU_MATTRS}) {}

  QBDI::Context jitExec(llvm::ArrayRef<uint8_t> code, QBDI::Context &inputCtx,
                        llvm::sys::MemoryBlock &stack);

  QBDI::Context realExec(llvm::ArrayRef<uint8_t> code, QBDI::Context &inputCtx,
                         llvm::sys::MemoryBlock &stack);

  InMemoryObject compileWithContextSwitch(const char *source);

  void initContext(QBDI::Context &ctx);
};

#endif
