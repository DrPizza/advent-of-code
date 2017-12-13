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

	constexpr cyclic_iterator(underlying_iterator pos_, underlying_iterator first_, underlying_iterator last_) noexcept : first(first_), last(last_), current(pos_) {
		if(current == last) {
			current = first;
		}
	}

	constexpr my_type& operator=(const underlying_iterator& rhs) noexcept {
		current = rhs;
		if(current == last) {
			current = first;
		}
		return *this;
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

template<typename T>
struct disjoint_set
{
	struct element
	{
		T value;
		std::size_t size;
		std::size_t rank;
		element* parent;
	};

	std::size_t set_size(const T& id) {
		return find_parent(id)->size;
	}

	bool connected(const T& left, const T& right) {
		return table.find(left) != std::end(table)
			&& table.find(right) != std::end(table)
			&& find_parent(left) == find_parent(right);
	}

	void insert_set(const T& singleton) {
		if(table.find(singleton) == std::end(table)) {
			++distinct_sets;
			element& e = table[singleton];
			e.value = singleton;
			e.size = 1;
			e.parent = &e;
			e.rank = 0;
		}
	}

	void merge_sets(const T& left, const T& right) {
		if(table.find(left) == std::end(table)) {
			insert_set(left);
		}
		if(table.find(right) == std::end(table)) {
			insert_set(right);
		}
		gsl::not_null<element*> e1 = find_parent(left);
		gsl::not_null<element*> e2 = find_parent(right);
		if(e1 == e2) {
			return;
		}
		--distinct_sets;
		if(e1->rank < e2->rank) {
			e1->parent = e2;
			e2->size += e1->size;
		} else if(e1->rank > e2->rank) {
			e2->parent = e1;
			e1->size += e2->size;
		} else {
			e2->parent = e1;
			e1->rank += 1;
			e1->size += e2->size;
		}
	}

	std::size_t count_sets() const noexcept {
		return distinct_sets;
	}

private:
	gsl::not_null<element*> find_parent(const T& key) {
		element& e = table[key];
		if(&e != e.parent) {
			e.parent = find_parent(e.parent->value);
		}
		return e.parent;
	}

	std::unordered_map<T, element> table;
	std::size_t distinct_sets = 0;
};

template<typename T, typename InIt, typename Trans, typename Red>
T transform_reduce(InIt first, InIt last, T init, Red&& red, Trans&& trans) {
	T result{ init };
	for(; first != last; ++first) {
		result = std::forward<Red>(red)(result, std::forward<Trans>(trans)(*first));
	}
	return result;
}
