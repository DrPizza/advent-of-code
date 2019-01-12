#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>

#include <range/v3/all.hpp>

struct advent_2018_14 : problem
{
	advent_2018_14() noexcept : problem(2018, 14) {
	}

protected:
	std::string recipe_count;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, recipe_count);
	}

	std::string part_1() override {
		std::vector<std::uint8_t> recipes = { 3ui8, 7ui8 };
		std::size_t elf_1 = 0ui64;
		std::size_t elf_2 = 1ui64;
		const std::ptrdiff_t limit = std::stoll(recipe_count);
		for(std::ptrdiff_t i = 0i64; i != limit + 9i64; ++i) {
			const std::uintmax_t new_recipe = std::uintmax_t{ recipes[elf_1] } + recipes[elf_2];
			if(new_recipe > 9ui64) {
				recipes.push_back(gsl::narrow_cast<std::uint8_t>(new_recipe / 10ui64));
			}
			recipes.push_back(gsl::narrow_cast<std::uint8_t>(new_recipe % 10ui64));
			elf_1 += (recipes[elf_1] + 1ui64);
			elf_2 += (recipes[elf_2] + 1ui64);
			elf_1 %= recipes.size();
			elf_2 %= recipes.size();
		}
		const std::vector<std::uint8_t> answer(std::next(std::begin(recipes), limit), std::next(std::begin(recipes), limit + 10i64));
		return answer | ranges::view::transform([] (std::uint8_t v) -> char { return v + '0'; });
	}

	std::string part_2() override {
		std::vector<std::uint8_t> recipes = { 3ui8, 7ui8 };
		std::vector<std::uint8_t> input_digits = recipe_count | ranges::view::transform([] (char ch) { return gsl::narrow_cast<std::uint8_t>(ch - '0'); });
		std::size_t elf_1 = 0ui64;
		std::size_t elf_2 = 1ui64;
		for(std::size_t i = 0ui64; i < 20'000'000ui64; ++i) {
			const std::uintmax_t new_recipe = std::uintmax_t{ recipes[elf_1] } + recipes[elf_2];
			if(new_recipe > 9ui64) {
				recipes.push_back(gsl::narrow_cast<std::uint8_t>(new_recipe / 10ui64));
			}
			recipes.push_back(gsl::narrow_cast<std::uint8_t>(new_recipe % 10ui64));
			elf_1 += (recipes[elf_1] + 1ui64);
			elf_2 += (recipes[elf_2] + 1ui64);
			elf_1 %= recipes.size();
			elf_2 %= recipes.size();
		}
		auto end = std::search(std::begin(recipes), std::end(recipes), std::begin(input_digits), std::end(input_digits));
		const std::ptrdiff_t offset = std::distance(std::begin(recipes), end);
		return std::to_string(offset);
	}
};

REGISTER_SOLVER(2018, 14);
