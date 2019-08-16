/*
 * symbols.cpp
 *
 *  Created on: 16-Aug-2019
 *      Author: akash
 */

#include "llvm2goto.h"

using namespace ll2gb;

typet translator::get_goto_type(const llvm::DIType*) {

}

typet translator::get_goto_type(const llvm::Type*) {

}

symbolt translator::create_symbol(const llvm::DIType *di_type) {
	symbolt symbol;
	return symbol;
}

symbolt translator::create_symbol(const llvm::Type *type) {
	symbolt symbol;
	return symbol;
}

symbolt translator::create_goto_func_symbol(const llvm::FunctionType *type,
		const llvm::Function &F) {
	symbolt symbol;
	symbol.clear();
	symbol.is_thread_local = false;
	symbol.mode = ID_C;
	auto func_code_type = code_typet();
	code_typet::parameterst para;
	if (F.hasMetadata()) {
		auto *meta_data =
				llvm::dyn_cast<llvm::DISubprogram>(F.getSubprogram())->getType();
		for (auto arg_iter = F.arg_begin(), arg_end = F.arg_end();
				arg_iter != arg_end; arg_iter++) {
			auto arg_symbol = symbol_table.lookup(F.getName().str() + "::"
					+ arg_iter->getName().str());
			code_typet::parametert p(arg_symbol->type);
			para.push_back(p);
		}
		func_code_type.parameters() = para;
		if (&*meta_data->getTypeArray()[0] != NULL) {
			auto *mdn =
					llvm::dyn_cast<llvm::DIType>(&*meta_data->getTypeArray()[0]);
			type->getReturnType();
			while (mdn->getTag() == llvm::dwarf::DW_TAG_typedef)
				mdn = llvm::dyn_cast<llvm::DIType>(llvm::dyn_cast<
						llvm::DIDerivedType>(mdn)->getBaseType());
			func_code_type.return_type() = get_goto_type(mdn);
		}
		else {
			func_code_type.return_type() = void_typet();
		}
	}
	symbol.type = func_code_type;
	return symbol;
}
