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
#ifndef STL2_DETAIL_ALGORITHM_COUNT_HPP
#define STL2_DETAIL_ALGORITHM_COUNT_HPP

#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/callable.hpp>

///////////////////////////////////////////////////////////////////////////
// count [alg.count]
//
STL2_OPEN_NAMESPACE {
	template <InputIterator I, Sentinel<I> S, class T, class Proj = identity>
	requires
		models::IndirectCallableRelation<
			equal_to<>, projected<I, __f<Proj>>, const T*>
	difference_type_t<I>
	count(I first, S last, const T& value, Proj&& proj_ = Proj{})
	{
		auto proj = ext::make_callable_wrapper(__stl2::forward<Proj>(proj_));
		difference_type_t<I> n = 0;
		for (; first != last; ++first) {
			if (proj(*first) == value) {
				++n;
			}
		}
		return n;
	}

	template <InputRange Rng, class T, class Proj = identity>
	requires
		models::IndirectCallableRelation<
			equal_to<>, projected<iterator_t<Rng>, __f<Proj>>, const T*>
	difference_type_t<iterator_t<Rng>>
	count(Rng&& rng, const T& value, Proj&& proj = Proj{})
	{
		return __stl2::count(__stl2::begin(rng), __stl2::end(rng),
			value, __stl2::forward<Proj>(proj));
	}

	// Extension
	template <class E, class T, class Proj = identity>
	requires
		models::IndirectCallableRelation<
			equal_to<>, projected<const E*, __f<Proj>>, const T*>
	std::ptrdiff_t
	count(std::initializer_list<E>&& rng,
				const T& value, Proj&& proj = Proj{})
	{
		return __stl2::count(rng, value, __stl2::forward<Proj>(proj));
	}
} STL2_CLOSE_NAMESPACE

#endif
