#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <utility>
#include <map>

struct advent_2015_14 : problem
{
	advent_2015_14() noexcept : problem(2015, 14) {
	}

protected:
	struct reindeer
	{
		std::string name;
		std::size_t speed;
		std::size_t time;
		std::size_t rest;
	};

	std::vector<reindeer> reindeers;

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(([[:alpha:]]+) can fly ([[:digit:]]+) km/s for ([[:digit:]]+) seconds, but then must rest for ([[:digit:]]+) seconds.)");
		for(std::string line; std::getline(fin, line); ) {
			std::smatch m;
			std::regex_search(line, m, pattern);
			reindeers.push_back(reindeer{ m[1].str(), std::stoull(m[2].str()), std::stoull(m[3].str()), std::stoull(m[4].str()) });
		}
	}

	std::size_t fly_distance(const reindeer& r, const std::size_t total_time) noexcept {
		const std::size_t cycle_time = r.time + r.rest;
		const std::size_t cycle_distance = r.speed * r.time;
		const std::size_t whole_cycles = total_time / cycle_time;
		const std::size_t remaining_time = total_time % cycle_time;
		const std::size_t extra_distance = std::min(r.time, remaining_time) * r.speed;
		return (whole_cycles * cycle_distance) + extra_distance;
	}

	std::string part_1() override {
		const std::size_t total_time = 2503;
		std::size_t furthest = 0;
		for(const reindeer& r : reindeers) {
			const std::size_t distance = fly_distance(r, total_time);
			furthest = std::max(distance, furthest);
		}
		return std::to_string(furthest);
	}

	std::string part_2() override {
		std::map<std::string, std::size_t> scoreboard;
		for(std::size_t i = 1; i < 2503; ++i) {
			std::multimap<std::size_t, std::string, std::greater<std::size_t>> per_second_scoreboard;
			for(const reindeer& r : reindeers) {
				per_second_scoreboard.insert(std::make_pair(fly_distance(r, i), r.name));
			}
			const auto winners = per_second_scoreboard.equal_range(per_second_scoreboard.begin()->first);
			for(auto j = winners.first; j != winners.second; ++j) {
				++scoreboard[j->second];
			}
		}
		std::size_t most_points = 0;
		for(const auto& p : scoreboard) {
			most_points = std::max(p.second, most_points);
		}
		return std::to_string(most_points);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_14>() {
	advent_2015_14 a;
	a.solve();
}
