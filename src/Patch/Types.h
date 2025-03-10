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
#ifndef TYPES_H
#define TYPES_H

#include <vector>

#include "ExecBlock/Context.h"
#include "Patch/Register.h"

#include "QBDI/State.h"

namespace QBDI {

/*! Structure representing a register in LLVM.
 *
 * The goals of this structure is to force the manipulation of the
 * integer llvm id by the method getValue().
 *
 * With this method, the compiler cannot not longuer perform the followed
 * implicit conversion:
 *  - Reg((unsigned) Reg(x) )
 */
struct RegLLVM {
  unsigned int id;

public:
  /*! Create a new register variable.
   *
   *  @param[in] id The llvm id of the register to represent.
   */
  inline RegLLVM(unsigned int id = 0) : id(id){};

  /*! Get the llvm value of the register
   *
   * @return llvm register id.
   */
  inline unsigned int getValue() const { return id; }

  inline bool operator==(const RegLLVM &o) const { return id == o.id; }

  inline bool operator!=(const RegLLVM &o) const { return id != o.id; }

  /*! Needed to create a std::map
   */
  inline bool operator<(const RegLLVM &o) const { return id < o.id; }
};

inline bool operator==(unsigned int id, const RegLLVM &reg) {
  return reg == id;
}

inline bool operator!=(unsigned int id, const RegLLVM &reg) {
  return reg != id;
}

/*! Structure representing a register variable in PatchDSL.
 */
struct Reg {
  unsigned int id;

public:
  using Vec = std::vector<Reg>;

  /*! Create a new register variable.
   *
   *  @param[in] id The id of the register to represent.
   */
  inline Reg(unsigned int id) : id(id){};

  /*! Get back the id of the register in GPRState
   *
   * @return GPRState register id.
   */
  inline unsigned int getID() const { return id; }

  /*! Convert this structure to an LLVM register id.
   *
   * @return LLVM register id.
   */
  inline operator RegLLVM() const { return GPR_ID[id]; }

  /*! Get the llvm value of the register
   *
   * @return llvm register id.
   */
  inline unsigned int getValue() const { return GPR_ID[id].getValue(); }

  /*! Return the offset of this register storage in the context part of the data
   * block.
   *
   * @return The offset.
   */
  inline rword offset() const {
    return offsetof(Context, gprState) + sizeof(rword) * id;
  }

  inline bool operator==(const RegLLVM &o) const { return o == *this; }

  inline bool operator!=(const RegLLVM &o) const { return o != *this; }

  /* Anbigious method: do we compare the index or the llvm integer ?
   */
  inline bool operator==(unsigned int id) const = delete;
  inline bool operator!=(unsigned int id) const = delete;

  /*! Needed to create a std::set
   */
  inline bool operator<(const Reg &o) const { return id < o.id; }
};

/*! Structure representing a shadow variable in PatchDSL.
 */
struct Shadow {

  uint16_t tag;

public:
  /*! Allocate a new shadow variable in the data block with the corresponding
   * tag.
   *
   *  @param[in] tag The tag of the new shadow variable.
   */
  inline Shadow(uint16_t tag) : tag(tag) {}

  /*! Return the tag associated with this shadow variable.
   *
   *  @return The tag of the shadow variable.
   */
  inline rword getTag() const { return tag; }
};

enum ShadowReservedTag : uint16_t {

  // MemoryAccess Tag
  MEMORY_TAG_BEGIN = 0xffe0,
  MEMORY_TAG_END = 0xfff0,

  // also defined in Callback.h
  Untagged = 0xffff,
};

/*! Structure representing a constant value in PatchDSL.
 */
struct Constant {

  rword v;

  /*! Represent a constant value.
   *
   *  @param[in] v The represented value.
   */
  inline Constant(rword v) : v(v) {}

  /*! Convert this structure to its value.
   *
   * @return This constant value.
   */
  inline operator rword() const { return v; }
};

/*! Structure representing a memory offset variable in PatchDSL.
 */
struct Offset {

  int64_t offset;

public:
  /*! Allocate a new offset variable with its offset value.
   *
   *  @param[in] offset The offset value
   */
  inline Offset(int64_t offset) : offset(offset) {}

  /*! Allocate a new offset variable with the offset in the context of a
   * specific register.
   *
   *  @param[in] reg The register whose offset to represent.
   */
  inline Offset(Reg reg) : offset(reg.offset()) {}

  /*! Convert this structure to its value.
   *
   * @return This offset value.
   */
  inline operator int64_t() const { return offset; }
};

/*! Structure representing a temporary register variable in PatchDSL.
 */
struct Temp {

  unsigned int id;

public:
  /*! Represent a temporary register variable idenified by a unique ID. Inside a
   * patch rules or a instrumentation rules, Temp with identical ids point to
   * the same physical register. The id 0xFFFFFFFF is reserved for internal
   * uses. The mapping from id to physical register is determined at generation
   * time and the allocation and deallocation instructions are automatically
   * added to the patch.
   *
   *  @param[in] id The id of the temp to represent.
   */
  inline Temp(unsigned int id) : id(id) {}

  /*! Convert this Temp to its id.
   *
   * @return This Temp id.
   */
  inline operator unsigned int() const { return id; }
};

/*! Structure representing an operand instruction variable in PatchDSL.
 */
struct Operand {

  unsigned int idx;

public:
  /*! Represent an operand instruction identified by its index in the LLVM
   * MCInst representation of the instruction.
   *
   *  @param[in] idx The operand index.
   */
  inline Operand(unsigned int idx) : idx(idx) {}

  /*! Convert this Operand to its idx.
   *
   * @return This Operand idx.
   */
  inline operator unsigned int() const { return idx; }
};

/* Tag value for RelocatableInst
 */
enum RelocatableInstTag {
  RelocInst = 0,
  RelocTagChangeScratchRegister = 0x1,
  RelocTagPatchBegin = 0x10,
  RelocTagPreInstMemAccess = 0x20,
  RelocTagPreInstStdCBK = 0x21,
  RelocTagPatchInstBegin = 0x30,
  RelocTagPatchInstEnd = 0x31,
  RelocTagPostInstMemAccess = 0x40,
  RelocTagPostInstStdCBK = 0x41,
  RelocTagInvalid = 0xff,
};

} // namespace QBDI

#endif // TYPES_H
