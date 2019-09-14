/*
 * symbol_util.h
 *
 *  Created on: 20-Aug-2019
 *      Author: Akash Banerjee
 */

#ifndef SYMBOL_UTIL_H
#define SYMBOL_UTIL_H

#include "translator.h"

class ll2gb::translator::symbol_util {
	static typet get_basic_type(const llvm::DIBasicType*);
	static typet get_tag_type(const llvm::DIDerivedType*);
	static typet get_composite_type(const llvm::DICompositeType*);
	static typet get_derived_type(const llvm::DIDerivedType*);

	static std::set<std::string> typedef_tag_set; ///<Stores typdefs whose type symbols have already been added.
public:
	static std::string lookup_namespace(std::string);
	static typet get_goto_type(const llvm::DIType*);
	static typet get_goto_type(const llvm::Type*);
	static symbolt create_symbol(const llvm::DIVariable*);
	static symbolt create_symbol(const llvm::Type*,
			llvm::DILocalScope *di_scope = nullptr);
	static symbolt create_goto_func_symbol(const llvm::Function&);
};

#endif /* SYMBOL_UTIL_H */
