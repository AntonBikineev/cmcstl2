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
#ifndef STL2_DETAIL_ALGORITHM_INCLUDES_HPP
#define STL2_DETAIL_ALGORITHM_INCLUDES_HPP

#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/callable.hpp>

///////////////////////////////////////////////////////////////////////////
// includes [includes]
//
STL2_OPEN_NAMESPACE {
	template <InputIterator I1, Sentinel<I1> S1,
		InputIterator I2, Sentinel<I2> S2, class Comp = less<>,
		class Proj1 = identity, class Proj2 = identity>
	requires
		models::IndirectCallableStrictWeakOrder<
			__f<Comp>, projected<I1, __f<Proj1>>, projected<I2, __f<Proj2>>>
	bool includes(I1 first1, S1 last1, I2 first2, S2 last2, Comp&& comp_ = Comp{},
		Proj1&& proj1_ = Proj1{}, Proj2&& proj2_ = Proj2{})
	{
		auto comp = ext::make_callable_wrapper(__stl2::forward<Comp>(comp_));
		auto proj1 = ext::make_callable_wrapper(__stl2::forward<Proj1>(proj1_));
		auto proj2 = ext::make_callable_wrapper(__stl2::forward<Proj2>(proj2_));

		while (true) {
			if (first2 == last2) {
				return true;
			}
			if (first1 == last1) {
				return false;
			}
			if (comp(proj2(*first2), proj1(*first1))) {
				return false;
			}
			if (!comp(proj1(*first1), proj2(*first2))) {
				++first2;
			}
			++first1;
		}
	}

	template <InputRange Rng1, InputRange Rng2, class Comp = less<>,
		class Proj1 = identity, class Proj2 = identity>
	requires
		models::IndirectCallableStrictWeakOrder<__f<Comp>,
			projected<iterator_t<Rng1>, __f<Proj1>>,
			projected<iterator_t<Rng2>, __f<Proj2>>>
	bool includes(Rng1&& rng1, Rng2&& rng2, Comp&& comp = Comp{},
		Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{})
	{
		return __stl2::includes(
			__stl2::begin(rng1), __stl2::end(rng1),
			__stl2::begin(rng2), __stl2::end(rng2),
			__stl2::forward<Comp>(comp),
			__stl2::forward<Proj1>(proj1),
			__stl2::forward<Proj2>(proj2));
	}

	// Extension
	template <class E, InputRange Rng2, class Comp = less<>,
		class Proj1 = identity, class Proj2 = identity>
	requires
		models::IndirectCallableStrictWeakOrder<__f<Comp>,
			projected<const E*, __f<Proj1>>,
			projected<iterator_t<Rng2>, __f<Proj2>>>
	bool includes(std::initializer_list<E>&& rng1, Rng2&& rng2, Comp&& comp = Comp{},
		Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{})
	{
		return __stl2::includes(
			__stl2::begin(rng1), __stl2::end(rng1),
			__stl2::begin(rng2), __stl2::end(rng2),
			__stl2::forward<Comp>(comp),
			__stl2::forward<Proj1>(proj1),
			__stl2::forward<Proj2>(proj2));
	}

	// Extension
	template <InputRange Rng1, class E, class Comp = less<>,
		class Proj1 = identity, class Proj2 = identity>
	requires
		models::IndirectCallableStrictWeakOrder<__f<Comp>,
			projected<iterator_t<Rng1>, __f<Proj1>>,
			projected<const E*, __f<Proj2>>>
	bool includes(Rng1&& rng1, std::initializer_list<E>&& rng2, Comp&& comp = Comp{},
		Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{})
	{
		return __stl2::includes(
			__stl2::begin(rng1), __stl2::end(rng1),
			__stl2::begin(rng2), __stl2::end(rng2),
			__stl2::forward<Comp>(comp),
			__stl2::forward<Proj1>(proj1),
			__stl2::forward<Proj2>(proj2));
	}

	// Extension
	template <class E1, class E2, class Comp = less<>,
		class Proj1 = identity, class Proj2 = identity>
	requires
		models::IndirectCallableStrictWeakOrder<__f<Comp>,
			projected<const E1*, __f<Proj1>>,
			projected<const E2*, __f<Proj2>>>
	bool includes(std::initializer_list<E1>&& rng1,
		std::initializer_list<E2>&& rng2, Comp&& comp = Comp{},
		Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{})
	{
		return __stl2::includes(
			__stl2::begin(rng1), __stl2::end(rng1),
			__stl2::begin(rng2), __stl2::end(rng2),
			__stl2::forward<Comp>(comp),
			__stl2::forward<Proj1>(proj1),
			__stl2::forward<Proj2>(proj2));
	}
} STL2_CLOSE_NAMESPACE

#endif
