#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <variant>
#include <unordered_map>
#include <memory>
#include <regex>

#include <gsl/gsl>

namespace assembler {

	struct instruction;

	using reg           = char;
	using operand       = std::variant<reg, std::ptrdiff_t, instruction*>;
	using register_file = std::unordered_map<char, std::ptrdiff_t>;

	inline std::ptrdiff_t resolve_operand(operand op, register_file& registers) {
		if(std::holds_alternative<reg>(op)) {
			return registers[std::get<reg>(op)];
		} else if(std::holds_alternative<std::ptrdiff_t>(op)) {
			return std::get<std::ptrdiff_t>(op);
		}
		__assume(0);
	}

	inline std::string print_register(reg r) {
		return std::string(1, r);
	}

	struct processor;

	struct instruction
	{
		virtual std::ptrdiff_t execute(processor& cpu) = 0;
		virtual std::string emit_asm() = 0;

		std::ptrdiff_t abs_addr = 0;
		std::string opcode;

		virtual ~instruction() = default;
		instruction(const std::string& opcode_) : opcode(opcode_) {
		}

		instruction(const instruction&) = default;
		instruction(instruction&&) = default;
		instruction& operator=(const instruction&) = default;
		instruction& operator=(instruction&&) = default;
	};

	using instruction_ptr = std::unique_ptr<instruction>;
	using program         = std::vector<instruction_ptr>;
	using port            = std::vector<std::ptrdiff_t>;

	struct processor
	{
		program* instructions;
		std::size_t instruction_pointer = 0;
		register_file registers;
		port* output;

		processor(program* instructions_) : instructions(instructions_) {
		}

		bool single_step() {
			const instruction_ptr& ins = (*instructions)[instruction_pointer];
			const std::ptrdiff_t delta = ins->execute(*this);
			if(delta == 0 || instruction_pointer + delta >= instructions->size()) {
				return false;
			}
			instruction_pointer += delta;
			return true;
		}

		void run() {
			while(single_step()) {
				;
			}
		}
	};

	inline std::string print_operand(operand op) {
		if(std::holds_alternative<reg>(op)) {
			return std::string(1, std::get<reg>(op));
		} else if(std::holds_alternative<std::ptrdiff_t>(op)) {
			return std::to_string(std::get<std::ptrdiff_t>(op));
		} else if(std::holds_alternative<instruction*>(op)) {
			instruction* ins = std::get<instruction*>(op);
			if(ins == nullptr) {
				return "END-OF-PROGRAM";
			} else {
				return "&(" + ins->emit_asm() + ")";
			}
		}
		__assume(0);
	}

	struct nilary_instruction : instruction
	{
		std::string emit_asm() override {
			return opcode;
		}

		nilary_instruction(const std::string& opcode_) noexcept : instruction(opcode_) {
		}
	};

	struct nop0 : nilary_instruction
	{
		nop0() noexcept : nilary_instruction("nop0") {
		}

		std::ptrdiff_t execute(processor&) override {
			return 1;
		}
	};

	struct unary_instruction : instruction
	{
		operand op1;

		std::string emit_asm() override {
			return opcode + " " + print_operand(op1);
		}

		unary_instruction(const std::string& opcode_, operand op1_) noexcept : instruction(opcode_), op1(op1_) {
		}
	};

	struct binary_instruction : instruction
	{
		operand op1;
		operand op2;

		std::string emit_asm() override {
			return opcode + " " + print_operand(op1) + " " + print_operand(op2);
		}

		binary_instruction(const std::string& opcode_, operand op1_, operand op2_) noexcept : instruction(opcode_), op1(op1_), op2(op2_) {
		}
	};

	struct ternary_instruction : instruction
	{
		operand op1;
		operand op2;
		operand op3;

		std::string emit_asm() override {
			return opcode + " " + print_operand(op1) + " " + print_operand(op2) + " " + print_operand(op3);
		}

		ternary_instruction(const std::string& opcode_, operand op1_, operand op2_, operand op3_) noexcept : instruction(opcode_), op1(op1_), op2(op2_), op3(op3_) {
		}
	};

	struct inc : unary_instruction
	{
		inc(operand op1_) noexcept : unary_instruction("inc", op1_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			if(!std::holds_alternative<reg>(op1)) {
				return 1;
			}
			++cpu.registers[std::get<reg>(op1)];
			return 1;
		}
	};

	struct out : unary_instruction
	{
		out(operand op1_) noexcept : unary_instruction("out", op1_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.output->push_back(resolve_operand(op1, cpu.registers));
			return 1;
		}
	};

	struct dec : unary_instruction
	{
		dec(operand op1_) noexcept : unary_instruction("dec", op1_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			if(!std::holds_alternative<reg>(op1)) {
				return 1;
			}
			--cpu.registers[std::get<reg>(op1)];
			return 1;
		}
	};

	struct nop1 : unary_instruction
	{
		nop1(operand op1_) noexcept : unary_instruction("nop1", op1_) {
		}

		std::ptrdiff_t execute(processor&) override {
			return 1;
		}
	};

	struct set : binary_instruction
	{
		set(operand op1_, operand op2_) noexcept : binary_instruction("set", op1_, op2_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.registers[std::get<reg>(op1)] = resolve_operand(op2, cpu.registers);
			return 1;
		}
	};

	struct cpy : binary_instruction
	{
		cpy(operand op1_, operand op2_) noexcept : binary_instruction("cpy", op1_, op2_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			if(!std::holds_alternative<reg>(op2)) {
				return 1;
			}
			cpu.registers[std::get<reg>(op2)] = resolve_operand(op1, cpu.registers);
			return 1;
		}
	};

	struct sub : binary_instruction
	{
		sub(operand op1_, operand op2_) noexcept : binary_instruction("sub", op1_, op2_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.registers[std::get<reg>(op1)] -= resolve_operand(op2, cpu.registers);
			return 1;
		}
	};

	struct add : binary_instruction
	{
		add(operand op1_, operand op2_) noexcept : binary_instruction("add", op1_, op2_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.registers[std::get<reg>(op1)] += resolve_operand(op2, cpu.registers);
			return 1;
		}
	};

	struct mul : binary_instruction
	{
		mul(operand op1_, operand op2_) noexcept : binary_instruction("mul", op1_, op2_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.registers[std::get<reg>(op1)] *= resolve_operand(op2, cpu.registers);
			return 1;
		}
	};

	struct mod : binary_instruction
	{
		mod(operand op1_, operand op2_) noexcept : binary_instruction("mod", op1_, op2_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.registers[std::get<reg>(op1)] %= resolve_operand(op2, cpu.registers);
			return 1;
		}
	};

	struct div : binary_instruction
	{
		div(operand op1_, operand op2_) noexcept : binary_instruction("div", op1_, op2_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.registers[std::get<reg>(op1)] /= resolve_operand(op2, cpu.registers);
			return 1;
		}
	};

	struct sqrt : binary_instruction
	{
		sqrt(operand op1_, operand op2_) noexcept : binary_instruction("sqrt", op1_, op2_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.registers[std::get<reg>(op1)] = gsl::narrow_cast<std::ptrdiff_t>(std::ceil(std::sqrt(resolve_operand(op2, cpu.registers))));
			return 1;
		}
	};

	struct nop2 : binary_instruction
	{
		nop2(operand op1_, operand op2_) noexcept : binary_instruction("nop2", op1_, op2_) {
		}

		std::ptrdiff_t execute(processor&) override {
			return 1;
		}
	};

	struct jmp : instruction
	{
		operand destination;

		jmp(operand destination_) noexcept : jmp("jmp", destination_) {
		}

		jmp(const std::string& opcode_, operand destination_) noexcept : instruction(opcode_), destination(destination_) {
		}

		std::string emit_asm() override {
			return opcode + " " + print_operand(destination);
		}

		std::ptrdiff_t execute(processor& cpu) override {
			return resolve_operand(destination, cpu.registers);
		}
	};

	struct conditional_jump : jmp
	{
		operand control;

		conditional_jump(const std::string& opcode_, operand control_, operand destination_) noexcept : jmp(opcode_, destination_), control(control_) {
		}

		std::string emit_asm() override {
			return opcode + " " + print_operand(control) + " " + print_operand(destination);
		}

		std::ptrdiff_t execute(processor& cpu) override {
			const std::ptrdiff_t control_value = std::holds_alternative<reg>(control) ? cpu.registers[std::get<reg>(control)]
				: std::get<std::ptrdiff_t>(control);
			if(execute_condition(control_value)) {
				return resolve_operand(destination, cpu.registers);
			} else {
				return 1;
			}
		}

		virtual bool execute_condition(std::ptrdiff_t control_value) = 0;
	};

	struct jnz : conditional_jump
	{
		jnz(operand control_, operand destination_) noexcept : conditional_jump("jnz", control_, destination_) {
		}

		bool execute_condition(std::ptrdiff_t control_value) noexcept override {
			return control_value != 0;
		}
	};

	struct jgz : conditional_jump
	{
		jgz(operand control_, operand destination_) noexcept : conditional_jump("jgz", control_, destination_) {
		}

		bool execute_condition(std::ptrdiff_t control_value) noexcept override {
			return control_value > 0;
		}
	};

	struct tadd : ternary_instruction
	{
		tadd(operand op1_, operand op2_, operand op3_) noexcept : ternary_instruction("tadd", op1_, op2_, op3_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.registers[std::get<reg>(op1)] = resolve_operand(op2, cpu.registers) + resolve_operand(op3, cpu.registers);
			return 1;
		}
	};

	struct tsub : ternary_instruction
	{
		tsub(operand op1_, operand op2_, operand op3_) noexcept : ternary_instruction("tsub", op1_, op2_, op3_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.registers[std::get<reg>(op1)] = resolve_operand(op2, cpu.registers) - resolve_operand(op3, cpu.registers);
			return 1;
		}
	};

	struct tmul : ternary_instruction
	{
		tmul(operand op1_, operand op2_, operand op3_) noexcept : ternary_instruction("tmul", op1_, op2_, op3_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.registers[std::get<reg>(op1)] = resolve_operand(op2, cpu.registers) * resolve_operand(op3, cpu.registers);
			return 1;
		}
	};

	struct tdiv : ternary_instruction
	{
		tdiv(operand op1_, operand op2_, operand op3_) noexcept : ternary_instruction("tdiv", op1_, op2_, op3_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.registers[std::get<reg>(op1)] = resolve_operand(op2, cpu.registers) / resolve_operand(op3, cpu.registers);
			return 1;
		}
	};

	struct tmod : ternary_instruction
	{
		tmod(operand op1_, operand op2_, operand op3_) noexcept : ternary_instruction("tmod", op1_, op2_, op3_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			cpu.registers[std::get<reg>(op1)] = resolve_operand(op2, cpu.registers) % resolve_operand(op3, cpu.registers);
			return 1;
		}
	};

	struct nop3 : ternary_instruction
	{
		nop3(operand op1_, operand op2_, operand op3_) noexcept : ternary_instruction("nop3", op1_, op2_, op3_) {
		}

		std::ptrdiff_t execute(processor&) override {
			return 1;
		}
	};

	struct tgl : unary_instruction
	{
		tgl(operand op1_) noexcept : unary_instruction("tgl", op1_) {
		}

		std::ptrdiff_t execute(processor& cpu) override {
			const std::ptrdiff_t delta = cpu.registers[std::get<reg>(op1)];
			const std::size_t offset   = cpu.instruction_pointer + delta;
			if(offset >= cpu.instructions->size()) {
				return 1;
			}
			instruction_ptr preserve = std::move((*cpu.instructions)[offset]);
			instruction* raw = preserve.get();
			instruction_ptr replacement;
			if(inc* i = dynamic_cast<inc*>(raw); i != nullptr) {
				replacement = std::make_unique<dec>(i->op1);
			} else if(unary_instruction* ui = dynamic_cast<unary_instruction*>(raw); ui != nullptr) {
				replacement = std::make_unique<inc>(ui->op1);
			} else if(jnz* j = dynamic_cast<jnz*>(raw); j != nullptr) {
				replacement = std::make_unique<cpy>(j->control, j->destination);
			} else if(binary_instruction* bi = dynamic_cast<binary_instruction*>(raw); bi != nullptr) {
				replacement = std::make_unique<jnz>(bi->op1, bi->op2);
			}
			(*cpu.instructions)[offset] = std::move(replacement);
			return 1;
		}
	};

	inline reg parse_reg(const std::string& ins) {
		return ins[0];
	}

	inline operand parse_operand(const std::string& ins) {
		std::regex pattern(R"(([[:lower:]])|(-?[[:digit:]]+))");
		std::smatch m;
		std::regex_search(ins, m, pattern);
		if(m[1].matched) {
			return m[1].str()[0];
		} else if(m[2].matched) {
			return std::stoll(m[2].str());
		}
		__assume(0);
	}

	template<typename T>
	instruction_ptr parse_unary(const std::string& ins) {
		const std::regex pattern(R"((-?[[:alnum:]]+))", std::regex::optimize);
		std::smatch m;
		std::regex_search(ins, m, pattern);
		operand o1 = parse_operand(m[1].str());
		return std::make_unique<T>(o1);
	}

	template<typename T>
	instruction_ptr parse_binary(const std::string& ins) {
		const std::regex pattern(R"((-?[[:alnum:]]+) (-?[[:alnum:]]+))", std::regex::optimize);
		std::smatch m;
		std::regex_search(ins, m, pattern);
		operand o1 = parse_operand(m[1].str());
		operand o2 = parse_operand(m[2].str());
		return std::make_unique<T>(o1, o2);
	}

	template<typename T>
	instruction_ptr parse_ternary(const std::string& ins) {
		const std::regex pattern(R"((-?[[:alnum:]]+) (-?[[:alnum:]]+) (-?[[:alnum:]]+))", std::regex::optimize);
		std::smatch m;
		std::regex_search(ins, m, pattern);
		operand o1 = parse_operand(m[1].str());
		operand o2 = parse_operand(m[2].str());
		operand o3 = parse_operand(m[3].str());
		return std::make_unique<T>(o1, o2, o3);
	}

	inline instruction_ptr parse_instruction(const std::string ins) {
		const std::string opcode = ins.substr(0, ins.find(' '));
		if(opcode == "inc") {
			return parse_unary<inc>(ins.substr(4));
		} else if(opcode == "dec") {
			return parse_unary<dec>(ins.substr(4));
		} else if(opcode == "tgl") {
			return parse_unary<tgl>(ins.substr(4));
		} else if(opcode == "out") {
			return parse_unary<out>(ins.substr(4));
		} else if(opcode == "set") {
			return parse_binary<set>(ins.substr(4));
		} else if(opcode == "cpy") {
			return parse_binary<cpy>(ins.substr(4));
		} else if(opcode == "add") {
			return parse_binary<add>(ins.substr(4));
		} else if(opcode == "sub") {
			return parse_binary<sub>(ins.substr(4));
		} else if(opcode == "mul") {
			return parse_binary<mul>(ins.substr(4));
		} else if(opcode == "div") {
			return parse_binary<div>(ins.substr(4));
		} else if(opcode == "mod") {
			return parse_binary<mod>(ins.substr(4));
		} else if(opcode == "sqrt") {
			return parse_binary<sqrt>(ins.substr(4));
		} else if(opcode == "jmp") {
			return parse_unary<jmp>(ins.substr(4));
		} else if(opcode == "jnz") {
			return parse_binary<jnz>(ins.substr(4));
		} else if(opcode == "jgz") {
			return parse_binary<jgz>(ins.substr(4));
		}
		__assume(0);
	}

	inline void virtualize_jumps(program& insns) {
		for(std::size_t i = 0; i < insns.size(); ++i) {
			const instruction_ptr& ins = insns[i];
			jmp* const j = dynamic_cast<jmp*>(ins.get());
			if(j && std::holds_alternative<std::ptrdiff_t>(j->destination)) {
				const std::ptrdiff_t relative_address = std::get<ptrdiff_t>(j->destination);
				const std::ptrdiff_t absolute_address = gsl::narrow_cast<std::ptrdiff_t>(i) + relative_address;
				if(absolute_address >= gsl::narrow_cast<std::ptrdiff_t>(insns.size())) {
					using ip = instruction*;
					j->destination = ip { nullptr };
				} else {
					j->destination = { insns[gsl::narrow_cast<std::size_t>(absolute_address)].get() };
				}
			}
		}
	}

	inline void devirtualize_jumps(program& insns) {
		for(std::size_t i = 0; i < insns.size(); ++i) {
			insns[i]->abs_addr = gsl::narrow_cast<std::ptrdiff_t>(i);
		}

		for(std::size_t i = 0; i < insns.size(); ++i) {
			const instruction_ptr& ins = insns[i];
			jmp* j = dynamic_cast<jmp*>(ins.get());
			if(j) {
				if(std::holds_alternative<instruction*>(j->destination)) {
					const instruction* const target = std::get<instruction*>(j->destination);
					if(target != nullptr) {
						const std::ptrdiff_t delta = target->abs_addr - ins->abs_addr;
						j->destination = delta;
					} else {
						j->destination = std::ptrdiff_t{ 0 };
					}
				}
			}
		}
	}

	template<typename T>
	std::ptrdiff_t find_instruction(program& insns, std::ptrdiff_t start, std::ptrdiff_t step, reg arg1) {
		for(std::ptrdiff_t i = start + step; i != start; i = gsl::narrow_cast<std::ptrdiff_t>((i + step) % insns.size())) {
			const instruction_ptr& ins = insns[gsl::narrow_cast<std::size_t>(i)];
			if(T* t = dynamic_cast<T*>(ins.get()); t != nullptr) {
				if(std::get<reg>(t->op1) == arg1) {
					return i;
				}
			}
		}
		return -1;
	}

	inline void optimize(program& insns) {
		const auto fix_jumps = [&](const instruction* first, const instruction* second, instruction* replacement) -> bool {
			// jumps pointing at second should return false and disable the replacement entirely
			if(second != nullptr) {
				for(std::size_t i = 0; i < insns.size(); ++i) {
					const instruction_ptr& ins = insns[i];
					if(jmp* const j = dynamic_cast<jmp*>(ins.get()); j != nullptr) {
						if(std::get<instruction*>(j->destination) == second) {
							return false;
						}
					}
				}
			}
			// jumps pointing at first should be redirected to replacement
			for(std::size_t i = 0; i < insns.size(); ++i) {
				const instruction_ptr& ins = insns[i];
				if(jmp* const j = dynamic_cast<jmp*>(ins.get()); j != nullptr && std::holds_alternative<instruction*>(j->destination)) {
					if(std::get<instruction*>(j->destination) == first) {
						std::get<instruction*>(j->destination) = replacement;
					}
				}
			}
			return true;
		};

		const auto set_sub_neg_to_add = [&]() -> bool {
			for(std::size_t i = 0; i < insns.size(); ++i) {
				if(sub* ins1 = dynamic_cast<sub*>(insns[i].get()); ins1 != nullptr) {
					if(std::holds_alternative<std::ptrdiff_t>(ins1->op2)) {
						const std::ptrdiff_t value = std::get<std::ptrdiff_t>(ins1->op2);
						if(value < 0) {
							instruction_ptr replacement = std::make_unique<add>(ins1->op1, operand{ -value });
							if(fix_jumps(ins1, nullptr, replacement.get())) {
								insns[i] = std::move(replacement);
							}
						}
					}
				}
			}
			return false;
		};

		const auto set_arith_to_tern_arith = [&]() -> bool {
			for(std::size_t i = 1; i < insns.size(); ++i) {
				const set* const                 s1 = dynamic_cast<const set*>(insns[i - 1].get());
				const binary_instruction* const bi1 = dynamic_cast<const binary_instruction*>(insns[i - 0].get());
				if(s1 != nullptr
				&& bi1 != nullptr) {
					if(s1->op1 == bi1->op2) {
						instruction_ptr replacement = nullptr;
						if(dynamic_cast<const sub*>(bi1) != nullptr) {
							replacement = std::make_unique<tsub>(s1->op1, s1->op2, bi1->op2);
						} else if(dynamic_cast<const add*>(bi1) != nullptr) {
							replacement = std::make_unique<tadd>(s1->op1, s1->op2, bi1->op2);
						} else if(dynamic_cast<const mul*>(bi1) != nullptr) {
							replacement = std::make_unique<tmul>(s1->op1, s1->op2, bi1->op2);
						} else if(dynamic_cast<const mod*>(bi1) != nullptr) {
							replacement = std::make_unique<tmod>(s1->op1, s1->op2, bi1->op2);
						} else if(dynamic_cast<const div*>(bi1) != nullptr) {
							replacement = std::make_unique<tdiv>(s1->op1, s1->op2, bi1->op2);
						} else {
							continue;
						}
						if(fix_jumps(s1, bi1, replacement.get())) {
							insns[i - 1] = std::move(replacement);
							insns.erase(insns.begin() + gsl::narrow_cast<std::ptrdiff_t>(i),
							            insns.begin() + gsl::narrow_cast<std::ptrdiff_t>(i) + 1);
							return true;
						}
					}
				}
			}
			return false;
		};

		const auto use_mod = [&]() -> bool {
			for(std::size_t i = 0; i < insns.size(); ++i) {
				if(set* ins1 = dynamic_cast<set*>(insns[i].get()); ins1 != nullptr && 'e' == std::get<reg>(ins1->op1)) {
					instruction_ptr replacement = std::make_unique<set>('g', 'b');
					if(fix_jumps(ins1, nullptr, replacement.get())) {
						insns[i + 0] = std::move(replacement);
						insns[i + 1] = std::make_unique<mod>('g', 'd');

						const std::ptrdiff_t target_offset = find_instruction<binary_instruction>(insns,
						                                                                          gsl::narrow_cast<std::ptrdiff_t>(i + 1),
						                                                                          1,
						                                                                          'd');
						if(target_offset == -1) {
							break;
						}
						instruction* target = insns[gsl::narrow_cast<std::size_t>(target_offset)].get();

						insns[i + 2] = std::make_unique<jnz>('g', target);
						insns.erase(insns.begin() + gsl::narrow_cast<std::ptrdiff_t>(i) + 3,
						            insns.begin() + gsl::narrow_cast<std::ptrdiff_t>(i) + 5);
						for(std::size_t j = i + 2; j < insns.size() - 3; ++j) {
							bool update_e_found = false;
							if(const sub* const su = dynamic_cast<sub*>(insns[j].get()); su != nullptr && 'e' == std::get<reg>(su->op1)) {
								update_e_found = true;
							}
							if(const add* const ad = dynamic_cast<add*>(insns[j].get()); ad != nullptr && 'e' == std::get<reg>(ad->op1)) {
								update_e_found = true;
							}
							if(update_e_found) {
								insns.erase(insns.begin() + gsl::narrow_cast<std::ptrdiff_t>(j) + 0,
								            insns.begin() + gsl::narrow_cast<std::ptrdiff_t>(j) + 4);
								break;
							}
						}
						return true;
					}
				}
			}
			return false;
		};

		const auto bail_early = [&]() -> bool {
			for(std::size_t i = 1; i < insns.size(); ++i) {
				const set* const s1 = dynamic_cast<set*>(insns[i - 1].get());
				const jmp* const j1 = dynamic_cast<jmp*>(insns[i - 0].get());
				if(s1 != nullptr
				&& j1 == nullptr) {
					if('f' == std::get<reg>(s1->op1)
					&& operand{ std::ptrdiff_t { 0 } } == s1->op2) {
						const std::ptrdiff_t target_offset = find_instruction<binary_instruction>(insns,
						                                                                          gsl::narrow_cast<std::ptrdiff_t>(i + 1),
						                                                                          1,
						                                                                          'h');
						if(target_offset == -1) {
							break;
						}
						instruction* target = insns[gsl::narrow_cast<std::size_t>(target_offset)].get();

						instruction_ptr j = std::make_unique<jmp>(operand{ target });
						insns.insert(insns.begin() + gsl::narrow_cast<std::ptrdiff_t>(i) + 1, std::move(j));
						return true;
					}
				}
			}
			return false;
		};

		const auto use_sqrt = [&]() -> bool {
			for(std::size_t i = 0; i < insns.size(); ++i) {
				bool sqrt_target = false;
				bool has_slack = true;
				if(const set* const ins1 = dynamic_cast<set*>(insns[i].get()); ins1 != nullptr && 'g' == std::get<reg>(ins1->op1) && operand{ 'd' } == ins1->op2) {
					sqrt_target = true;
				}
				if(const tsub* const ins1 = dynamic_cast<tsub*>(insns[i].get()); ins1 != nullptr && 'g' == std::get<reg>(ins1->op1) && operand{ 'd' } == ins1->op2 && operand{ 'b' } == ins1->op3) {
					sqrt_target = true;
					has_slack = false;
				}
				if(sqrt_target) {
					instruction_ptr replacement = std::make_unique<sqrt>('g', operand{ 'b' });
					if(fix_jumps(insns[i].get(), nullptr, replacement.get())) {
						insns[i + 0] = std::move(replacement);
						if(!has_slack) {
							insns.insert(insns.begin() + gsl::narrow_cast<std::ptrdiff_t>(i) + 1, nullptr);
						}
						insns[i + 1] = std::make_unique<sub>('g', operand{ 'd' });

						jmp* j = dynamic_cast<jmp*>(insns[i + 2].get());
						insns[i + 2] = std::make_unique<jgz>('g', j->destination);
						return true;
					}
				}
			}
			return false;
		};

		const auto uncond_jmp = [&]() -> bool {
			for(std::size_t i = 0; i < insns.size(); ++i) {
				if(conditional_jump* j = dynamic_cast<conditional_jump*>(insns[i].get()); j != nullptr) {
					if(std::holds_alternative<std::ptrdiff_t>(j->control)
					&& std::get<std::ptrdiff_t>(j->control) != 0) {
						instruction_ptr replacement = std::make_unique<jmp>(j->destination);
						if(fix_jumps(insns[i].get(), nullptr, replacement.get())) {
							insns[i] = std::move(replacement);
							return true;
						}
					}
				}
			}
			return false;
		};

		const auto inc_to_mul = [&]() -> bool {
			for(std::size_t i = 5; i < insns.size(); ++i) {
				const instruction* const i1 =                  insns[i - 5].get();
				const inc* const a1 = dynamic_cast<const inc*>(insns[i - 4].get());
				const dec* const d1 = dynamic_cast<const dec*>(insns[i - 3].get());
				const jnz* const j1 = dynamic_cast<const jnz*>(insns[i - 2].get());
				const dec* const d2 = dynamic_cast<const dec*>(insns[i - 1].get());
				const jnz* const j2 = dynamic_cast<const jnz*>(insns[i - 0].get());
				if(a1 != nullptr && d1 != nullptr && j1 != nullptr && d2 != nullptr && j2 != nullptr) {
					if(d1->op1 == j1->control
					&& d2->op1 == j2->control
					&& a1->op1 != d1->op1
					&& a1->op1 != d2->op1
					&& std::get<instruction*>(j1->destination) == a1
					&& std::get<instruction*>(j2->destination) == i1) {
						operand accumulator = a1->op1;
						operand mul1 = d1->op1;
						operand mul2 = d2->op1;
						insns[i - 4] = std::make_unique<mul>(mul1, mul2);
						insns[i - 3] = std::make_unique<add>(accumulator, mul1);
						insns[i - 2] = std::make_unique<cpy>(std::ptrdiff_t{ 0 }, mul1);
						insns[i - 1] = std::make_unique<cpy>(std::ptrdiff_t{ 0 }, mul2);
						insns[i - 0] = std::make_unique<jnz>(mul1, std::ptrdiff_t{ 1 });
					}
				}
			}
			return false;
		};

		bool did_work = false;
		do {
			did_work = use_mod() || false;
			did_work = bail_early() || did_work;
			did_work = use_sqrt() || did_work;
			did_work = set_sub_neg_to_add() || did_work;
			did_work = set_arith_to_tern_arith() || did_work;
			did_work = uncond_jmp() || did_work;
			did_work = inc_to_mul() || did_work;
		} while(did_work);
	}

	inline std::string print_asm(const program& p) {
		std::string result;
		for(std::size_t i = 0; i < p.size(); ++i) {
			result += p[i]->emit_asm() + "\n";
		}
		return result;
	}
}

#endif
