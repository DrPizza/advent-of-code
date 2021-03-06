#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <regex>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string.hpp>

#pragma warning(disable: 26495) // warning C26495: Variable '%s' is uninitialized. Always initialize a member variable (type.6).

struct advent_2017_7 : problem
{
	advent_2017_7() noexcept : problem(2017, 7) {
	}

	struct node
	{
		std::string name;
		std::size_t weight;
		std::size_t total_weight;
		node* parent;
		std::vector<std::string> children;
		std::vector<gsl::not_null<node*>> child_nodes;
	};

protected:
	std::unordered_map<std::string, node> nodes;
	node* tree = nullptr;

	std::size_t set_total_weights(gsl::not_null<node*> n) {
		std::size_t total_weight = n->weight;
		for(const gsl::not_null<node*> ch : n->child_nodes) {
			total_weight += set_total_weights(ch);
		}
		n->total_weight = total_weight;
		return total_weight;
	}

	gsl::not_null<node*> find_bad_node(gsl::not_null<node*> n) {
		auto odd_one = utility::find_odd_one_out(std::begin(n->child_nodes), std::end(n->child_nodes), [](gsl::not_null<node*> lhs, gsl::not_null<node*> rhs) {
			return lhs->total_weight == rhs->total_weight;
		});
		if(odd_one != std::end(n->child_nodes)) {
			return find_bad_node(*odd_one);
		}
		return n;
	}

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(([[:lower:]]+) \(([[:digit:]]+)\)( -> (.*))?)");
		std::regex child_pattern(R"(([[:lower:]]+)(, )?)");
		for(std::string line; std::getline(fin, line);) {
			std::smatch m;
			std::regex_search(line, m, pattern);
			const std::string name   = m[1];
			const std::size_t weight = std::stoull(m[2]);
			node n{ name, weight, 0, nullptr };
			//node n{ m[1], std::stoull(m[2]), 0, nullptr };
			if(m[4].matched) {
				std::string fragment = m[4].str();
				auto child_it        = std::sregex_iterator(std::begin(fragment), std::end(fragment), child_pattern);
				auto child_end       = std::sregex_iterator();
				for(; child_it != child_end; ++child_it) {
					std::smatch c = *child_it;
					n.children.push_back(c[1]);
				}
			}
			nodes[n.name] = n;
		}
		for(auto it = std::begin(nodes); it != std::end(nodes); ++it) {
			for(auto& child_name : it->second.children) {
				const gsl::not_null<node*> child = gsl::make_not_null(&nodes[child_name]);

				child->parent = &it->second;
				it->second.child_nodes.push_back(child);
			}
		}
		tree = &std::begin(nodes)->second;
		while(tree->parent != nullptr) {
			tree = tree->parent;
		}
		set_total_weights(gsl::make_not_null(tree));
	}

	std::string part_1() override {
		return tree->name;
	}

	std::string part_2() override {
		const gsl::not_null<node*> bad_node = find_bad_node(gsl::make_not_null(tree));
		const std::size_t bad_total         = bad_node->total_weight;
		for(const gsl::not_null<const node*> sib : bad_node->parent->child_nodes) {
			if(sib->total_weight != bad_total) {
				const std::ptrdiff_t discrepancy = gsl::narrow<std::ptrdiff_t>(sib->total_weight) - gsl::narrow<std::ptrdiff_t>(bad_total);
				const std::size_t correct_weight = gsl::narrow<std::size_t>(gsl::narrow<std::ptrdiff_t>(bad_node->weight) + discrepancy);
				return std::to_string(correct_weight);
			}
		}
		__assume(0);
	}
};

REGISTER_SOLVER(2017, 7);
