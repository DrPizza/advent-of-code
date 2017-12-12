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

	template<typename T>
	struct disjoint_set
	{
		struct element
		{
			T value;
			std::size_t size;
			std::size_t rank;
			element* parent;
		};

		std::size_t set_size(const T& id) {
			return find_parent(id)->size;
		}

		bool connected(const T& left, const T& right) {
			return table.find(left) != std::end(table)
			    && table.find(right) != std::end(table)
			    && find_parent(left) == find_parent(right);
		}

		void insert_set(const T& singleton) {
			if(table.find(singleton) == std::end(table)) {
				++distinct_sets;
				element& e = table[singleton];
				e.value = singleton;
				e.size = 1;
				e.parent = &e;
				e.rank = 0;
			}
		}

		void merge_sets(const T& left, const T& right) {
			if(table.find(left) == std::end(table)) {
				insert_set(left);
			}
			if(table.find(right) == std::end(table)) {
				insert_set(right);
			}
			gsl::not_null<element*> e1 = find_parent(left);
			gsl::not_null<element*> e2 = find_parent(right);
			if(e1 == e2) {
				return;
			}
			--distinct_sets;
			if(e1->rank < e2->rank) {
				e1->parent = e2;
				e2->size += e1->size;
			} else if(e1->rank > e2->rank) {
				e2->parent = e1;
				e1->size += e2->size;
			} else {
				e2->parent = e1;
				e1->rank += 1;
				e1->size += e2->size;
			}
		}
		
		std::size_t count_sets() const noexcept {
			return distinct_sets;
		}

	private:
		gsl::not_null<element*> find_parent(const T& key) {
			element& e = table[key];
			if(&e != e.parent) {
				e.parent = find_parent(e.parent->value);
			}
			return e.parent;
		}

		std::unordered_map<T, element> table;
		std::size_t distinct_sets = 0;
	};

	disjoint_set<std::size_t> ds;

	void prepare_input() override {
		std::ifstream fin("day-12.txt");
		for(std::string line; std::getline(fin, line); ) {
			std::vector<std::string> fragments;
			boost::split(fragments, line, [](char c) { return c == ' '; });
			for(std::size_t i = 2; i != fragments.size(); ++i) {
				ds.merge_sets(std::stoull(fragments[0]), std::stoull(fragments[i].substr(0, fragments[i].find(','))));
			}
		}
	}

	std::string part_1() override {
		return std::to_string(ds.set_size(0u));
	}

	std::string part_2() override {
		return std::to_string(ds.count_sets());
	}
};
