#include "stdafx.h"

#include "problem.hpp"
#include "mdspan.hpp"

#include <fstream>
#include <regex>

struct advent_2015_6 : problem
{
	advent_2015_6() noexcept : problem(2015, 6) {
	}

protected:
	using grid_shape = md::extents<1'000, 1'000>;
	using grid_type = md::mdspan<uint32_t, grid_shape>;

	enum struct action
	{
		off,
		toggle,
		on,
	};

	struct coord
	{
		std::ptrdiff_t x, y;
	};

	struct command
	{
		action op;
		coord start;
		coord end;
	};

	std::vector<command> commands;

	void prepare_input(std::ifstream& fin) override {
		std::regex pattern(R"((turn (on|off)|toggle) ([[:digit:]]+),([[:digit:]]+) through ([[:digit:]]+),([[:digit:]]+))");
		for(std::string line; std::getline(fin, line); ) {
			std::smatch m;
			std::regex_search(line, m, pattern);
			action a = action::off;
			if(m[1].str() == "toggle") {
				a = action::toggle;
			} else if(m[2].str() == "on") {
				a = action::on;
			} else if(m[2].str() == "off") {
				a = action::off;
			}
			commands.push_back(command{ a, { std::stoll(m[3]), std::stoll(m[4]) }, { std::stoll(m[5]), std::stoll(m[6]) } });
		}
	}

	std::string part_1() override {
		std::unique_ptr<uint32_t[]> data{ std::make_unique<uint32_t[]>(1'000 * 1'000) };
		grid_type lights{ data.get() };
		gsl::span<uint32_t, 1'000 * 1'000> light_span{ data.get(), 1'000 * 1'000 };

		for(const command& c : commands) {
			for(std::ptrdiff_t x = c.start.x; x <= c.end.x; ++x) {
				for(std::ptrdiff_t y = c.start.y; y <= c.end.y; ++y) {
					switch(c.op) {
					case action::off:
						lights(x, y) = 0;
						break;
					case action::on:
						lights(x, y) = 1;
						break;
					case action::toggle:
						lights(x, y) = gsl::narrow<uint32_t>(1 - lights(x, y));
						break;
					}
				}
			}
		}
		const std::ptrdiff_t number_on = std::count(std::begin(light_span), std::end(light_span), 1ui8);
		return std::to_string(number_on);
	}

	std::string part_2() override {
		std::unique_ptr<uint32_t[]> data{ std::make_unique<uint32_t[]>(1'000 * 1'000) };
		grid_type lights{ data.get() };
		gsl::span<uint32_t, 1'000 * 1'000> light_span{ data.get(), 1'000 * 1'000 };

		for(const command& c : commands) {
			for(std::ptrdiff_t x = c.start.x; x <= c.end.x; ++x) {
				for(std::ptrdiff_t y = c.start.y; y <= c.end.y; ++y) {
					uint32_t current_value = lights(x, y);
					switch(c.op) {
					case action::off:
						lights(x, y) = current_value > 0 ? current_value - 1 : 0;
						break;
					case action::on:
						lights(x, y) = current_value + 1;
						break;
					case action::toggle:
						lights(x, y) = current_value + 2;
						break;
					}
				}
			}
		}
		const std::size_t total_brightness = std::accumulate(std::begin(light_span), std::end(light_span), 0ui64);
		return std::to_string(total_brightness);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_6>() {
	advent_2015_6 a;
	a.solve();
}
