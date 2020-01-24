/*
 * ll2gb.h
 *
 *  Created on: 21-Feb-2019
 *      Author: Akash Banerjee
 */

#ifndef LL2GB_H
#define LL2GB_H

#include<stdarg.h>

#include <llvm-c/Core.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Intrinsics.h>
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/IPO.h"

#include <goto-programs/goto_model.h>
#include <goto-programs/write_goto_binary.h>
#include <goto-programs/remove_skip.h>
#include <util/config.h>
#include <util/c_types.h>
#include <util/arith_tools.h>
#include <util/namespace.h>
#include <util/string_constant.h>
#include <langapi/mode.h>
#include <langapi/language_util.h>
#include <ansi-c/ansi_c_language.h>
#include <ansi-c/ansi_c_entry_point.h>
#include <linking/static_lifetime_init.h>
#include <cbmc/cbmc_parse_options.h>
#include <solvers/floatbv/float_bv.h>

namespace ll2gb {

extern unsigned verbose;
class translator;

std::unique_ptr<llvm::Module> get_llvm_ir(std::string in_irfile,
		llvm::LLVMContext &context);

void set_function_symbol_value(goto_functionst::function_mapt&, symbol_tablet&);
void add_function_definitions(std::string, goto_functionst&, symbol_tablet&);
void set_entry_point(goto_functionst&, symbol_tablet&);
bool is_assume_function(const std::string&);
bool is_assert_function(const std::string&);
bool is_assert_fail_function(const std::string&);
bool is_intrinsic_function(const std::string&);
exprt get_intrinsics(const std::string&,
		const std::vector<exprt>&,
		const symbol_tablet&,
		goto_programt&);

void print_help();
void parse_input(int argc,
		char **argv,
		std::vector<std::pair<std::string, std::string>>&);
void print_error();
void panic(int);
void secret();
}

#endif /* LL2GB_H */
