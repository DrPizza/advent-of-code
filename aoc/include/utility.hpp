#pragma once

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
