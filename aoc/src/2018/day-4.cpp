#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <map>
#include <array>

#include <range/v3/all.hpp>

struct advent_2018_4 : problem
{
	advent_2018_4() noexcept : problem(2018, 4) {
	}

protected:
	std::map<std::uintmax_t, std::array<std::uintmax_t, 60>> sleep_schedule;

	void prepare_input(std::ifstream& fin) override {
		std::vector<std::string> raw_lines;
		for(std::string line; std::getline(fin, line);) {
			raw_lines.push_back(line);
		}
		std::sort(std::begin(raw_lines), std::end(raw_lines));

		std::regex pick_guard_pattern  (R"(.*Guard #([[:digit:]]+) begins shift)");
		std::regex falls_asleep_pattern(R"(.*:([[:digit:]]{2})\] falls asleep)");
		std::regex wakes_up_pattern    (R"(.*:([[:digit:]]{2})\] wakes up)");

		std::uintmax_t chosen_guard = 0xffff'ffff'ffff'ffff;
		std::uintmax_t sleep_time   = 0xffff'ffff'ffff'ffff;
		std::uintmax_t wake_time    = 0xffff'ffff'ffff'ffff;

		for(const std::string& line : raw_lines) {
			std::smatch m;
			if(std::regex_match(line, m, pick_guard_pattern)) {
				chosen_guard = std::stoull(m[1]);
			} else if(std::regex_match(line, m, falls_asleep_pattern)) {
				sleep_time = std::stoull(m[1]);
			} else if(std::regex_match(line, m, wakes_up_pattern)) {
				wake_time = std::stoull(m[1]);
				for(auto& minute : sleep_schedule[chosen_guard] | ranges::view::slice(sleep_time, wake_time)) {
					++minute;
				}
			}
		}
	}

	std::string part_1() override {
		const auto find_most_slept_minute = [] (const auto& schedule) {
			return gsl::narrow_cast<std::size_t>(ranges::distance(ranges::begin(schedule), ranges::max_element(schedule)));
		};
		const auto sleepiest = ranges::max_element(sleep_schedule, std::less<>{}, [] (const auto& p) {
			return ranges::accumulate(p.second, 0ui64);
		});
		const std::size_t most_slept_minute = find_most_slept_minute(sleepiest->second);
		return std::to_string(sleepiest->first * most_slept_minute);
	}

	std::string part_2() override {
		const auto find_most_slept_minute = [] (const auto& schedule) {
			return gsl::narrow_cast<std::size_t>(ranges::distance(ranges::begin(schedule), ranges::max_element(schedule)));
		};
		const auto worst_minute = ranges::max_element(sleep_schedule, std::less<>{}, [] (const auto& p) {
			return ranges::max(p.second);
		});
		const std::size_t most_slept_minute = find_most_slept_minute(worst_minute->second);
		return std::to_string(worst_minute->first * most_slept_minute);
	}
};

REGISTER_SOLVER(2018, 4);
