#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>

#include <range/v3/all.hpp>

namespace
{
	struct nanobot_t
	{
		std::ptrdiff_t x;
		std::ptrdiff_t y;
		std::ptrdiff_t z;
		std::ptrdiff_t r;
	};

	std::ptrdiff_t manhattan(const nanobot_t& lhs, const nanobot_t& rhs) noexcept {
		return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y) + std::abs(lhs.z - rhs.z);
	}

	bool covers(const nanobot_t& n, const std::ptrdiff_t& x, const std::ptrdiff_t& y,
		const std::ptrdiff_t& z, const std::ptrdiff_t& padding)
	{
		return (std::abs(x - n.x)
		      + std::abs(y - n.y)
		      + std::abs(z - n.z) <= n.r + padding);
	}

	struct point_t
	{
		std::ptrdiff_t x;
		std::ptrdiff_t y;
		std::ptrdiff_t z;

		friend bool operator<(const point_t& lhs, const point_t& rhs) noexcept
		{
			return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
		}

		friend bool operator==(const point_t& lhs, const point_t& rhs) noexcept
		{
			return std::tie(lhs.x, lhs.y, lhs.z) == std::tie(rhs.x, rhs.y, rhs.z);
		}
	};

	bool covers(const nanobot_t& n, const point_t& p, const int64_t& padding)
	{
		return covers(n, p.x, p.y, p.z, padding);
	}
}

struct advent_2018_23 : problem
{
	advent_2018_23() noexcept : problem(2018, 23) {
	}

protected:
	std::vector<nanobot_t> nanobots;

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"(pos=<(-?[[:digit:]]+),(-?[[:digit:]]+),(-?[[:digit:]]+)>, r=(-?[[:digit:]]+))");
		std::smatch m;
		for(std::string line; std::getline(fin, line); ) {
			if(std::regex_match(line, m, pattern)) {
				nanobots.push_back({ std::stoll(m[1]), std::stoll(m[2]), std::stoll(m[3]), std::stoll(m[4]) });
			}
		}
	}

	void precompute() override {
		std::sort(std::begin(nanobots), std::end(nanobots), [](const nanobot_t& lhs, const nanobot_t& rhs) {
			return lhs.r > rhs.r;
		});
	}

	std::string part_1() override {
		const nanobot_t& strongest = nanobots.front();
		const std::size_t number_in_range = ranges::count_if(nanobots, [&strongest] (const nanobot_t& n) {
			return manhattan(n, strongest) <= strongest.r;
		});
		return std::to_string(number_in_range);
	}

	static std::size_t next_pow_2(std::size_t x) noexcept {
		unsigned long idx;
		_BitScanReverse64(&idx, x - 1ui64);
		return (1ui64 << (idx + 1ui64));
	}

	std::string part_2() override {
		std::ptrdiff_t x_min = std::numeric_limits<std::ptrdiff_t>::max();
		std::ptrdiff_t x_max = std::numeric_limits<std::ptrdiff_t>::min();
		std::ptrdiff_t y_min = std::numeric_limits<std::ptrdiff_t>::max();
		std::ptrdiff_t y_max = std::numeric_limits<std::ptrdiff_t>::min();
		std::ptrdiff_t z_min = std::numeric_limits<std::ptrdiff_t>::max();
		std::ptrdiff_t z_max = std::numeric_limits<std::ptrdiff_t>::min();
		for(const nanobot_t& n : nanobots) {
			x_min = std::min(x_min, n.x - n.r);
			x_max = std::max(x_max, n.x + n.r + 1);
			y_min = std::min(y_min, n.y - n.r);
			y_max = std::max(y_max, n.y + n.r + 1);
			z_min = std::min(z_min, n.z - n.r);
			z_max = std::max(z_max, n.z + n.r + 1);
		}
		std::ptrdiff_t scale(std::ptrdiff_t(1) << std::ptrdiff_t(std::log2(std::abs(x_max - x_min) + std::abs(y_max - y_min) + std::abs(z_max - z_min)) + 1));
		x_min = (x_min / scale) * scale;
		x_max = (x_max / scale + 1) * scale;
		y_min = (y_min / scale) * scale;
		y_max = (y_max / scale + 1) * scale;
		z_min = (z_min / scale) * scale;
		z_max = (z_max / scale + 1) * scale;

		const std::ptrdiff_t nx = (x_max - x_min) / scale;
		const std::ptrdiff_t ny = (y_max - y_min) / scale;
		const std::ptrdiff_t nz = (z_max - z_min) / scale;
		std::vector<point_t> points;
		for(std::ptrdiff_t dx = 0; dx < nx; ++dx) {
			for(std::ptrdiff_t dy = 0; dy < ny; ++dy) {
				for(std::ptrdiff_t dz = 0; dz < nz; ++dz) {
					points.push_back({ x_min + dx * scale, y_min + dy * scale, z_min + dz * scale });
				}
			}
		}

		std::ptrdiff_t x_best = 0i64, y_best = 0i64, z_best = 0i64;
		for(;;) {
			std::size_t max_bots = 0;
			std::vector<point_t> new_points;
			for(auto& point : points) {
				const std::size_t num_bots = std::count_if(std::begin(nanobots), std::end(nanobots), [&] (const nanobot_t& n) {
					return covers(n, point, scale);
				});

				if(num_bots > max_bots) {
					max_bots = num_bots;
					new_points.clear();
					new_points.emplace_back(point);
					x_best = point.x;
					y_best = point.y;
					z_best = point.z;
				} else if(num_bots != 0 && num_bots == max_bots) {
					new_points.emplace_back(point);
					if(manhattan({ point.x, point.y, point.z, 0 }, { 0, 0, 0, 0 }) < manhattan({ x_best, y_best, z_best, 0 }, { 0, 0, 0, 0 })) {
						x_best = point.x;
						y_best = point.y;
						z_best = point.z;
					}
				}
			}

			if(scale == 0) {
				std::swap(points, new_points);
				break;
			}
			points.clear();
			scale /= 2;
			if(scale == 0) {
				std::swap(points, new_points);
			} else {
				for(auto& point : new_points) {
					for(std::ptrdiff_t dx = -scale; dx <= scale; dx += scale) {
						for(std::ptrdiff_t dy = -scale; dy <= scale; dy += scale) {
							for(std::ptrdiff_t dz = -scale; dz <= scale; dz += scale) {
								points.push_back({ point.x + dx, point.y + dy, point.z + dz });
							}
						}
					}
				}
			}
		}
		return std::to_string(manhattan({ x_best, y_best, z_best, 0 }, { 0, 0, 0, 0 }));
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_23>() {
	advent_2018_23 a;
	a.solve();
}
