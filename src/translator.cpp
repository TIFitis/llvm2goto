/*
 * llvm2goto_translator.cpp
 *
 *  Created on: 21-Feb-2019
 *      Author: Akash Banerjee
 */

#include "llvm2goto.h"

using namespace ll2gb;

void translator::add_global_symbols() {

}

void translator::add_function_symbols() {
	for (llvm::Function &F : *llvm_module) {
		if (F.isDeclaration()) {
			continue;
		}
		if (F.getSubprogram() != NULL) {
			auto *md =
					llvm::dyn_cast<llvm::DISubprogram>(F.getSubprogram())->getType();
			unsigned int i = 1;
			for (auto arg_iter = F.arg_begin(), arg_end = F.arg_end();
					arg_iter != arg_end; arg_iter++, i++) {
				symbolt arg_symbol =
						create_symbol(llvm::dyn_cast<llvm::DIType>(&*md->getTypeArray()[i]));

				if (!arg_iter->getName().str().compare("argc"))
					arg_symbol.name = "argc'";
				else
					arg_symbol.name = F.getName().str() + "::"
							+ arg_iter->getName().str();
				if (!arg_iter->getName().str().compare("argv"))
					arg_symbol.name = "argv'";
				else
					arg_symbol.name = F.getName().str() + "::"
							+ arg_iter->getName().str();
				arg_symbol.base_name = arg_iter->getName().str();
				arg_symbol.is_lvalue = true;
				arg_symbol.is_parameter = true;
				arg_symbol.is_state_var = true;
				arg_symbol.is_thread_local = true;
				arg_symbol.is_file_local = true;
				symbol_table.add(arg_symbol);
				var_name_map[arg_symbol.name.c_str()] =
						arg_symbol.base_name.c_str();
			}
		}
		symbolt func_symbol = create_goto_func_symbol(F.getFunctionType(), F);
		func_symbol.name = dstringt(F.getName().str());
		func_symbol.base_name = func_symbol.name;
		func_symbol.is_lvalue = true;
		symbol_table.add(func_symbol);
		goto_functions.function_map[dstringt(F.getName())] =
				goto_functionst::goto_functiont();
	}
}

bool translator::generate_goto() {
	llvm::dbgs() << "Generating GOTO Binary\n";

	initialize_goto();
	add_global_symbols();
	add_function_symbols();

	for (auto F = llvm_module->getFunctionList().begin();
			F != llvm_module->getFunctionList().end(); F++) {
		F->dump();
	}
	llvm::dbgs() << "GOTO Binary generated successfully\n";
	return true;
}