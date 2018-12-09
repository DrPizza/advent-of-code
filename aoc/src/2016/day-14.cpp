#include "stdafx.h"

#include "problem.hpp"

#include <fstream>
#include <set>
#include <deque>
#include <array>

#pragma warning(push)
#pragma warning(disable: 4619 4365 6011 26432 26433 26439 26440 26443 26447 26455 26474 26495 26497 26493 26492 26481 26494 26472 26429 26481 26409 26466 26424 26434 26496)
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#pragma warning(pop)

using digest_type = std::array<std::uint8_t, 16>;
using hex_digest_type = std::array<std::uint8_t, 32>;

std::uint8_t find_trip(const hex_digest_type& d) noexcept {
	for(std::size_t i = 0; i < d.size() - 2; ++i) {
		if(d[i + 0] == d[i + 1]
		&& d[i + 1] == d[i + 2]) {
			return d[i];
		}
	}
	return 0xffui8;
}

bool has_five(const hex_digest_type& d, std::uint8_t needle) noexcept {
	for(std::size_t i = 0; i < d.size() - 4; ++i) {
		if(d[i + 0] == needle
		&& d[i + 1] == needle
		&& d[i + 2] == needle
		&& d[i + 3] == needle
		&& d[i + 4] == needle) {
			return true;
		}
	}
	return false;
}

hex_digest_type to_hex_digest(const digest_type& d) noexcept {
	hex_digest_type result = {};
	for(std::size_t i = 0; i < d.size(); ++i) {
		result[(i * 2)    ] = gsl::narrow_cast<std::uint8_t>((d[i] & 0xf0ui8) >> 4ui8);
		result[(i * 2) + 1] = gsl::narrow_cast<std::uint8_t>((d[i] & 0x0fui8) >> 0ui8);
	}
	for(std::size_t l = 0; l < result.size(); ++l) {
		if(result[l] < 10) {
			result[l] += '0';
		} else {
			result[l] += 'a' - 10;
		}
	}
	return result;
}

hex_digest_type generate_digest(const std::string& salt, std::size_t i, const std::size_t stretch_factor) {
	CryptoPP::Weak::MD5 md5;
	std::string trial = salt + std::to_string(i);
	digest_type digest;
	md5.CalculateDigest(digest.data(), reinterpret_cast<const byte*>(trial.data()), trial.size());
	for(std::size_t k = 0; k < stretch_factor; ++k) {
		hex_digest_type hd = to_hex_digest(digest);
		md5.CalculateDigest(digest.data(), hd.data(), hd.size());
	}
	return to_hex_digest(digest);
}

struct advent_2016_14 : problem
{
	advent_2016_14() noexcept : problem(2016, 14) {
	}

protected:
	std::string salt;

	void prepare_input(std::ifstream& fin) override {
		std::getline(fin, salt);
	}

	std::size_t find_key_index(const std::size_t stretch_factor) {
		std::size_t valid_keys = 0;
		std::deque<hex_digest_type> digests;

		for(std::size_t i = 0; i < 1'001; ++i) {
			digests.push_back(generate_digest(salt, i, stretch_factor));
		}

		for(std::size_t i = 0; ; ++i) {
			const hex_digest_type hd = digests[0];
			const std::uint8_t trip = find_trip(hd);
			if(trip != 0xffui8) {
				for(std::size_t j = 1; j <= 1'000; ++j) {
					if(has_five(digests[j], trip)) {
						++valid_keys;
						break;
					}
				}
			}
			if(valid_keys == 64) {
				return i;
			}
			digests.pop_front();
			digests.push_back(generate_digest(salt, i + 1'000 + 1, stretch_factor));
		}

		return 0;
	}

	std::string part_1() override {
		return std::to_string(find_key_index(0));
	}

	std::string part_2() override {
		return std::to_string(find_key_index(2016));
	}
};

template<>
void solve<advent_year::year_2016, advent_day::day_14>() {
	advent_2016_14 a;
	a.solve();
}
