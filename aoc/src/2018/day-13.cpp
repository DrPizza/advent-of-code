#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>

namespace
{
	std::vector<std::string> track;
	std::vector<std::string> pristine_track;
	std::vector<std::string> cart_locations;
	std::vector<std::string> pristine_cart_locations;
	struct cart;
	std::vector<cart> carts;
	std::vector<cart> pristine_carts;

	struct cart
	{
		cart(std::size_t row, std::size_t col, std::intmax_t dir) noexcept : r(row), c(col), direction(dir) {
		}

		bool drive() {
			if(crashed) {
				return false;
			}

			cart_locations[r][c] = ' ';
			c += movements[direction].first;
			r += movements[direction].second;

			if(cart_locations[r][c] != ' ') {
				crashed = true;
				return true;
			}
			switch(track[r][c]) {
			case '/':
				direction ^= 1;
				break;
			case '\\':
				direction = 3 - direction;
				break;
			case '+':
				direction = (direction + turns[turn_status]) % 4;
				turn_status = (turn_status + 1) % 3;
				break;
			case '-':
			case '|':
				break;
			}

			cart_locations[r][c] = symbols[direction];
			return false;
		}

		friend bool operator<(const cart& lhs, const cart& rhs) noexcept {
			return std::tie(lhs.r, lhs.c) < std::tie(rhs.r, rhs.c);
		}

		std::size_t r = 0;
		std::size_t c = 0;

		bool crashed = false;

		std::intmax_t direction = 0;
		std::intmax_t turn_status = 0;

		static constexpr std::pair<std::intmax_t, std::intmax_t> movements[4] = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
		static constexpr std::intmax_t turns[3] = { 3, 0, 1 };
		static constexpr char symbols[4] = { '^', '>', 'v', '<' };
	};
}

struct advent_2018_13 : problem
{
	advent_2018_13() : problem(2018, 13) {
	}

protected:
	void draw() const {
		COORD top_left = { 0, 0 };
		::SetConsoleCursorPosition(::GetStdHandle(STD_OUTPUT_HANDLE), top_left);

		std::vector<std::string> combined = track;
		for(std::size_t row = 0; row < track.size(); ++row) {
			for(std::size_t col = 0; col < track[row].size(); ++col) {
				if(cart_locations[row][col] != ' ') {
					combined[row][col] = cart_locations[row][col];
				}
			}
		}

		for(std::size_t row = 0; row < track.size(); ++row) {
			for(std::size_t col = 0; col < track[row].size(); ++col) {
				std::cout << combined[row][col];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	void prepare_input(std::ifstream& fin) override {
		pristine_track = ranges::getlines(fin) | ranges::to_vector;
		for(std::size_t row = 0; row < pristine_track.size(); ++row) {
			pristine_cart_locations.push_back(std::string(pristine_track[row].size(), ' '));
			for(std::size_t col = 0; col < pristine_track[row].size(); ++col) {
				switch(pristine_track[row][col]) {
				case '^':
					pristine_carts.push_back({ row, col, 0 });
					pristine_cart_locations[row][col] = '^';
					pristine_track[row][col] = '|';
					break;
				case '>':
					pristine_carts.push_back({ row, col, 1 });
					pristine_cart_locations[row][col] = '>';
					pristine_track[row][col] = '-';
					break;
				case 'v':
					pristine_carts.push_back({ row, col, 2 });
					pristine_cart_locations[row][col] = 'v';
					pristine_track[row][col] = '|';
					break;
				case '<':
					pristine_carts.push_back({ row, col, 3 });
					pristine_cart_locations[row][col] = '<';
					pristine_track[row][col] = '-';
					break;
				}
			}
		}
	}

	std::string part_1() override {
		track = pristine_track;
		cart_locations = pristine_cart_locations;
		carts = pristine_carts;

		for(;;) {
			std::sort(std::begin(carts), std::end(carts));
			for(cart& c : carts) {
				if(c.drive()) {
					return std::to_string(c.c) + "," + std::to_string(c.r);
				}
			}
		}
	}

	std::string part_2() override {
		track = pristine_track;
		cart_locations = pristine_cart_locations;
		carts = pristine_carts;

		for(;;) {
			std::sort(std::begin(carts), std::end(carts));
			for(cart& c : carts) {
				if(c.drive()) {
					for(cart& victim : carts) {
						if(victim.r == c.r && victim.c == c.c && !victim.crashed) {
							cart_locations[c.r][c.c] = ' ';
							victim.crashed = true;
						}
					}
				}
			}

			if(1ui64 == ranges::count(carts, false, &cart::crashed)) {
				const cart& c = *ranges::find(carts, false, &cart::crashed);
				return std::to_string(c.c) + "," + std::to_string(c.r);
			}
		}
	}
};

REGISTER_SOLVER(2018, 13);
