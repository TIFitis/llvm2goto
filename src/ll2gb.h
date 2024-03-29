/*
 * ll2gb.h
 *
 *  Created on: 21-Feb-2019
 *      Author: Akash Banerjee
 */

#ifndef LL2GB_H
#define LL2GB_H

#include <stdarg.h>

#include <llvm-c/Core.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/IR/Intrinsics.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/IPO/AlwaysInliner.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/IndVarSimplify.h>
#include <llvm/Transforms/Scalar/LoopStrengthReduce.h>
#include <llvm/Transforms/Utils.h>

#include <ansi-c/ansi_c_entry_point.h>
#include <ansi-c/ansi_c_language.h>
#include <ansi-c/cprover_library.h>
#include <cbmc/cbmc_parse_options.h>
#include <goto-instrument/function.h>
#include <goto-programs/goto_model.h>
#include <goto-programs/goto_program.h>
#include <goto-programs/remove_skip.h>
#include <goto-programs/write_goto_binary.h>
#include <langapi/language_util.h>
#include <langapi/mode.h>
#include <linking/static_lifetime_init.h>
#include <solvers/floatbv/float_bv.h>
#include <util/arith_tools.h>
#include <util/bitvector_expr.h>
#include <util/c_types.h>
#include <util/config.h>
#include <util/expr.h>
#include <util/floatbv_expr.h>
#include <util/message.h>
#include <util/namespace.h>
#include <util/std_code.h>
#include <util/string_constant.h>

namespace ll2gb {

extern llvm::cl::opt<bool> verbose;
extern llvm::cl::opt<bool> verbose_very;
extern llvm::cl::opt<bool> optEnabled;
extern llvm::cl::opt<bool> optimizeForced;
extern llvm::cl::opt<std::string> outputFilename;
extern llvm::cl::opt<std::string> InputFilename;

class translator;

std::unique_ptr<llvm::Module> get_llvm_ir();
bool run_llvm_passes(llvm::Module &);

bool is_assume_function(const std::string &);
bool is_assert_function(const std::string &);
bool is_assert_fail_function(const std::string &);

void print_version(llvm::raw_ostream &);
void parse_input(int argc, char **argv);
void print_error();
void panic(int);
void secret();
} // namespace ll2gb

#endif /* LL2GB_H */
