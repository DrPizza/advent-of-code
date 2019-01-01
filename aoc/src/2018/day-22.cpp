#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <queue>

namespace
{
	struct point
	{
		std::size_t c;
		std::size_t r;

		point& operator+=(const point& rhs) noexcept {
			c += rhs.c;
			r += rhs.r;
			return *this;
		}
		point operator+(const point& rhs) const noexcept {
			return { c + rhs.c, r + rhs.r };
		}
		bool operator==(const point& rhs) const noexcept {
			return std::tie(r, c) == std::tie(rhs.r, rhs.c);
		}
		bool operator!=(const point& rhs) const noexcept {
			return !(*this == rhs);
		}
		bool operator<(const point& rhs) const noexcept {
			return std::tie(r, c) < std::tie(rhs.r, rhs.c);
		}
		bool operator>(const point& rhs) const noexcept {
			return std::tie(r, c) > std::tie(rhs.r, rhs.c);
		}
	};

	std::size_t manhattan(const point& lhs, const point& rhs) noexcept {
		return std::abs(gsl::narrow_cast<std::ptrdiff_t>(lhs.r) - gsl::narrow_cast<std::ptrdiff_t>(rhs.r))
		     + std::abs(gsl::narrow_cast<std::ptrdiff_t>(lhs.c) - gsl::narrow_cast<std::ptrdiff_t>(rhs.c));
	}

	enum struct region_type_t
	{
		rocky,
		wet,
		narrow
	};

	using erosion_level_t = std::size_t;

	region_type_t get_region_type(erosion_level_t ero) noexcept {
		switch(ero % 3ui64) {
		case 0:
			return region_type_t::rocky;
		case 1:
			return region_type_t::wet;
		case 2:
			return region_type_t::narrow;
		}
	}

	std::size_t get_risk(region_type_t type) noexcept {
		switch(type) {
		case region_type_t::rocky:
			return 0ui64;
		case region_type_t::wet:
			return 1ui64;
		case region_type_t::narrow:
			return 2ui64;
		}
	}

	struct state_t
	{
		erosion_level_t level;
		region_type_t type;
	};

	using erosion_map_t = std::map<std::size_t, std::map<std::size_t, state_t>>;

	enum struct tool_t
	{
		neither,
		torch,
		gear
	};

	bool can_enter(region_type_t type, tool_t tool) noexcept {
		switch(type) {
		case region_type_t::rocky:
			return tool != tool_t::neither;
		case region_type_t::wet:
			return tool != tool_t::torch;
		case region_type_t::narrow:
			return tool != tool_t::gear;
		}
	}
}

struct advent_2018_22 : problem
{
	advent_2018_22() noexcept : problem(2018, 22) {
	}

protected:
	std::size_t depth;
	point target;

public:
	erosion_map_t erosion_map;

protected:
	state_t get_erosion_state(point pt) {
		if(erosion_map.count(pt.r) && erosion_map.at(pt.r).count(pt.c)) {
			return erosion_map.at(pt.r).at(pt.c);
		}

		std::size_t geo_idx = 0ui64;
		if(pt.r == 0ui64 && pt.c == 0ui64) {
			geo_idx = 0ui64;
		} else if(pt == target) {
			geo_idx = 0ui64;
		} else if(pt.r == 0ui64) {
			geo_idx = pt.c * 16807ui64;
		} else if(pt.c == 0ui64) {
			geo_idx = pt.r * 48271ui64;
		} else {
			const state_t up   = get_erosion_state({ pt.c - 1, pt.r });
			const state_t left = get_erosion_state({ pt.c    , pt.r - 1 });
			geo_idx = up.level * left.level;
		}

		state_t state = {};
		state.level = (geo_idx + depth) % 20183ui64;
		state.type = get_region_type(state.level);
		erosion_map[pt.r][pt.c] = state;
		return state;
	}

	void prepare_input(std::ifstream& fin) override {
		const std::regex depth_pattern(R"(depth: ([[:digit:]]+))");
		const std::regex target_pattern(R"(target: ([[:digit:]]+),([[:digit:]]+))");

		for(std::string line; std::getline(fin, line); ) {
			std::smatch m;
			if(std::regex_match(line, m, depth_pattern)) {
				depth = std::stoull(m[1]);
			} else if(std::regex_match(line, m, target_pattern)) {
				target.c = std::stoull(m[1]);
				target.r = std::stoull(m[2]);
			}
		}
	}

	std::string part_1() override {
		std::size_t risk = 0ui64;
		for(std::size_t y = 0; y <= target.r; ++y) {
			for(std::size_t x = 0; x <= target.c; ++x) {
				const state_t state = get_erosion_state({ x, y });
				risk += get_risk(state.type);
			}
		}
		return std::to_string(risk);
	}

	std::size_t a_star(const point& initial, tool_t equipped, const point& destination, tool_t required) {
		struct visit_state_t
		{
			point pos;
			tool_t tool;

			bool operator<(const visit_state_t& rhs) const noexcept {
				return std::tie(pos, tool) < std::tie(rhs.pos, rhs.tool);
			}

			bool operator>(const visit_state_t& rhs) const noexcept {
				return std::tie(pos, tool) > std::tie(rhs.pos, rhs.tool);
			}

			bool operator==(const visit_state_t& rhs) const noexcept {
				return std::tie(pos, tool) == std::tie(rhs.pos, rhs.tool);
			}
		};

		struct hash_point
		{
			inline std::size_t operator()(const point& p) const {
				return std::hash<std::uint32_t>()((std::uint32_t(p.r) << 16) + p.c);
			}
		};

		struct hash_visit_state
		{
			inline std::size_t operator()(const visit_state_t& s) const
			{
				return hash_point()(s.pos) ^ std::hash<std::uint8_t>()(uint8_t(s.tool));
			}
		};

		static const std::size_t move_cost = 1ui64;
		static const std::size_t change_tool_cost = 7ui64;

		// it is important to note that the heuristic is exact for adjacent squares,
		// because we also use it for updating the tentative g score (below) which uses
		// an exact cost, not just a heuristic.
		const auto heuristic = [] (const visit_state_t& lhs, const visit_state_t& rhs) {
			return (move_cost * manhattan(lhs.pos, rhs.pos))
			     + (change_tool_cost * (lhs.tool != rhs.tool));
		};

		const visit_state_t init = { initial, equipped };
		const visit_state_t goal = { destination, required };

		const size_t reservation = 16 * (destination.r + 1) * (destination.c + 1);
		
		std::unordered_map<visit_state_t, std::size_t, hash_visit_state> g_score;
		g_score.reserve(reservation);
		g_score[init] = 0ui64;
		
		std::unordered_map<visit_state_t, std::size_t, hash_visit_state> f_score;
		f_score.reserve(reservation);
		f_score[init] = heuristic(init, goal);

		const auto rank_by_f_score = [&f_score] (const visit_state_t& lhs, const visit_state_t& rhs) {
			const std::size_t lhs_score = f_score[lhs];
			const std::size_t rhs_score = f_score[rhs];
			if(lhs_score > rhs_score) {
				return true;
			}
			if(lhs_score < rhs_score) {
				return false;
			}
			return lhs > rhs;
		};

		std::vector<visit_state_t> open_queue;
		open_queue.push_back(init);
		std::make_heap(std::begin(open_queue), std::end(open_queue), rank_by_f_score);
		std::unordered_set<visit_state_t, hash_visit_state> closed_set;

		while(!open_queue.empty()) {
			std::pop_heap(std::begin(open_queue), std::end(open_queue), rank_by_f_score);
			visit_state_t current = open_queue.back();
			open_queue.pop_back();

			closed_set.insert(current);

			if(current.pos == destination
			&& current.tool == required) {
				break;
			}

			std::vector<visit_state_t> neighbours;
			const auto add_if_legal = [&neighbours, this] (visit_state_t v) {
				const state_t state = get_erosion_state(v.pos);
				if(can_enter(state.type, v.tool)) {
					neighbours.push_back(v);
				}
			};

			if(current.pos.r > 0) {
				add_if_legal({ { current.pos.c, current.pos.r - 1 }, current.tool });
			}
			if(current.pos.c > 0) {
				add_if_legal({ { current.pos.c - 1, current.pos.r }, current.tool });
			}
			add_if_legal({ { current.pos.c + 1, current.pos.r }, current.tool });
			add_if_legal({ { current.pos.c, current.pos.r + 1 }, current.tool });

			const state_t state = get_erosion_state(current.pos);
			switch(current.tool) {
			case tool_t::neither:
				add_if_legal({ { current.pos.c, current.pos.r }, tool_t::torch });
				add_if_legal({ { current.pos.c, current.pos.r }, tool_t::gear  });
				break;
			case tool_t::torch:
				add_if_legal({ { current.pos.c, current.pos.r }, tool_t::neither });
				add_if_legal({ { current.pos.c, current.pos.r }, tool_t::gear    });
				break;
			case tool_t::gear:
				add_if_legal({ { current.pos.c, current.pos.r }, tool_t::torch   });
				add_if_legal({ { current.pos.c, current.pos.r }, tool_t::neither });
				break;
			}

			const std::size_t current_g_score = g_score[current];
			for(const visit_state_t& neighbour : neighbours) {
				if(closed_set.count(neighbour)) {
					continue;
				}
				const std::size_t tentative_g_score = current_g_score + heuristic(current, neighbour);
				if(std::find(std::begin(open_queue), std::end(open_queue), neighbour) == std::end(open_queue)) {
					open_queue.push_back(neighbour);
					// we don't care about breaking the heap here... 
					//std::push_heap(std::begin(open_queue), std::end(open_queue), rank_by_f_score);
				} else if(tentative_g_score >= g_score[neighbour]) {
					continue;
				}

				g_score[neighbour] = tentative_g_score;
				// ... because this potentially breaks the heap property (by changing the sort criterion)
				f_score[neighbour] = tentative_g_score + heuristic(neighbour, goal);
			}
			// so we make sure to re-establish it here
			std::make_heap(std::begin(open_queue), std::end(open_queue), rank_by_f_score);
		}
		return g_score[goal];
	}

	std::size_t dijkstra(const point& initial, tool_t equipped, const point& destination, tool_t required) {
		struct visit_state_t
		{
			point pos;
			tool_t tool;
			std::size_t lowest_score;

			bool operator<(const visit_state_t& rhs) const noexcept {
				return std::tie(pos, tool) < std::tie(rhs.pos, rhs.tool);
			}

			bool operator>(const visit_state_t& rhs) const noexcept {
				return std::tie(pos, tool) > std::tie(rhs.pos, rhs.tool);
			}

			bool operator==(const visit_state_t& rhs) const noexcept {
				return std::tie(pos, tool) == std::tie(rhs.pos, rhs.tool);
			}

			bool operator!=(const visit_state_t& rhs) const noexcept {
				return std::tie(pos, tool) != std::tie(rhs.pos, rhs.tool);
			}

		};
		struct visit_state_t_score_comparator
		{
			bool operator()(const visit_state_t& lhs, const visit_state_t& rhs) const noexcept {
				return lhs.lowest_score < rhs.lowest_score;
			}
		};
		struct hash_point
		{
			inline std::size_t operator()(const point& p) const {
				return std::hash<std::uint32_t>()((std::uint32_t(p.r) << 16) + p.c);
			}
		};

		struct hash_visit_state
		{
			inline std::size_t operator()(const visit_state_t& s) const
			{
				return hash_point()(s.pos) ^ std::hash<std::uint8_t>()(uint8_t(s.tool));
			}
		};

		const visit_state_t init = { initial, equipped, 0ui64 };
		const visit_state_t goal = { destination, required, 0ui64 };

		std::unordered_map<visit_state_t, std::size_t, hash_visit_state> dist;
		dist[init] = 0ui64;

		std::multiset<visit_state_t, visit_state_t_score_comparator> candidates;
		candidates.insert(init);
		while(!candidates.empty()) {
			auto it = candidates.begin();
			visit_state_t u = *it;
			candidates.erase(it);

			if(u == goal) {
				return u.lowest_score;
			}

			std::vector<visit_state_t> neighbours;
			const auto add_if_legal = [&neighbours, this] (visit_state_t v) {
				const state_t state = get_erosion_state(v.pos);
				if(can_enter(state.type, v.tool)) {
					neighbours.push_back(v);
				}
			};

			if(u.pos != destination) {
				if(u.pos.c > 0) {
					add_if_legal({ { u.pos.c - 1, u.pos.r }, u.tool, u.lowest_score + 1ui64 });
				}
				if(u.pos.r > 0) {
					add_if_legal({ { u.pos.c, u.pos.r - 1 }, u.tool, u.lowest_score + 1ui64 });
				}
				add_if_legal({ { u.pos.c + 1, u.pos.r }, u.tool, u.lowest_score + 1ui64 });
				add_if_legal({ { u.pos.c, u.pos.r + 1 }, u.tool, u.lowest_score + 1ui64 });
			}

			const state_t state = get_erosion_state(u.pos);
			switch(u.tool) {
			case tool_t::neither:
				add_if_legal({ { u.pos.c, u.pos.r }, tool_t::torch  , u.lowest_score + 7ui64 });
				add_if_legal({ { u.pos.c, u.pos.r }, tool_t::gear   , u.lowest_score + 7ui64 });
				break;
			case tool_t::torch:
				add_if_legal({ { u.pos.c, u.pos.r }, tool_t::neither, u.lowest_score + 7ui64 });
				add_if_legal({ { u.pos.c, u.pos.r }, tool_t::gear   , u.lowest_score + 7ui64 });
				break;
			case tool_t::gear:
				add_if_legal({ { u.pos.c, u.pos.r }, tool_t::torch  , u.lowest_score + 7ui64 });
				add_if_legal({ { u.pos.c, u.pos.r }, tool_t::neither, u.lowest_score + 7ui64 });
				break;
			}

			for(const visit_state_t& v : neighbours) {
				const auto alt = v.lowest_score;

				if(dist.count(v) == 0u
				|| alt < dist[v]) {
					dist[v] = alt;

					auto v_it = std::find_if(std::begin(candidates), std::end(candidates), [=] (const visit_state_t& vis) {
						return vis.pos == v.pos && vis.tool == v.tool;
					});
					if(v_it == std::end(candidates)) {
						candidates.insert(v);
					} else {
						visit_state_t vv = *v_it;
						vv.lowest_score = alt;
						candidates.erase(v_it);
						candidates.insert(vv);
					}

				}
			}
		}
		__assume(0);
	}

	std::string part_2() override {
		//1013
		//const std::size_t rescue_time = dijkstra({ 0, 0 }, tool_t::torch, target, tool_t::torch);
		const std::size_t rescue_time = a_star({ 0, 0 }, tool_t::torch, target, tool_t::torch);
		return std::to_string(rescue_time);
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_22>() {
	advent_2018_22 a;
	a.solve();
}
