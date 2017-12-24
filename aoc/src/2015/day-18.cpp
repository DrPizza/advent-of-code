#include "stdafx.h"

#include "problem.hpp"
#include "mdspan.hpp"

#include <fstream>
#include <string>
#include <memory>

struct advent_2015_18 : problem
{
	advent_2015_18() noexcept : problem(2015, 18) {
	}

protected:
	using grid_shape = md::extents<-1, -1>;
	using grid_type = md::mdspan<uint8_t, grid_shape>;

	std::unique_ptr<uint8_t[]> current;
	std::unique_ptr<uint8_t[]> previous;

	std::vector<std::string> raw_life;

	void prepare_input(std::ifstream& fin) override {
		for(std::string line; std::getline(fin, line);) {
			raw_life.push_back(line);
		}
	}

	void iterate(const grid_type& old, const grid_type& next) {
		for(std::ptrdiff_t r = 1; r < old.extent(0) - 1; ++r) {
			for(std::ptrdiff_t c = 1; c < old.extent(1) - 1; ++c) {
				const std::uint8_t neighbours = gsl::narrow<uint8_t>(old(r - 1, c - 1) + old(r - 1, c) + old(r - 1, c + 1)
				                                                   + old(r    , c - 1)                 + old(r    , c + 1)
				                                                   + old(r + 1, c - 1) + old(r + 1, c) + old(r + 1, c + 1));
				const std::uint8_t state = old(r, c);
				if(state == 0) {
					next(r, c) = (neighbours == 3) ? 1ui8 : 0ui8;
				} else {
					next(r, c) = (neighbours == 2 || neighbours == 3) ? 1ui8 : 0ui8;
				}
			}
		}
	}

	std::string part_1() override {
		const std::size_t row_count    = raw_life.size();
		const std::size_t column_count = raw_life[0].size();

		current  = std::make_unique<uint8_t[]>((row_count + 2) * (column_count + 2));
		previous = std::make_unique<uint8_t[]>((row_count + 2) * (column_count + 2));

		grid_type life(current.get(), row_count + 2, column_count + 2);
		grid_type old(previous.get(), row_count + 2, column_count + 2);

		for(std::size_t r = 0; r < row_count; ++r) {
			for(std::size_t c = 0; c < column_count; ++c) {
				old(r + 1, c + 1) = raw_life[r][c] == '#' ? 1ui8 : 0ui8;
			}
		}

		for(std::size_t i = 0; i < 100; ++i) {
			iterate(old, life);
			std::swap(old, life);
		}

		std::size_t lit = 0;
		for(std::ptrdiff_t r = 1; r < old.extent(0) - 1; ++r) {
			for(std::ptrdiff_t c = 1; c < old.extent(1) - 1; ++c) {
				lit += old(r, c);
			}
		}

		return std::to_string(lit);
	}

	std::string part_2() override {
		const std::size_t row_count = raw_life.size();
		const std::size_t column_count = raw_life[0].size();

		current = std::make_unique<uint8_t[]>((row_count + 2) * (column_count + 2));
		previous = std::make_unique<uint8_t[]>((row_count + 2) * (column_count + 2));

		grid_type life(current.get(), row_count + 2, column_count + 2);
		grid_type old(previous.get(), row_count + 2, column_count + 2);

		for(std::size_t r = 0; r < row_count; ++r) {
			for(std::size_t c = 0; c < column_count; ++c) {
				old(r + 1, c + 1) = raw_life[r][c] == '#' ? 1ui8 : 0ui8;
			}
		}

		for(std::size_t i = 0; i < 100; ++i) {
			old(1        , 1           ) = 1;
			old(1        , column_count) = 1;
			old(row_count, 1           ) = 1;
			old(row_count, column_count) = 1;

			iterate(old, life);
			std::swap(old, life);
		}

		old(1        , 1           ) = 1;
		old(1        , column_count) = 1;
		old(row_count, 1           ) = 1;
		old(row_count, column_count) = 1;

		std::size_t lit = 0;
		for(std::ptrdiff_t r = 1; r < old.extent(0) - 1; ++r) {
			for(std::ptrdiff_t c = 1; c < old.extent(1) - 1; ++c) {
				lit += old(r, c);
			}
		}

		return std::to_string(lit);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_18>() {
	advent_2015_18 a;
	a.solve();
}
