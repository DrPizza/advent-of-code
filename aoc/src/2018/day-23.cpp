#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <queue>

#include <range/v3/all.hpp>

namespace
{
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

	std::ptrdiff_t manhattan(const point_t& lhs, const point_t& rhs) noexcept {
		return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y) + std::abs(lhs.z - rhs.z);
	}

	struct bounding_box_t;

	struct nanobot_t
	{
		point_t pos;
		std::ptrdiff_t r;

		bool contains(const point_t& p) const noexcept {
			return manhattan(pos, p) <= r;
		}

		bool intersects(const bounding_box_t&) const noexcept;
	};

	bool covers(const nanobot_t& n, const point_t& p, const int64_t& padding)
	{
		return manhattan(n.pos, p) <= n.r + padding;
	}

	struct bounding_box_t
	{
		point_t bottom_left;
		point_t top_right;
		
		std::ptrdiff_t x_size() const noexcept {
			return top_right.x - bottom_left.x + 1;
		}

		std::ptrdiff_t y_size() const noexcept {
			return top_right.y - bottom_left.y + 1;
		}

		std::ptrdiff_t z_size() const noexcept {
			return top_right.z - bottom_left.z + 1;
		}

		bool contains(const point_t& p) const noexcept {
			return bottom_left.x <= p.x && p.x <= top_right.x
			    && bottom_left.y <= p.y && p.y <= top_right.y
			    && bottom_left.z <= p.z && p.z <= top_right.z;
		}

		bool intersects(const bounding_box_t& b) const noexcept {
			return   bottom_left.x <= b.top_right.x
			    && b.bottom_left.x <=   top_right.x
			    &&   bottom_left.y <= b.top_right.y
			    && b.bottom_left.y <=   top_right.y
			    &&   bottom_left.z <= b.top_right.z
			    && b.bottom_left.z <=   top_right.z;
		}
	};

	bool nanobot_t::intersects(const bounding_box_t& b) const noexcept {
		point_t pt{};
		pt.x = pos.x < b.bottom_left.x ? b.bottom_left.x
		     : b.top_right.x < pos.x   ? b.top_right.x
		     :                           pos.x;
		pt.y = pos.y < b.bottom_left.y ? b.bottom_left.y
		     : b.top_right.y < pos.y   ? b.top_right.y
		     :                           pos.y;
		pt.z = pos.z < b.bottom_left.y ? b.bottom_left.z
		     : b.top_right.z < pos.z   ? b.top_right.z
		     :                           pos.z;
		return contains(pt);
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
			return manhattan(n.pos, strongest.pos) <= strongest.r;
		});
		return std::to_string(number_in_range);
	}

	static std::size_t next_pow_2(std::size_t x) noexcept {
		unsigned long idx;
		_BitScanReverse64(&idx, x - 1ui64);
		return (1ui64 << (idx + 1ui64));
	}

	std::string part_3() {
		using visible_box_t = std::tuple<std::size_t, bounding_box_t>;
		const auto compare_by_visible = [] (const visible_box_t& lhs, const visible_box_t& rhs) {
			return std::get<0>(lhs) < std::get<0>(rhs);
		};

		std::priority_queue<visible_box_t, std::vector<visible_box_t>, decltype(compare_by_visible)> q(compare_by_visible);

		std::ptrdiff_t visible = 0i64;
		std::ptrdiff_t closest = 0i64;

		const auto split = [&] () {
			while(!q.empty()) {
				auto [count, box] = q.top();
				q.pop();
				
				if(count <= visible) {
					continue;
				}

				if(box.x_size() <= 1 && box.y_size() <= 1 && box.z_size() <= 1) {
					for(std::ptrdiff_t x = box.bottom_left.x; x <= box.top_right.x; ++x) {
						for(std::ptrdiff_t y = box.bottom_left.y; y <= box.top_right.y; ++y) {
							for(std::ptrdiff_t z = box.bottom_left.z; z <= box.top_right.z; ++z) {
								point_t pt = { x, y, z };
								const auto visible_here = ranges::count_if(nanobots, [pt] (const auto& n) {
									return n.contains(pt);
								});
								if(visible_here == visible
								&& manhattan(pt, { 0, 0, 0 }) < closest) {
									closest = manhattan(pt, { 0, 0, 0 });
								} else if(visible_here > visible) {
									visible = visible_here;
									closest = manhattan(pt, { 0, 0, 0 });
								}
							}
						}
					}
					continue;
				}
				if(box.x_size() > box.y_size() && box.x_size() > box.z_size()) {
					std::ptrdiff_t x_mid = box.bottom_left.x + (box.top_right.x - box.bottom_left.x) / 2;
					std::ptrdiff_t left = 0i64;
					std::ptrdiff_t right = 0i64;
					bounding_box_t left_box{ box };
					left_box.top_right.x = x_mid;
					bounding_box_t right_box{ box };
					right_box.bottom_left.x = x_mid + 1i64;
					for(const nanobot_t& n : nanobots) {
						if(n.intersects(left_box)) {
							++left;
						}
						if(n.intersects(right_box)) {
							++right;
						}
					}
					if(left > visible) {
						q.push(std::make_tuple(left, left_box));
					}
					if(right > visible) {
						q.push(std::make_tuple(right, right_box));
					}
				} else if(box.y_size() > box.z_size()) {
					std::ptrdiff_t y_mid = box.bottom_left.y + (box.top_right.y - box.bottom_left.y) / 2;
					std::ptrdiff_t left = 0i64;
					std::ptrdiff_t right = 0i64;
					bounding_box_t left_box{ box };
					left_box.top_right.y = y_mid;
					bounding_box_t right_box{ box };
					right_box.bottom_left.y = y_mid + 1i64;
					for(const nanobot_t& n : nanobots) {
						if(n.intersects(left_box)) {
							++left;
						}
						if(n.intersects(right_box)) {
							++right;
						}
					}
					if(left > visible) {
						q.push(std::make_tuple(left, left_box));
					}
					if(right > visible) {
						q.push(std::make_tuple(right, right_box));
					}
				} else {
					std::ptrdiff_t z_mid = box.bottom_left.z + (box.top_right.z - box.bottom_left.z) / 2;
					std::ptrdiff_t left = 0i64;
					std::ptrdiff_t right = 0i64;
					bounding_box_t left_box{ box };
					left_box.top_right.z = z_mid;
					bounding_box_t right_box{ box };
					right_box.bottom_left.z = z_mid + 1i64;
					for(const nanobot_t& n : nanobots) {
						if(n.intersects(left_box)) {
							++left;
						}
						if(n.intersects(right_box)) {
							++right;
						}
					}
					if(left > visible) {
						q.push(std::make_tuple(left, left_box));
					}
					if(right > visible) {
						q.push(std::make_tuple(right, right_box));
					}
				}
			}
		};

		std::ptrdiff_t x_min = std::numeric_limits<std::ptrdiff_t>::max();
		std::ptrdiff_t x_max = std::numeric_limits<std::ptrdiff_t>::min();
		std::ptrdiff_t y_min = std::numeric_limits<std::ptrdiff_t>::max();
		std::ptrdiff_t y_max = std::numeric_limits<std::ptrdiff_t>::min();
		std::ptrdiff_t z_min = std::numeric_limits<std::ptrdiff_t>::max();
		std::ptrdiff_t z_max = std::numeric_limits<std::ptrdiff_t>::min();
		for(const nanobot_t& n : nanobots) {
			x_min = std::min(x_min, n.pos.x);
			x_max = std::max(x_max, n.pos.x);
			y_min = std::min(y_min, n.pos.y);
			y_max = std::max(y_max, n.pos.y);
			z_min = std::min(z_min, n.pos.z);
			z_max = std::max(z_max, n.pos.z);
		}

		q.push(std::make_tuple(nanobots.size(), bounding_box_t{ {x_min, y_min, z_min}, {x_max, y_max, z_max} }));
		split();

		return std::to_string(closest);
	}

	std::string part_2() override {
		std::ptrdiff_t x_min = std::numeric_limits<std::ptrdiff_t>::max();
		std::ptrdiff_t x_max = std::numeric_limits<std::ptrdiff_t>::min();
		std::ptrdiff_t y_min = std::numeric_limits<std::ptrdiff_t>::max();
		std::ptrdiff_t y_max = std::numeric_limits<std::ptrdiff_t>::min();
		std::ptrdiff_t z_min = std::numeric_limits<std::ptrdiff_t>::max();
		std::ptrdiff_t z_max = std::numeric_limits<std::ptrdiff_t>::min();
		for(const nanobot_t& n : nanobots) {
			x_min = std::min(x_min, n.pos.x - n.r);
			x_max = std::max(x_max, n.pos.x + n.r + 1);
			y_min = std::min(y_min, n.pos.y - n.r);
			y_max = std::max(y_max, n.pos.y + n.r + 1);
			z_min = std::min(z_min, n.pos.z - n.r);
			z_max = std::max(z_max, n.pos.z + n.r + 1);
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
					if(manhattan(point, { 0, 0, 0 }) < manhattan({ x_best, y_best, z_best}, { 0, 0, 0 })) {
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
		return std::to_string(manhattan({x_best, y_best, z_best}, {0, 0, 0}));
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_23>() {
	advent_2018_23 a;
	a.solve();
}
