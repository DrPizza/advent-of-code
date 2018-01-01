#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <map>
#include <optional>
#include <array>
#include <tuple>
#include <regex>

struct advent_2016_10 : problem
{
	advent_2016_10() noexcept : problem(2016, 10) {
	}

protected:
	using chip = std::size_t;

	enum struct target_type
	{
		lower = 0,
		higher = 1,
		bot,
		output
	};

	struct target
	{
		target_type type;
		std::size_t id;
	};

	struct bot;

	static inline std::map<std::size_t, bot> bots;
	static inline std::map<std::size_t, chip> outputs;

	struct bot
	{
		std::optional<chip> chips[2];
		std::optional<target> targets[2];

		void add_chip(chip c) {
			if(!chips[0].has_value()) {
				chips[0] = c;
			} else if(!chips[1].has_value()) {
				chips[1] = c;
				if(chips[0] > chips[1]) {
					std::swap(chips[0], chips[1]);
				}
				process();
			}
		}

		void add_targets(target l, target h) {
			targets[0] = l;
			targets[1] = h;
			process();
		}

		void process() const {
			if(!has_both_chips() || !has_both_targets()) {
				return;
			}
			const target& l = targets[0].value();
			if(l.type == target_type::bot) {
				bots[l.id].add_chip(chips[0].value());
			} else {
				outputs[l.id] = chips[0].value();
			}
			const target& h = targets[1].value();
			if(h.type == target_type::bot) {
				bots[h.id].add_chip(chips[1].value());
			} else {
				outputs[h.id] = chips[1].value();
			}
		}

		bool has_both_chips() const noexcept {
			return chips[0].has_value() && chips[1].has_value();
		}

		bool has_both_targets() const noexcept {
			return targets[0].has_value() && targets[1].has_value();
		}
	};

	std::vector<std::string> instructions;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			instructions.push_back(line);
		}
	}

	void precompute() override {
		std::regex value_pattern(R"(value ([[:digit:]]+) goes to bot ([[:digit:]]+))");
		std::regex target_pattern(R"(bot ([[:digit:]]+) gives low to (output|bot) ([[:digit:]]+) and high to (output|bot) ([[:digit:]]+))");

		for(const std::string& s : instructions) {
			std::smatch m;
			if(std::regex_search(s, m, value_pattern)) {
				const std::size_t id = std::stoull(m[2].str());
				const std::size_t value = std::stoull(m[1].str());
				bots[id].add_chip(value);
			} else if(std::regex_search(s, m, target_pattern)) {
				const std::size_t id = std::stoull(m[1].str());
				const target_type low_type = m[2].str() == "output" ? target_type::output : target_type::bot;
				const std::size_t low_target = std::stoull(m[3].str());
				const target_type high_type = m[4].str() == "output" ? target_type::output : target_type::bot;
				const std::size_t high_target = std::stoull(m[5].str());
				bots[id].add_targets(target{ low_type, low_target },
				                     target{ high_type, high_target });
			}
		}
	}

	std::string part_1() override {
		std::size_t result = 0u;
		for(const auto& p : bots) {
			if(p.second.chips[0] == 17u
			&& p.second.chips[1] == 61u) {
				result = p.first;
			}
		}
		return std::to_string(result);
	}

	std::string part_2() override {
		const std::size_t product = outputs[0] * outputs[1] * outputs[2];
		return std::to_string(product);
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_10>() {
	advent_2016_10 a;
	a.solve();
}
