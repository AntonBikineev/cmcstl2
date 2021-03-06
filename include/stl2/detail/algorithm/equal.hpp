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
#ifndef STL2_DETAIL_ALGORITHM_EQUAL_HPP
#define STL2_DETAIL_ALGORITHM_EQUAL_HPP

#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/algorithm.hpp>

///////////////////////////////////////////////////////////////////////////
// equal [alg.equal]
//
STL2_OPEN_NAMESPACE {
	template <InputIterator I1, Sentinel<I1> S1, InputIterator I2,
		class Pred, class Proj1, class Proj2>
	requires
		models::IndirectlyComparable<
			I1, I2, __f<Pred>, __f<Proj1>, __f<Proj2>>
	bool __equal_3(I1 first1, S1 last1, I2 first2, Pred&& pred_,
		Proj1&& proj1_, Proj2&& proj2_)
	{
		auto pred = ext::make_callable_wrapper(__stl2::forward<Pred>(pred_));
		auto proj1 = ext::make_callable_wrapper(__stl2::forward<Proj1>(proj1_));
		auto proj2 = ext::make_callable_wrapper(__stl2::forward<Proj2>(proj2_));

		for (; first1 != last1; ++first1, ++first2) {
			if (!pred(proj1(*first1), proj2(*first2))) {
				return false;
			}
		}
		return true;
	}

	template <InputIterator I1, Sentinel<I1> S1,
		InputIterator I2, Sentinel<I2> S2,
		class Pred, class Proj1, class Proj2>
	requires
		models::IndirectlyComparable<
			I1, I2, __f<Pred>, __f<Proj1>, __f<Proj2>>
	bool __equal_4(I1 first1, S1 last1, I2 first2, S2 last2, Pred&& pred_,
		Proj1&& proj1_, Proj2&& proj2_)
	{
		auto pred = ext::make_callable_wrapper(__stl2::forward<Pred>(pred_));
		auto proj1 = ext::make_callable_wrapper(__stl2::forward<Proj1>(proj1_));
		auto proj2 = ext::make_callable_wrapper(__stl2::forward<Proj2>(proj2_));

		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			if (!pred(proj1(*first1), proj2(*first2))) {
				return false;
			}
		}
		return first1 == last1 && first2 == last2;
	}

	template <class I1, class S1, class I2, class Pred = equal_to<>,
		class Proj1 = identity, class Proj2 = identity>
	[[deprecated]] bool
	equal(I1&& first1, S1&& last1, I2&& first2, Pred&& pred = Pred{},
		Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{})
	requires
		models::InputIterator<__f<I1>> &&
		models::Sentinel<__f<S1>, __f<I1>> &&
		models::InputIterator<__f<I2>> &&
		models::IndirectlyComparable<
			__f<I1>, __f<I2>, __f<Pred>, __f<Proj1>, __f<Proj2>>
	{
		return __stl2::__equal_3(
			__stl2::forward<I1>(first1), __stl2::forward<S1>(last1),
			__stl2::forward<I2>(first2), __stl2::forward<Pred>(pred),
			__stl2::forward<Proj1>(proj1), __stl2::forward<Proj2>(proj2));
	}

	template <InputRange Rng1, class I2, class Pred = equal_to<>,
		class Proj1 = identity, class Proj2 = identity>
	[[deprecated]] bool equal(Rng1&& rng1, I2&& first2, Pred&& pred = Pred{},
		Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{})
	requires
		!is_array<remove_reference_t<I2>>::value &&
		models::InputIterator<__f<I2>> &&
		models::IndirectlyComparable<
			iterator_t<Rng1>, __f<I2>, __f<Pred>, __f<Proj1>, __f<Proj2>>
	{
		return __stl2::__equal_3(__stl2::begin(rng1), __stl2::end(rng1),
			__stl2::forward<I2>(first2), __stl2::forward<Pred>(pred),
			__stl2::forward<Proj1>(proj1), __stl2::forward<Proj2>(proj2));
	}

	template <class I1, class S1, class I2, class S2, class Pred = equal_to<>,
		class Proj1 = identity, class Proj2 = identity>
	requires
		models::InputIterator<__f<I1>> &&
		models::InputIterator<__f<I2>> &&
		Sentinel<__f<S1>, __f<I1>>() &&
		Sentinel<__f<S2>, __f<I2>>() &&
		models::IndirectlyComparable<
			__f<I1>, __f<I2>, __f<Pred>, __f<Proj1>, __f<Proj2>>
	bool equal(I1&& first1, S1&& last1, I2&& first2, S2&& last2, Pred&& pred = Pred{},
		Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{})
	{
		return __stl2::__equal_4(
			__stl2::forward<I1>(first1), __stl2::forward<S1>(last1),
			__stl2::forward<I2>(first2), __stl2::forward<S2>(last2),
			__stl2::forward<Pred>(pred), __stl2::forward<Proj1>(proj1),
			__stl2::forward<Proj2>(proj2));
	}

	template <class I1, class S1, class I2, class S2, class Pred = equal_to<>,
		class Proj1 = identity, class Proj2 = identity>
	requires
		models::InputIterator<__f<I1>> &&
		models::InputIterator<__f<I2>> &&
		SizedSentinel<__f<S1>, __f<I1>>() &&
		SizedSentinel<__f<S2>, __f<I2>>() &&
		models::IndirectlyComparable<
			__f<I1>, __f<I2>, __f<Pred>, __f<Proj1>, __f<Proj2>>
	bool equal(I1&& first1_, S1&& last1_, I2&& first2_, S2&& last2_, Pred&& pred = Pred{},
		Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{})
	{
		// This function is a bit weird: it takes first1...last2 by
		// forwarding reference, despite actually using them, so that the
		// compiler can tell this function is more constrained than the
		// non-SizedSentinel overload which takes those arguments by
		// forwarding reference for good reason.
		auto first1 = __stl2::forward<I1>(first1_);
		auto last1 = __stl2::forward<S1>(last1_);
		auto first2 = __stl2::forward<I2>(first2_);
		auto len1 = __stl2::distance(first1, last1);
		auto len2 = __stl2::distance(first2, __stl2::forward<S2>(last2_));
		return len1 == len2 &&
			__stl2::__equal_3(__stl2::move(first1), __stl2::move(last1),
				__stl2::move(first2), __stl2::forward<Pred>(pred),
				__stl2::forward<Proj1>(proj1), __stl2::forward<Proj2>(proj2));
	}

	template <InputRange Rng1, InputRange Rng2, class Pred = equal_to<>,
		class Proj1 = identity, class Proj2 = identity>
	requires
		models::IndirectlyComparable<
			iterator_t<Rng1>, iterator_t<Rng2>,
			__f<Pred>, __f<Proj1>, __f<Proj2>>
	bool equal(Rng1&& rng1, Rng2&& rng2, Pred&& pred = Pred{},
		Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{})
	{
		return __stl2::__equal_4(
			__stl2::begin(rng1), __stl2::end(rng1),
			__stl2::begin(rng2), __stl2::end(rng2),
			__stl2::forward<Pred>(pred),
			__stl2::forward<Proj1>(proj1),
			__stl2::forward<Proj2>(proj2));
	}

	template <InputRange Rng1, InputRange Rng2, class Pred = equal_to<>,
		class Proj1 = identity, class Proj2 = identity>
	requires
		models::SizedRange<Rng1> &&
		models::SizedRange<Rng2> &&
		models::IndirectlyComparable<
			iterator_t<Rng1>, iterator_t<Rng2>,
		__f<Pred>, __f<Proj1>, __f<Proj2>>
	bool equal(Rng1&& rng1, Rng2&& rng2, Pred&& pred = Pred{},
		Proj1&& proj1 = Proj1{}, Proj2&& proj2 = Proj2{})
	{
		auto len1 = __stl2::distance(rng1);
		auto len2 = __stl2::distance(rng2);
		return len1 == len2 &&
			__stl2::__equal_3(
				__stl2::begin(rng1), __stl2::end(rng1),
				__stl2::begin(rng2), __stl2::forward<Pred>(pred),
				__stl2::forward<Proj1>(proj1),
				__stl2::forward<Proj2>(proj2));
	}

	// Extension
	template <class E, class...Args>
	bool equal(std::initializer_list<E>&& rng, Args&&...args)
	requires requires { __stl2::equal(rng, (Args&&)args...); }
	{
		return __stl2::equal(rng, __stl2::forward<Args>(args)...);
	}

	// Extension
	template <class E1, class E2, class...Args>
	bool equal(std::initializer_list<E1>&& rng1,
		std::initializer_list<E2>&& rng2, Args&&...args)
	requires requires { __stl2::equal(rng1, rng2, (Args&&)args...); }
	{
		return __stl2::equal(rng1, rng2, __stl2::forward<Args>(args)...);
	}
} STL2_CLOSE_NAMESPACE

#endif
