#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>

#include <range/v3/all.hpp>

struct advent_2018_8 : problem
{
	advent_2018_8() noexcept : problem(2018, 8) {
	}

protected:
	struct node
	{
		std::vector<node> children;
		std::vector<std::uintmax_t> metadata;
	};

	node read_node(std::istream_iterator<std::uintmax_t>& it) {
		node n;
		const std::size_t child_count = *it++;
		const std::size_t metadata_count = *it++;
		for(std::size_t i = 0; i < child_count; ++i) {
			n.children.push_back(read_node(it));
		}
		for(std::size_t i = 0; i < metadata_count; ++i) {
			n.metadata.push_back(*it++);
		}
		return n;
	}

	node root;

	void prepare_input(std::ifstream& fin) override {
		std::istream_iterator<std::uintmax_t> it(fin);
		root = read_node(it);
	}

	static std::uintmax_t sum_metadata(const node& n) {
		return ranges::accumulate(n.children | ranges::view::transform(sum_metadata), 0)
		     + ranges::accumulate(n.metadata, 0ui64);
	}

	std::string part_1() override {
		return std::to_string(sum_metadata(root));
	}

	static std::uintmax_t calc_value(const node& n) {
		return n.children.size() == 0 ? sum_metadata(n)
		                              : ranges::accumulate(n.metadata
		                                                  | ranges::view::filter   ([&](std::size_t idx) { return idx > 0 && idx <= n.children.size(); })
		                                                  | ranges::view::transform([&](std::size_t idx) { return calc_value(n.children[idx - 1]); })
		                                                  , 0);
	}

	std::string part_2() override {
		return std::to_string(calc_value(root));
	}
};

REGISTER_SOLVER(2018, 8);
