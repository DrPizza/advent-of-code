#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <map>

#include <range/v3/all.hpp>

struct advent_2018_7 : problem
{
	advent_2018_7() noexcept : problem(2018, 7) {
	}

protected:
	std::map<char, std::set<char>> flow;
	std::multimap<char, char> dependencies;

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(Step ([[:alnum:]]) must be finished before step ([[:alnum:]]) can begin.)");
		for(std::string line; std::getline(fin, line); ) {
			std::smatch m;
			if(std::regex_match(line, m, pattern)) {
				const char src = m[1].str()[0];
				const char tgt = m[2].str()[0];
				flow[src].insert(tgt);
				dependencies.insert({ tgt, src });
			}
		}
	}

	std::string part_1() override {
		std::map<char, std::set<char>> my_flow(flow);
		std::multimap<char, char> my_deps(dependencies);

		std::string sequence;

		std::set<char> possible = my_flow | ranges::view::keys | ranges::view::filter([&my_deps] (char ch) { return my_deps.find(ch) == my_deps.end(); });
		while(possible.size() > 0) {
			const char ch = *possible.begin();
			sequence += ch;
			possible.erase(ch);
			for(const char c : my_flow[ch]) {
				const auto rng = my_deps.equal_range(c);
				for(auto i = rng.first; i != rng.second; ) {
					if(i->second == ch) {
						my_deps.erase(i++);
					} else {
						++i;
					}
				}
				if(my_deps.find(c) == my_deps.end()) {
					possible.insert(c);
				}
			}
			my_flow.erase(my_flow.find(ch));
		}

		return sequence;
	}

	struct worker
	{
		char work_id;
		std::uintmax_t time;
	};

	std::string part_2() override {
		std::uintmax_t time = 0ui64;
		std::vector<worker> workers(5);

		std::map<char, std::set<char>> my_flow(flow);
		std::multimap<char, char> my_deps(dependencies);

		std::set<char> possible = my_flow | ranges::view::keys | ranges::view::filter([&my_deps] (char ch) { return my_deps.find(ch) == my_deps.end(); });

		for(; !possible.empty() || (ranges::max(workers | ranges::view::transform(&worker::time)) != 0); ++time) {
			for(auto& w : workers) {
				if(w.work_id != 0) {
					if(--w.time == 0) {
						for(const char c : my_flow[w.work_id]) {
							const auto rng = my_deps.equal_range(c);
							for(auto i = rng.first; i != rng.second; ) {
								if(i->second == w.work_id) {
									my_deps.erase(i++);
								} else {
									++i;
								}
							}
							if(my_deps.find(c) == my_deps.end()) {
								possible.insert(c);
							}
						}
						w.work_id = 0;
						w.time = 0;
					}
				}
			}
			for(auto& w : workers) {
				if(w.work_id == 0 && !possible.empty()) {
					w.work_id = *possible.begin();
					possible.erase(w.work_id);
					w.time = 61ui64 + (w.work_id - 'A');
				}
			}
		}
		return std::to_string(time);
	}
};

REGISTER_SOLVER(2018, 7);
