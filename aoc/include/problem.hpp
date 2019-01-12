#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <gsl/gsl>
#include <string>
#include <map>
#include <tuple>

struct problem
{
	void solve();

	problem(std::size_t year_, std::size_t day_) noexcept : year(year_), day(day_) {
	}

	problem(const problem&) = default;
	problem(problem&&)      = default;
	problem& operator=(const problem&) = default;
	problem& operator=(problem&&) = default;
	virtual ~problem()            = default;

protected:
	[[gsl::suppress(f.6)]]
	virtual void prepare_input(std::ifstream&) {
	}

	[[gsl::suppress(f.6)]]
	virtual void precompute() {
	}

	[[gsl::suppress(f.6)]]
	virtual void tidy_up() {
	}

	[[gsl::suppress(f.6)]]
	virtual std::string part_1() {
		return "";
	}

	[[gsl::suppress(f.6)]] virtual std::string part_2() {
		return "";
	}

private:
	std::size_t year;
	std::size_t day;
};

using problem_factory_t = std::function<std::shared_ptr<problem> ()>;

std::map<std::tuple<std::size_t, std::size_t>, problem_factory_t>& get_registrations();

bool register_solver(std::size_t year, std::size_t day, problem_factory_t factory);

#define REGISTER_SOLVER(YEAR, DAY) static bool registered = register_solver(YEAR, DAY, [] () -> std::shared_ptr<problem> { return std::make_shared<advent_##YEAR##_##DAY>(); })

#endif
