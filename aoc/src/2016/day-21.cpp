#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <regex>
#include <algorithm>
#include <vector>

namespace
{

	struct command
	{
		virtual void forward(std::string& s) const = 0;
		virtual void backward(std::string& s) const = 0;

		command() = default;
		command(const command&) = default;
		command(command&&) = default;
		command& operator=(const command&) = default;
		command& operator=(command&&) = default;

		virtual ~command() = default;
	};

	struct swap_position : command
	{
		std::size_t first;
		std::size_t second;

		swap_position(std::size_t first_, std::size_t second_) noexcept : first(first_), second(second_) {
		}

		void forward(std::string& s) const override {
			std::swap(s[first], s[second]);
		}

		void backward(std::string& s) const override {
			std::swap(s[first], s[second]);
		}
	};

	struct swap_letter : command
	{
		char first;
		char second;

		swap_letter(char first_, char second_) noexcept : first(first_), second(second_) {
		}

		void forward(std::string& s) const override {
			std::swap(s[s.find(first)], s[s.find(second)]);
		}

		void backward(std::string& s) const override {
			std::swap(s[s.find(first)], s[s.find(second)]);
		}
	};

	struct rotate : command
	{
		enum direction
		{
			left, right
		};
		direction dir;
		std::ptrdiff_t count;

		rotate(direction dir_, std::ptrdiff_t count_) noexcept : dir(dir_), count(count_) {
		}

		void forward(std::string& s) const override {
			std::rotate(std::begin(s), (dir == left ? std::begin(s) + count : std::end(s) - count), std::end(s));
		}

		void backward(std::string& s) const override {
			std::rotate(std::begin(s), (dir == left ? std::end(s) - count : std::begin(s) + count), std::end(s));
		}
	};

	struct rotate_position : command
	{
		char position;

		rotate_position(char position_) noexcept : position(position_) {
		}

		void forward(std::string& s) const override {
			const std::size_t offset = s.find(position);
			const std::size_t distance = (1 + offset + (offset > 4 ? 1 : 0)) % s.size();
			std::rotate(std::begin(s), std::end(s) - gsl::narrow<std::ptrdiff_t>(distance), std::end(s));
		}

		void backward(std::string& s) const override {
			const std::size_t offset = s.find(position) != 0 ? s.find(position) : s.size();
			const std::size_t distance = ((offset + (offset % 2 == 1 ? 1 : 10)) / 2) % s.size();
			std::rotate(std::begin(s), std::begin(s) + gsl::narrow<std::ptrdiff_t>(distance), std::end(s));
		}
	};

	struct reverse : command
	{
		std::ptrdiff_t first;
		std::ptrdiff_t second;

		reverse(std::ptrdiff_t first_, std::ptrdiff_t second_) noexcept : first(first_), second(second_) {
		}

		void forward(std::string& s) const override {
			std::reverse(std::begin(s) + first, std::begin(s) + second + 1ui64);
		}

		void backward(std::string& s) const override {
			std::reverse(std::begin(s) + first, std::begin(s) + second + 1ui64);
		}
	};

	struct move : command
	{
		std::size_t source;
		std::size_t destination;

		move(std::size_t source_, std::size_t destination_) noexcept : source(source_), destination(destination_) {
		}

		void forward(std::string& s) const override {
			const char c = s[source];
			s.erase(source, 1);
			s.insert(destination, 1, c);
		}

		void backward(std::string& s) const override {
			const char c = s[destination];
			s.erase(destination, 1);
			s.insert(source, 1, c);
		}
	};

	using command_ptr = std::unique_ptr<command>;
}

struct advent_2016_21 : problem
{
	advent_2016_21() noexcept : problem(2016, 21) {
	}

protected:
	std::vector<command_ptr> commands;

	void prepare_input(std::ifstream& fin) override {
		std::regex swap_pos(R"(swap position ([[:digit:]]) with position ([[:digit:]]))");
		std::regex swap_let(R"(swap letter ([[:lower:]]) with letter ([[:lower:]]))");
		std::regex rot(R"(rotate (left|right) ([[:digit:]]) steps?)");
		std::regex rot_pos(R"(rotate based on position of letter ([[:lower:]]))");
		std::regex rev(R"(reverse positions ([[:digit:]]) through ([[:digit:]]))");
		std::regex mov(R"(move position ([[:digit:]]) to position ([[:digit:]]))");

		for(std::string line; std::getline(fin, line);) {
			std::smatch m;
			if(std::regex_search(line, m, swap_pos)) {
				commands.push_back(std::make_unique<swap_position>(std::stoull(m[1].str()), std::stoull(m[2].str())));
			} else if(std::regex_search(line, m, swap_let)) {
				commands.push_back(std::make_unique<swap_letter>(m[1].str()[0], m[2].str()[0]));
			} else if(std::regex_search(line, m, rot)) {
				commands.push_back(std::make_unique<rotate>(m[1].str() == "left" ? rotate::left : rotate::right, std::stoll(m[2].str())));
			} else if(std::regex_search(line, m, rot_pos)) {
				commands.push_back(std::make_unique<rotate_position>(m[1].str()[0]));
			} else if(std::regex_search(line, m, rev)) {
				commands.push_back(std::make_unique<reverse>(std::stoll(m[1].str()), std::stoll(m[2].str())));
			} else if(std::regex_search(line, m, mov)) {
				commands.push_back(std::make_unique<move>(std::stoull(m[1].str()), std::stoull(m[2].str())));
			}
		}
	}

	std::string part_1() override {
		std::string password = "abcdefgh";
		for(const command_ptr& c : commands) {
			c->forward(password);
		}
		return password;
	}

	std::string part_2() override {
		std::string password = "fbgdceah";
		std::reverse(std::begin(commands), std::end(commands));
		for(const command_ptr& c : commands) {
			c->backward(password);
		}
		return password;
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_21>() {
	advent_2016_21 a;
	a.solve();
}
