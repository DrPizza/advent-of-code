#include "stdafx.h"

#include "problem.hpp"
#include "utility.hpp"

#include <fstream>
#include <iomanip>
#include <regex>
#include <utility>
#include <numeric>
#include <valarray>

struct advent_2017_20 : problem
{
	advent_2017_20() noexcept : problem(2017, 20) {
	}

	struct particle
	{
		std::valarray<std::ptrdiff_t> position;
		std::valarray<std::ptrdiff_t> velocity;
		std::valarray<std::ptrdiff_t> acceleration;
	};

	std::vector<std::string> raw_particles;
	std::vector<particle> particles;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line); ) {
			raw_particles.push_back(line);
		}
	}

	void precompute() override {
		std::regex pattern(R"(p=<(-?[[:digit:]]+),(-?[[:digit:]]+),(-?[[:digit:]]+)>, v=<(-?[[:digit:]]+),(-?[[:digit:]]+),(-?[[:digit:]]+)>, a=<(-?[[:digit:]]+),(-?[[:digit:]]+),(-?[[:digit:]]+)>)");
		for(const std::string& line : raw_particles) {
			std::smatch m;
			std::regex_search(line, m, pattern);
			particles.push_back(particle{
				{ std::stoll(m[1].str()), std::stoll(m[2].str()), std::stoll(m[3].str()) },
				{ std::stoll(m[4].str()), std::stoll(m[5].str()), std::stoll(m[6].str()) },
				{ std::stoll(m[7].str()), std::stoll(m[8].str()), std::stoll(m[9].str()) }
			});
		}
	}

	void update_particles(std::vector<particle>& ps) {
		for(particle& p : ps) {
			p.velocity += p.acceleration;
			p.position += p.velocity;
		}
	}

	std::string part_1() override {
		auto it = std::min_element(std::begin(particles), std::end(particles), [](const particle& lhs, const particle& rhs) {
			return std::abs(lhs.acceleration).sum() != std::abs(rhs.acceleration).sum() ? std::abs(lhs.acceleration).sum() < std::abs(rhs.acceleration).sum()
			     : std::abs(lhs.velocity).sum()     != std::abs(rhs.velocity).sum()     ? std::abs(lhs.velocity).sum()     < std::abs(rhs.velocity).sum()
			     :                                                                        std::abs(lhs.position).sum()     < std::abs(rhs.position).sum();
		});
		const std::ptrdiff_t particle_number = std::distance(std::begin(particles), it);
		return std::to_string(particle_number);
	}

	void remove_collisions(std::vector<particle>& ps) {
		const auto lt_position = [](const particle& lhs, const particle& rhs) {
			return lhs.position[0] != rhs.position[0] ? lhs.position[0] < rhs.position[0]
			     : lhs.position[1] != rhs.position[1] ? lhs.position[1] < rhs.position[1]
			     :                                      lhs.position[2] < rhs.position[2];
		};
		const auto equal_position = [](const particle& lhs, const particle& rhs) {
			return (lhs.position == rhs.position).min();
		};

		std::sort(std::begin(ps), std::end(ps), lt_position);

		for(auto it = std::adjacent_find(std::begin(ps), std::end(ps), equal_position);
		    it != std::end(ps);
		    it = std::adjacent_find(std::begin(ps), std::end(ps), equal_position)
		) {
			auto end = it + 2;
			while(equal_position(*it, *end)) {
				++end;
			}
			ps.erase(it, end);
		}
	}

	std::string part_2() override {
		std::vector<particle> my_particles(particles);
		for(std::size_t i = 0; i < 100; ++i) {
			remove_collisions(my_particles);
			update_particles(my_particles);
		}
		return std::to_string(my_particles.size());
	}
};

REGISTER_SOLVER(2017, 20);
