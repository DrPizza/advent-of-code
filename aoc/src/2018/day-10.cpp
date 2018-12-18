#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <vector>
#include <valarray>
#include <unordered_map>

struct advent_2018_10 : problem
{
	advent_2018_10() noexcept : problem(2018, 10) {
	}

protected:
	struct point
	{
		std::ptrdiff_t x;
		std::ptrdiff_t y;

		friend bool operator==(const point& lhs, const point& rhs) noexcept {
			return lhs.x == rhs.x && lhs.y == rhs.y;
		}
	};

	struct particle
	{
		point position;
		point velocity;

		static auto position_at_time(std::ptrdiff_t t) noexcept {
			return [t] (const particle& p) noexcept {
				return point{ p.position.x + (t * p.velocity.x),
				              p.position.y + (t * p.velocity.y) };
			};
		};

		friend bool operator==(const particle& lhs, const particle& rhs) noexcept {
			return lhs.position == rhs.position && lhs.velocity == rhs.velocity;
		}
	};

	std::vector<particle> particles;

	struct bounding_box
	{
		point top_right   = { std::numeric_limits<std::ptrdiff_t>::min(), std::numeric_limits<std::ptrdiff_t>::min() };
		point bottom_left = { std::numeric_limits<std::ptrdiff_t>::max(), std::numeric_limits<std::ptrdiff_t>::max() };

		void update(const point& p) noexcept {
			top_right   = { std::max(p.x, top_right.x  ), std::max(p.y, top_right.y  ) };
			bottom_left = { std::min(p.x, bottom_left.x), std::min(p.y, bottom_left.y) };
		}

		std::ptrdiff_t area() const noexcept {
			return std::abs(top_right.x - bottom_left.x) * std::abs(top_right.y - bottom_left.y);
		}

		static bounding_box enlarge(bounding_box b, const point& p) noexcept {
			b.update(p);
			return b;
		}
	};

	std::ptrdiff_t time_taken = 0;
	std::vector<point> lights;

	void prepare_input(std::ifstream& fin) override {
		particles = ranges::getlines(fin)
		          | ranges::view::transform([] (const std::string& line) {
		          			static const std::regex pattern(R"(position=< ?(-?[[:digit:]]+),  ?(-?[[:digit:]]+)> velocity=< ?(-?[[:digit:]]+),  ?(-?[[:digit:]]+)>)");
		          			std::smatch m;
		          			std::regex_match(line, m, pattern);
		          			std::ptrdiff_t x = std::stoll(m[1]);
		          			std::ptrdiff_t y = std::stoll(m[2]);
		          			std::ptrdiff_t dx = std::stoll(m[3]);
		          			std::ptrdiff_t dy = std::stoll(m[4]);
		          			return particle{ {x, y}, {dx, dy} };
		          		})
		          | ranges::to_vector;

		std::ptrdiff_t t_min = std::numeric_limits<std::ptrdiff_t>::max();
		std::ptrdiff_t t_max = 0ui64;
		ranges::for_each(particles, [&t_min, &t_max] (const particle& p) noexcept {
			const std::ptrdiff_t approx_x_time = std::abs(p.position.x / p.velocity.x);
			const std::ptrdiff_t approx_y_time = std::abs(p.position.x / p.velocity.x);
			t_min = std::min({ t_min, approx_x_time, approx_y_time });
			t_max = std::max({ t_max, approx_x_time, approx_y_time });
		});

		namespace r = ranges;
		namespace v = ranges::view;

		auto z = r::min(v::closed_iota(t_min, t_max)
		              | v::transform([&](const auto& t) {
		              		return std::make_tuple(t,
		              		                       v::cartesian_product(v::single(t) | v::transform(&particle::position_at_time), v::all(particles))
		              		                     | v::transform([](const auto& fn_tup) { return std::get<0>(fn_tup)(std::get<1>(fn_tup)); }));
		              		})
		              | v::transform([](const auto& tup) {
		              		return std::make_tuple(std::get<0>(tup),
		              		                       std::get<1>(tup) | r::to_vector,
		              		                       r::accumulate(std::get<1>(tup), bounding_box{}, &bounding_box::enlarge));
		              		})
		              , ranges::less{}, [] (const auto& tup) { return std::get<2>(tup).area(); });

		time_taken = std::get<0>(z);
		lights     = std::get<1>(z);
	}

	static const inline std::unordered_map<std::uint64_t, char> letter_codes = { {
		{ 0b001100'010010'100001'100001'100001'111111'100001'100001'100001'100001, 'A'},
		{ 0b111110'100001'100001'100001'111110'100001'100001'100001'100001'111110, 'B'},
		{ 0b011110'100001'100000'100000'100000'100000'100000'100000'100001'011110, 'C'},
		{ 0b111111'100000'100000'100000'111110'100000'100000'100000'100000'111111, 'E'},
		{ 0b111111'100000'100000'100000'111110'100000'100000'100000'100000'100000, 'F'},
		{ 0b011110'100001'100000'100000'100000'100111'100001'100001'100011'011101, 'G'},
		{ 0b100001'100001'100001'100001'111111'100001'100001'100001'100001'100001, 'H'},
		{ 0b000111'000010'000010'000010'000010'000010'000010'100010'100010'011100, 'J'},
		{ 0b100001'100010'100100'101000'110000'110000'101000'100100'100010'100001, 'K'},
		{ 0b100000'100000'100000'100000'100000'100000'100000'100000'100000'111111, 'L'},
		{ 0b100001'110001'110001'101001'101001'100101'100101'100011'100011'100001, 'N'},
		{ 0b111110'100001'100001'100001'111110'100000'100000'100000'100000'100000, 'P'},
		{ 0b111110'100001'100001'100001'111110'100100'100010'100010'100001'100001, 'R'},
		{ 0b100001'100001'010010'010010'001100'001100'010010'010010'100001'100001, 'X'},
		{ 0b111111'000001'000001'000010'000100'001000'010000'100000'100000'111111, 'Z'}
	}};

	std::string part_1() override {
		const std::ptrdiff_t x_min = ranges::min(lights, ranges::less{}, &point::x).x;
		const std::ptrdiff_t x_max = ranges::max(lights, ranges::less{}, &point::x).x;
		const std::ptrdiff_t y_min = ranges::min(lights, ranges::less{}, &point::y).y;
		const std::ptrdiff_t y_max = ranges::max(lights, ranges::less{}, &point::y).y;

		using screen_type = std::valarray<char>;

		const std::size_t letter_width = 6ui64;
		const std::size_t letter_gap = 2ui64;
		const std::size_t letter_height = 10ui64;
		const std::size_t pixels_per_letter = letter_width * letter_height;
		const std::size_t width  = x_max - x_min + 1ui64;
		const std::size_t height = y_max - y_min + 1ui64;
		const std::size_t total_letters = (width + letter_gap) / (letter_width + letter_gap);
		const std::size_t total_pixels = width * height;

		screen_type screen = screen_type(total_pixels);

		screen = ' ';
		for(point p : lights) {
			p.x -= x_min;
			p.y -= y_min;

			screen[(p.y * width) + p.x] = '*';
		}

		screen_type letters = screen[std::gslice(0ui64, { total_letters, height, letter_width }, { letter_width + letter_gap, width, 1ui64 })];

		std::string result;
		for(std::size_t letter = 0; letter < total_letters; ++letter) {
			std::uint64_t letter_code = 0;
			for(std::size_t pixel = 0; pixel < pixels_per_letter; ++pixel) {
				const std::uint64_t bit = letters[(letter * pixels_per_letter) + pixel] == '*' ? 1ui64 : 0ui64;
				letter_code |= bit << (pixels_per_letter - 1ui64 - pixel);
			}
			result += letter_codes.at(letter_code);
		}
		return result;
	}

	std::string part_2() override {
		return std::to_string(time_taken);
	}
};

template<>
void solve<advent_year::year_2018, advent_day::day_10>() {
	advent_2018_10 a;
	a.solve();
}
