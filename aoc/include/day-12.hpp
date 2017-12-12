#pragma once

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <boost/algorithm/string.hpp>

struct advent_12 : problem
{
	advent_12() noexcept : problem(12) {
	}

	std::vector<std::vector<std::size_t>> programs;

	std::unordered_map<std::size_t, std::unordered_set<std::size_t>> groups;

	void prepare_input() override {
		std::ifstream fin("day-12.txt");
		for(std::string line; std::getline(fin, line); ) {
			std::vector<std::string> fragments;
			boost::split(fragments, line, [](char c) { return c == ' '; });
			programs.push_back(std::vector<std::size_t>{});
			std::vector<std::size_t>& children = programs.back();
			for(std::size_t i = 2; i != fragments.size(); ++i) {
				children.push_back(std::stoull(fragments[i].substr(0, fragments[i].find(','))));
			}
		}
	}

	void build_group(std::size_t parent_pid, std::unordered_set<std::size_t>& visited) {
		for(const std::size_t pid : programs[parent_pid]) {
			if(visited.find(pid) == std::end(visited)) {
				visited.insert(pid);
				build_group(pid, visited);
			}
		}
	}

	bool found_in_existing_groups(std::size_t pid) {
		for(const auto& g : groups) {
			if(g.second.find(pid) != std::end(g.second)) {
				return true;
			}
		}
		return false;
	}

	void precompute() noexcept override {
		for(std::size_t i = 0; i < programs.size(); ++i) {
			if(!found_in_existing_groups(i)) {
				std::unordered_set<std::size_t>& group = groups[i];
				build_group(i, group);
			}
		}
	}

	std::string part_1() override {
		return std::to_string(groups[0].size());
	}

	std::string part_2() override {
		return std::to_string(groups.size());;
	}
};
