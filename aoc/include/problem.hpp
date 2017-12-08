#pragma once

#include <iostream>
#include <string>
#include <gsl/gsl>

struct problem {
	void solve() {
		prepare_input();
		precompute();
		std::cout << "advent of code 2017 day " << day << std::endl;
		std::cout << "\tpart 1: " << part_1() << std::endl;
		std::cout << "\tpart 2: " << part_2() << std::endl;
	}

	problem(std::size_t day_) noexcept : day(day_) {
	}

	problem(const problem&) = default;
	problem(problem&&) = default;
	problem& operator=(const problem&) = default;
	problem& operator=(problem&&) = default;
	virtual ~problem() = default;

protected:
	[[gsl::suppress(f.6)]]
	virtual void prepare_input() {
	}

	[[gsl::suppress(f.6)]]
	virtual void precompute() {
	}

	[[gsl::suppress(f.6)]]
	virtual std::string part_1() {
		return "";
	}

	[[gsl::suppress(f.6)]]
	virtual std::string part_2() {
		return "";
	}

private:
	std::size_t day;
};
