#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <vector>
#include <list>

#include <range/v3/all.hpp>

struct advent_2018_9 : problem
{
	advent_2018_9() noexcept : problem(2018, 9) {
	}

protected:
	std::uintmax_t players = 0;
	std::uintmax_t score = 0;

	void prepare_input(std::ifstream& fin) override {
		std::string line;
		std::getline(fin, line);
		std::regex pattern(R"(([[:digit:]]+) players; last marble is worth ([[:digit:]]+) points)");
		std::smatch m;
		std::regex_match(line, m, pattern);
		players = std::stoull(m[1]);
		score = std::stoull(m[2]);
	}

	std::size_t play_game(std::uintmax_t max_players, std::uintmax_t last_score) {
		std::list<std::uintmax_t> circle;
		circle.push_back(0);

		std::vector<std::size_t> scores(max_players);

		using cl_it = utility::cyclic_iterator<std::list<std::uintmax_t>::iterator>;
		cl_it current(std::begin(circle), std::end(circle));

		for(std::uintmax_t marble = 1; marble < last_score; ++marble) {
			if(marble % 23 != 0) {
				std::advance(current, 2);
				current = cl_it(circle.insert(current.base(), marble), std::begin(circle), std::end(circle));
			} else {
				std::advance(current, -7);
				const std::uintmax_t removed = *current;
				current = cl_it(circle.erase(current.base()), std::begin(circle), std::end(circle));
				scores[marble % max_players] += marble + removed;
			}
		}
		return ranges::max(scores);
	}

	std::string part_1() override {
		return std::to_string(play_game(players, score));
	}

	std::string part_2() override {
		return std::to_string(play_game(players, score * 100));
	}
};

REGISTER_SOLVER(2018, 9);
