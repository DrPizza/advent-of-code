#include "stdafx.h"

#include "problem.hpp"

#include <fstream>

#pragma warning(push)

#pragma warning(disable: 4619 4365 6011 26432 26440 26439 26474 26495 26497 26493 26492 26481 26494 26472 26429 26481 26409 26466 26424 26434 26496)
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#pragma warning(pop)

struct advent_2016_5 : problem
{
	advent_2016_5() noexcept : problem(2016, 5) {
	}

protected:
	std::string key;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, key);
	}

	std::string part_1() override {
		CryptoPP::Weak::MD5 md5;
		std::string password;
		std::array<uint8_t, 16> digest{};
		for(std::size_t i = 0; password.size() < 8; ++i) {
			std::string trial = key + std::to_string(i);
			md5.CalculateDigest(digest.data(), reinterpret_cast<const byte*>(trial.data()), trial.size());
			if(digest[0] == 0x00
			&& digest[1] == 0x00
			&& digest[2] <= 0x0f) {
				const char c = gsl::narrow<char>(digest[2] & 0xf);
				if(c < 10) {
					password += c + '0';
				} else {
					password += (c - 10) + 'a';
				}
			}
		}
		return password;
	}

	std::string part_2() override {
		CryptoPP::Weak::MD5 md5;
		std::string password(8, ' ');
		std::array<uint8_t, 16> digest{};
		std::size_t filled_digits = 0;
		for(std::size_t i = 0; filled_digits < 8; ++i) {
			std::string trial = key + std::to_string(i);
			md5.CalculateDigest(digest.data(), reinterpret_cast<const byte*>(trial.data()), trial.size());
			if(digest[0] == 0x00
			&& digest[1] == 0x00
			&& digest[2] <= 0x0f) {
				std::size_t offset = gsl::narrow<std::size_t>(digest[2] & 0xf);
				char c = gsl::narrow<char>((digest[3] & 0xf0) >> 4);
				if(c < 10) {
					c = c + '0';
				} else {
					c = (c - 10) + 'a';
				}
				if(offset < 8 && password[offset] == ' ') {
					password[offset] = c;
					++filled_digits;
				}
			}
		}
		return password;
	}

};

template<>
void solve<advent_year::year_2016, advent_day::day_5>() {
	advent_2016_5 a;
	a.solve();
}
