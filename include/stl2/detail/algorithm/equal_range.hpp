// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_ALGORITHM_EQUAL_RANGE_HPP
#define STL2_DETAIL_ALGORITHM_EQUAL_RANGE_HPP

#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/algorithm/lower_bound.hpp>
#include <stl2/detail/algorithm/upper_bound.hpp>
#include <stl2/detail/concepts/callable.hpp>
#include <stl2/detail/range/range.hpp>

///////////////////////////////////////////////////////////////////////////
// equal_range [equal.range]
//
STL2_OPEN_NAMESPACE {
	namespace ext {
		template<ForwardIterator I, class T, class Comp = less<>, class Proj = identity>
		requires
			models::IndirectCallableStrictWeakOrder<
				__f<Comp>, const T*, projected<I, __f<Proj>>>
		ext::range<I> equal_range_n(I first, difference_type_t<I> dist, const T& value,
			Comp comp_ = Comp{}, Proj proj_ = Proj{})
		{
			if (0 < dist) {
				auto comp = ext::make_callable_wrapper(__stl2::forward<Comp>(comp_));
				auto proj = ext::make_callable_wrapper(__stl2::forward<Proj>(proj_));
				do {
					auto half = dist / 2;
					auto middle = __stl2::next(first, half);
					auto&& v = *middle;
					auto&& pv = proj(__stl2::forward<decltype(v)>(v));
					if (comp(pv, value)) {
						first = __stl2::move(middle);
						++first;
						dist -= half + 1;
					} else if (comp(value, pv)) {
						dist = half;
					} else {
						return {
							ext::lower_bound_n(
								__stl2::move(first), half, value,
								__stl2::ref(comp), __stl2::ref(proj)),
							ext::upper_bound_n(__stl2::next(middle),
								dist - (half + 1), value,
								__stl2::ref(comp), __stl2::ref(proj))
						};
					}
				} while (0 != dist);
			}
			return {first, first};
		}
	}

	template <ForwardIterator I, Sentinel<I> S, class T,
		class Comp = less<>, class Proj = identity>
	requires
		models::IndirectCallableStrictWeakOrder<
			__f<Comp>, const T*, projected<I, __f<Proj>>>
	ext::range<I> equal_range(I first, S last, const T& value,
		Comp&& comp_ = Comp{}, Proj&& proj_ = Proj{})
	{
		// Probe exponentially for either end-of-range, an iterator that
		// is past the equal range (i.e., denotes an element greater
		// than value), or is in the equal range (denotes an element equal
		// to value).
		auto comp = ext::make_callable_wrapper(__stl2::forward<Comp>(comp_));
		auto proj = ext::make_callable_wrapper(__stl2::forward<Proj>(proj_));
		auto dist = difference_type_t<I>{1};
		while (true) {
			auto mid = first;
			auto d = __stl2::advance(mid, dist, last);
			STL2_ASSUME(d >= 0);
			if (d || mid == last) {
				// at the end of the input range
				return ext::equal_range_n(
					__stl2::move(first), dist - d, value,
					__stl2::ref(comp), __stl2::ref(proj));
			}
			auto&& v = *mid;
			auto&& pv = proj(__stl2::forward<decltype(v)>(v));
			// if value < *mid, mid is after the target range.
			if (comp(value, pv)) {
				return ext::equal_range_n(
					__stl2::move(first), dist, value,
					__stl2::ref(comp), __stl2::ref(proj));
			} else if (!comp(pv, value)) {
				// *mid == value: the lower bound is <= mid, and the upper bound is > mid.
				return {
					ext::lower_bound_n(__stl2::move(first), dist, value,
						__stl2::ref(comp), __stl2::ref(proj)),
					__stl2::upper_bound(__stl2::move(mid), __stl2::move(last),
						value, __stl2::ref(comp), __stl2::ref(proj))
				};
			}
			// *mid < value, mid is before the target range.
			first = __stl2::move(mid);
			++first;
			dist *= 2;
		}
	}

	template <ForwardIterator I, SizedSentinel<I> S, class T,
		class Comp = less<>, class Proj = identity>
	requires
		models::IndirectCallableStrictWeakOrder<
			__f<Comp>, const T*, projected<I, __f<Proj>>>
	ext::range<I> equal_range(I first, S last, const T& value,
		Comp&& comp = Comp{}, Proj&& proj = Proj{})
	{
		auto len = __stl2::distance(first, __stl2::move(last));
		return ext::equal_range_n(__stl2::move(first), len, value,
			__stl2::forward<Comp>(comp), __stl2::forward<Proj>(proj));
	}

	template <ForwardRange Rng, class T,
		class Comp = less<>, class Proj = identity>
	requires
		models::IndirectCallableStrictWeakOrder<
			__f<Comp>, const T*, projected<iterator_t<Rng>, __f<Proj>>>
	ext::range<safe_iterator_t<Rng>> equal_range(Rng&& rng, const T& value,
		Comp&& comp = Comp{}, Proj&& proj = Proj{})
	{
		return __stl2::equal_range(
			__stl2::begin(rng), __stl2::end(rng), value,
			__stl2::forward<Comp>(comp), __stl2::forward<Proj>(proj));
	}

	template <ForwardRange Rng, class T, class Comp = less<>,
		class Proj = identity>
	requires
		models::IndirectCallableStrictWeakOrder<
			__f<Comp>, const T*, projected<iterator_t<Rng>, __f<Proj>>> &&
		models::SizedRange<Rng>
	ext::range<safe_iterator_t<Rng>> equal_range(Rng&& rng, const T& value,
		Comp&& comp = Comp{}, Proj&& proj = Proj{})
	{
		return ext::equal_range_n(
			__stl2::begin(rng), __stl2::size(rng), value,
			__stl2::forward<Comp>(comp), __stl2::forward<Proj>(proj));
	}

	// Extension
	template <class E, class T, class Comp = less<>, class Proj = identity>
	requires
		models::IndirectCallableStrictWeakOrder<
			__f<Comp>, const T*, projected<const E*, __f<Proj>>>
	ext::range<dangling<const E*>>
	equal_range(std::initializer_list<E>&& rng, const T& value,
		Comp&& comp = Comp{}, Proj&& proj = Proj{})
	{
		return ext::equal_range_n(rng.begin(), rng.size(), value,
			__stl2::forward<Comp>(comp), __stl2::forward<Proj>(proj));
	}
} STL2_CLOSE_NAMESPACE

#endif
