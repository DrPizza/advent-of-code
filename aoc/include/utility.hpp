#pragma once

#include <functional>
#include <iterator>
#include <cstddef>

template<typename It, typename Pred>
It find_odd_one_out(It begin, It end, Pred p) {
	if(begin == end) {
		return end;
	}
	auto candidate          = *begin;
	std::size_t times_seen  = 0;
	auto candidate_position = end;

	for(auto it = begin; it != end; ++it) {
		if(p(candidate, *it)) {
			++times_seen;
		} else {
			candidate_position = it;
		}
	}
	if(times_seen == 1) {
		return begin;
	} else {
		return candidate_position;
	}
}

template<typename It>
It find_odd_one_out(It begin, It end) {
	return find_odd_one_out(begin, end, std::equal_to<void>{});
}

template<typename T>
struct cyclic_iterator
{
	using underlying_iterator = T;

	using iterator_category = typename underlying_iterator::iterator_category;
	using value_type = typename underlying_iterator::value_type;
	using difference_type = typename underlying_iterator::difference_type;
	using pointer = typename underlying_iterator::pointer;
	using reference = typename underlying_iterator::reference;

	using my_type = cyclic_iterator<underlying_iterator>;

	constexpr cyclic_iterator(underlying_iterator first_, underlying_iterator last_) noexcept : first(first_), last(last_), current(first_) {
	}

	constexpr bool operator==(const my_type& rhs) const noexcept {
		return current == rhs.current;
	}

	constexpr bool operator!=(const my_type& rhs) const noexcept {
		return current != rhs.current;
	}

	constexpr bool operator<(const my_type&) const noexcept {
		return false;
	}

	constexpr reference operator*() const noexcept {
		return *current;
	}

	constexpr pointer operator->() const noexcept {
		if constexpr(std::is_pointer_v<underlying_iterator>) {
			return current;
		} else {
			return current.operator->();
		}
	}

	constexpr my_type& operator++() noexcept {
		++current;
		if(current == last) {
			current = first;
		}
		return *this;
	}

	constexpr my_type operator++(int) noexcept {
		my_type ret(*this);
		++*this;
		return ret;
	}

	constexpr my_type& operator--() noexcept {
		if(current == first) {
			current = last;
		}
		--current;
		return *this;
	}

	constexpr my_type operator--(int) noexcept {
		my_type ret(*this);
		--*this;
		return ret;
	}

	constexpr my_type& operator+=(difference_type delta) noexcept {
		advance(delta);
		return *this;
	}

	constexpr my_type& operator-=(difference_type delta) noexcept {
		advance(-delta);
		return *this;
	}

	constexpr my_type operator+(difference_type delta) const noexcept {
		my_type ret(*this);
		ret += delta;
		return ret;
	}

	constexpr my_type operator-(difference_type delta) const noexcept {
		my_type ret(*this);
		ret -= delta;
		return ret;
	}

private:
	constexpr void advance(difference_type delta) noexcept {
		const auto underlying_size = last - first;
		const auto offset = ((current - first) + delta) % underlying_size;
		current = first + (offset < 0 ? offset + underlying_size : offset);
	}

	underlying_iterator first;
	underlying_iterator last;

	underlying_iterator current;
};