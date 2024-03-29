/*
 * translator.h
 *
 *  Created on: 20-Aug-2019
 *      Author: Akash Banerjee
 */

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "ll2gb.h"

class ll2gb::translator {
private:
  std::unique_ptr<llvm::Module> &llvm_module;
  goto_programt goto_program;
  goto_functionst goto_functions;
  static symbol_tablet symbol_table;
  null_message_handlert msg_handler;
  c_object_factory_parameterst object_factory_params;

  std::map<const llvm::Instruction *, std::string>
      var_name_map; ///< map from instructions to their symbol names
  std::map<std::string, std::string>
      aux_name_map; /// Map to go back from auxiliary names to original var
                    /// names
  std::map<const llvm::InsertValueInst *, exprt>
      ins_value_name_map; ///< map from instructions to their symbol names
  static std::map<const llvm::Argument *, std::string>
      func_arg_name_map; ///< map from func args to their symbol names
  std::map<const llvm::AllocaInst *, llvm::DbgDeclareInst *>
      alloca_dbg_map; ///< map from Allocas to their DbgDeclare if exists
  std::map<const llvm::CallInst *, std::string>
      call_ret_sym_map; ///< map from Call Instructions to their return symbol
                        ///< names
  std::map<const llvm::BasicBlock *, goto_programt::targett>
      block_target_map; ///< map from BB to first goto target for that block
  std::map<const llvm::BranchInst *,
           std::pair<goto_programt::targett, goto_programt::targett>>
      br_instr_target_map; ///< map from BranchInst to their goto targets
  std::map<const llvm::SwitchInst *, std::vector<goto_programt::targett>>
      switch_instr_target_map; ///< map from SwitchInst to their goto targets
  static std::map<llvm::DIScope *, std::string> scope_name_map;
  std::map<const llvm::Value *, exprt> state_map;
  std::set<const llvm::Value *> save_state_values;

  bool trans_instruction(const llvm::Instruction &);
  bool trans_block(const llvm::BasicBlock &);
  void set_branches();
  void set_switches();
  void set_returns(goto_programt::targett &);
  bool trans_function(llvm::Function &);
  bool trans_module();
  void analyse_ir();
  void collect_operands(const llvm::Instruction &,
                        std::set<const llvm::Value *> &);
  void add_function_symbols();
  void set_function_symbol_value(goto_functionst::function_mapt &,
                                 symbol_tablet &);
  void set_entry_point(goto_functionst &, symbol_tablet &);
  void add_global_symbols();
  void add_initial_symbols();
  void set_config();
  void initialize_goto() {
    register_language(new_ansi_c_language);
    set_config();
    add_initial_symbols();
  }

  void trans_alloca(const llvm::AllocaInst &);
  void trans_br(const llvm::BranchInst &);
  void trans_call(const llvm::CallInst &);
  void trans_call_llvm_intrinsic(const llvm::IntrinsicInst &);
  void trans_insertvalue(const llvm::InsertValueInst &);
  void trans_ret(const llvm::ReturnInst &);
  void trans_store(const llvm::StoreInst &);
  void trans_switch(const llvm::SwitchInst &);

  exprt get_expr(const llvm::Value &, bool new_state_required = false);

  exprt get_expr_phi(const llvm::PHINode &);
  exprt get_expr_extractvalue(const llvm::ExtractValueInst &);
  exprt get_expr_gep(const llvm::GetElementPtrInst &);
  exprt get_expr_bitcast(const llvm::BitCastInst &);
  exprt get_expr_fcmp(const llvm::FCmpInst &);
  exprt get_expr_icmp(const llvm::ICmpInst &);
  exprt get_expr_trunc(const llvm::TruncInst &);
  exprt get_expr_load(const llvm::LoadInst &);
  exprt get_expr_add(const llvm::Instruction &);
  exprt get_expr_fadd(const llvm::Instruction &);
  exprt get_expr_sub(const llvm::Instruction &);
  exprt get_expr_fsub(const llvm::Instruction &);
  exprt get_expr_mul(const llvm::Instruction &);
  exprt get_expr_fmul(const llvm::Instruction &);
  exprt get_expr_sdiv(const llvm::Instruction &);
  exprt get_expr_srem(const llvm::Instruction &);
  exprt get_expr_fdiv(const llvm::Instruction &);
  exprt get_expr_udiv(const llvm::Instruction &);
  exprt get_expr_urem(const llvm::Instruction &);
  exprt get_expr_and(const llvm::Instruction &);
  exprt get_expr_or(const llvm::Instruction &);
  exprt get_expr_xor(const llvm::Instruction &);
  exprt get_expr_shl(const llvm::Instruction &);
  exprt get_expr_lshr(const llvm::Instruction &);
  exprt get_expr_ashr(const llvm::Instruction &);
  exprt get_expr_zext(const llvm::ZExtInst &);
  exprt get_expr_sext(const llvm::SExtInst &);
  exprt get_expr_fpext(const llvm::FPExtInst &);
  exprt get_expr_fptosi(const llvm::FPToSIInst &);
  exprt get_expr_sitofp(const llvm::SIToFPInst &);
  exprt get_expr_fptoui(const llvm::FPToUIInst &);
  exprt get_expr_uitofp(const llvm::UIToFPInst &);
  exprt get_expr_fptrunc(const llvm::FPTruncInst &);
  exprt get_expr_fneg(const llvm::Instruction &);
  exprt get_expr_ptrtoint(const llvm::PtrToIntInst &);
  exprt get_expr_inttoptr(const llvm::IntToPtrInst &);
  exprt get_expr_const(const llvm::Constant &);
  exprt get_expr_select(const llvm::SelectInst &);

  enum class intrinsics {
    malloc,
    calloc,
    free,
    __fpclassify,
    __fpclassifyf,
    __fpclassifyl,
    fesetround,
    fegetround,
    fdim,
    fmod,
    fmodf,
    remainder,
    lround,
    sin,
    cos,
    modff,
    lrint,
    nan,
    __isinf,
    __isinff,
    __isinfl,
    __isnan,
    __isnanf,
    __isnanl,
    __signbit,
    __signbitf,
    abort,
    cprover_round_to_integralf,
    cprover_round_to_integral,
    cprover_remainder,
    cprover_remainderf,
    llvm_memcpy_p0i8_p0i8_i64,
    llvm_memset_p0i8_i64,
    llvm_trunc_f64,
    llvm_fabs_f80,
    llvm_fabs_f64,
    llvm_fabs_f32,
    llvm_floor_f64,
    llvm_ceil_f64,
    llvm_rint_f64,
    llvm_nearbyint_f64,
    llvm_round_f64,
    llvm_copysign_f64,
    llvm_maxnum_f64,
    llvm_minnum_f64,
    ll2gb_default
  };
  static std::map<intrinsics, bool> intrinsic_support_added;
  bool is_intrinsic_function(const std::string &);
  typet get_intrinsic_return_type(const std::string &);
  void add_intrinsic_support(const llvm::Function &);
  void add_llvm_intrinsic_support(const llvm::Function &);
  void add_llvm_intrinsic_support(const std::string &);
  static intrinsics get_intrinsic_id(const std::string &);
  void add_llvm_rint_support();
  void add_llvm_nearbyint_support();
  void add_llvm_memcpy_support();
  void add_llvm_memset_support();
  void add_llvm_trunc_support();
  void add_llvm_fabs_support();
  void add_llvm_fabs80_support();
  void add_llvm_fabs32_support();
  void add_llvm_floor_support();
  void add_llvm_ceil_support();
  void add_llvm_round_support();
  void add_llvm_copysign_support();
  void add_llvm_maxnum_support();
  void add_llvm_minnum_support();

  void make_func_call(const llvm::CallInst &);

  source_locationt get_location(const llvm::Instruction &);

  class symbol_util;
  ///< A sub-class to group all the symbol and type related methods.
  class scope_tree;
  ///< A sub-class to implement the scoping rules.

public:
  static std::string error_state; ///< If any error is encountered, the errmsg
                                  ///< is stored in this string.
  translator(std::unique_ptr<llvm::Module> &M) : llvm_module{M} {}
  bool generate_goto();
  void write_goto(const std::string &);

  /// Returns true if there is any error, signified by non-empty error_state.
  static bool check_state() { return !error_state.empty(); }

  static void check_optimizations_safe(const llvm::Module &);
  static void identify_state_saves(const llvm::Module &);
  ~translator();
};

#endif /* TRANSLATOR_H */
