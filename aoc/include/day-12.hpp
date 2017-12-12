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

	struct program
	{
		std::size_t pid;
		std::vector<std::size_t> children;
	};

	std::unordered_map<std::size_t, program> programs;

	void prepare_input() override {
		std::ifstream fin("day-12.txt");
		for(std::string line; std::getline(fin, line); ) {
			std::vector<std::string> fragments;
			boost::split(fragments, line, [](char c) { return c == ' '; });
			program p = { std::stoull(fragments[0]) };
			for(std::size_t i = 2; i != fragments.size(); ++i) {
				p.children.push_back(std::stoull(fragments[i].substr(0, fragments[i].find(','))));
			}
			programs[p.pid] = p;
		}
	}

	void build_group(std::size_t parent_pid, std::unordered_set<std::size_t>& visited) {
		for(const std::size_t pid : programs[parent_pid].children) {
			if(visited.find(pid) == std::end(visited)) {
				visited.insert(pid);
				build_group(pid, visited);
			}
		}
	}

	bool found_in_existing_groups(std::size_t pid, const std::vector<std::unordered_set<std::size_t>>& groups) {
		for(const auto& g : groups) {
			if(g.find(pid) != std::end(g)) {
				return true;
			}
		}
		return false;
	}

	std::string part_1() override {
		std::unordered_set<std::size_t> group;
		build_group(0, group);
		return std::to_string(group.size());
	}

	std::string part_2() override {
		std::vector<std::unordered_set<std::size_t>> groups;
		for(const auto& p : programs) {
			if(!found_in_existing_groups(p.first, groups)) {
				std::unordered_set<std::size_t> group;
				build_group(p.first, group);
				groups.push_back(group);
			}
		}
		return std::to_string(groups.size());;
	}
};
