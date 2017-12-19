#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <sstream>
#include <array>

#pragma warning(push)

#pragma warning(disable: 4619 4365 26432 26440 26439 26474 26495 26497 26493 26492 26481 26494 26472 26429 26481 26409 26466 26424 26434 26496)
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#pragma warning(pop)

struct advent_2015_4 : problem
{
	advent_2015_4() noexcept : problem(2015, 4) {
	}

protected:
	std::string key;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, key);
	}

	std::string part_1() override {
		CryptoPP::Weak::MD5 md5;
		std::array<uint8_t, 16> digest{};
		std::size_t i = 0;
		for(; ; ++i) {
			std::string trial = key + std::to_string(i);
			md5.CalculateDigest(digest.data(), reinterpret_cast<const byte*>(trial.data()), trial.size());
			if(digest[0] == 0x00
			&& digest[1] == 0x00
			&& digest[2] <= 0x0f) {
				break;
			}
		}
		return std::to_string(i);
	}

	std::string part_2() override {
		CryptoPP::Weak::MD5 md5;
		std::array<uint8_t, 16> digest{};
		std::size_t i = 0;
		for(; ; ++i) {
			std::string trial = key + std::to_string(i);
			md5.CalculateDigest(digest.data(), reinterpret_cast<const byte*>(trial.data()), trial.size());
			if(digest[0] == 0x00
			&& digest[1] == 0x00
			&& digest[2] == 0x00) {
				break;
			}
		}
		return std::to_string(i);
	}
};

template<>
void solve<advent_year::year_2015, advent_day::day_4>() {
	advent_2015_4 a;
	a.solve();
}
