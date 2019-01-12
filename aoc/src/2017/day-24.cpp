#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <string>
#include <unordered_map>

struct advent_2017_24 : problem
{
	advent_2017_24() noexcept : problem(2017, 24) {
	}

protected:
	using component = std::pair<std::size_t, std::size_t>;
	//using component = std::pair<int, int>;
	std::vector<component> components;
	std::vector<bool> used;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			const std::size_t f = std::stoull(line.substr(0ui64, line.find('/')));
			const std::size_t s = std::stoull(line.substr(line.find('/') + 1ui64, std::string::npos));
			const component c = std::make_pair(f, s);
			components.push_back(c);
		}
		used.resize(components.size(), false);
	}

	std::size_t highest_strength = 0;
	std::size_t longest_length = 0;
	std::size_t highest_longest_strength = 0;

	void find_strongest_bridge(std::size_t next_port,
	                           std::size_t current_strength,
	                           std::size_t current_length) {
		highest_strength = std::max(highest_strength, current_strength);
		if(current_length > longest_length) {
			longest_length = current_length;
			highest_longest_strength = current_strength;
		} else if(current_length == longest_length) {
			highest_longest_strength = std::max(highest_longest_strength, current_strength);
		}

		for(std::size_t i = 0; i < components.size(); ++i) {
			if(!used[i]) {
				const component c = components[i];
				if(c.first != next_port && c.second != next_port) {
					continue;
				}

				const std::size_t unused_port = c.first == next_port ? c.second : c.first;
				used[i] = true;
				find_strongest_bridge(unused_port, current_strength + c.first + c.second, current_length + 1);
				used[i] = false;
			}
		}
	}

	void precompute() override {
		find_strongest_bridge(0, 0, 0);
	}

	std::string part_1() override {
		return std::to_string(highest_strength);
	}
	std::string part_2() override {
		return std::to_string(highest_longest_strength);
	}
};

REGISTER_SOLVER(2017, 24);
