/*
 * util.cpp
 *
 *  Created on: 16-Aug-2019
 *      Author: Akash Banerjee
 */

#include "llvm2goto.h"

using namespace std;
using namespace llvm;
using namespace ll2gb;

void ll2gb::print_help() {
	outs() << "Version: 2.0\n"
			<< "Usage: llvm2goto <ir_file> -o <op_file_name>\n"
			<< "       llvm2goto <ir_file_1> <ir_file_2> ...\n";
	exit(1);
}

void ll2gb::parse_input(int argc,
		char **argv,
		vector<pair<string, string>> &file_names) {

	if (argc < 2) {
		print_help();
	}
	if (!string(argv[1]).compare("-h") || !string(argv[1]).compare("--help"))
		print_help();

	if (argc == 4) {
		if (!string(argv[1]).compare("-o")) {
			file_names.push_back(make_pair(argv[3], argv[2]));
		}
		else if (!string(argv[2]).compare("-o")) {
			file_names.push_back(make_pair(argv[1], argv[3]));
		}
		else
			print_help();
	}
	else {
		for (auto i = 1; i < argc; i++) {
			string temp(argv[i]);
			if (!temp.compare("-o")) {
				print_help();
			}
			auto index = temp.find(".ll");
			if (index != temp.npos)
				file_names.push_back(make_pair(argv[i],
						string(argv[i]).substr(0, index) + ".gb"));
			else
				file_names.push_back(make_pair(argv[i], temp + ".gb"));
		}
	}
}

void ll2gb::set_function_symbol_value(goto_functionst::function_mapt &function_map,
		symbol_tablet &symbol_table) {
	for (typename goto_functionst::function_mapt::iterator it =
			function_map.begin(); it != function_map.end(); it++) {
		goto_programt::instructionst instructions = it->second.body.instructions;
		code_blockt cb;
		for (goto_programt::targett ins = instructions.begin();
				ins != instructions.end(); ins++) {
			cb.add(ins->code);
		}
		symbolt *symbol = const_cast<symbolt*>(symbol_table.lookup(it->first));
		symbol->value.swap(cb);
	}
}

void ll2gb::add_function_definitions(std::string name,
		goto_functionst &goto_functions,
		symbol_tablet &symbol_table) {
	goto_programt gp;
	code_blockt cb = to_code_block(to_code(symbol_table.lookup(name)->value));
	for (unsigned int b = 0; b < cb.operands().size(); b++) {
		goto_programt::targett ins = gp.add_instruction();
		codet c = to_code(cb.operands()[b]);
		if (ID_assign == c.get_statement()) {
			ins->make_assignment();
			ins->code = code_assignt(c.operands()[0], c.operands()[1]);
		}
		else if (ID_output == c.get_statement()) {
			c.operands().resize(2);

			const symbolt &return_symbol = *symbol_table.lookup("return'");

			c.op0() =
					address_of_exprt(index_exprt(string_constantt(return_symbol.base_name),
							from_integer(0, index_type())));

			c.op1() = return_symbol.symbol_expr();
			ins->make_other(c);
		}
		else if (ID_label == c.get_statement()) {
			ins->make_skip();
		}
		else if (ID_function_call == c.get_statement()) {
			ins->make_function_call(c);
		}
		else if (ID_input == c.get_statement()) {
			c.operands().resize(2);
			c.op0() = address_of_exprt(index_exprt(string_constantt("argc"),
					from_integer(0, index_type())));
			c.op1() = symbol_table.lookup("argc'")->symbol_expr();
			ins->make_other(c);
		}
		else if (ID_assume == c.get_statement()) {
			ins->make_assumption(c.op0());
		}
		else {
			ins->code = c;
		}
	}
	gp.add_instruction(END_FUNCTION);
	gp.update();
	(*goto_functions.function_map.find(name)).second.body.swap(gp);
}

void ll2gb::set_entry_point(goto_functionst &goto_functions,
		symbol_tablet &symbol_table) {
	set_function_symbol_value(goto_functions.function_map, symbol_table);
	int argc = 0;
	const char **argv = nullptr;
	cbmc_parse_optionst parse_options(argc, argv);
	c_object_factory_parameterst object_factory_params;
	optionst options;
	parse_options.set_default_options(options);
	object_factory_params.set(options);
	parse_options.get_message_handler();
	ansi_c_entry_point(symbol_table,
			parse_options.get_message_handler(),
			object_factory_params);
	goto_functions.function_map.insert(std::pair<const dstringt,
			goto_functionst::goto_functiont>("__CPROVER__start",
			goto_functionst::goto_functiont()));
	add_function_definitions("__CPROVER__start", goto_functions, symbol_table);
	goto_functions.function_map.insert(std::pair<const dstringt,
			goto_functionst::goto_functiont>(
	INITIALIZE_FUNCTION, goto_functionst::goto_functiont()));
	add_function_definitions(INITIALIZE_FUNCTION, goto_functions, symbol_table);
}

