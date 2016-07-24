// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Eric Niebler 2014
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
//===-------------------------- algorithm ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
#ifndef STL2_DETAIL_ALGORITHM_PARTITION_HPP
#define STL2_DETAIL_ALGORITHM_PARTITION_HPP

#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/algorithm/find_if_not.hpp>
#include <stl2/detail/concepts/algorithm.hpp>
#include <stl2/detail/concepts/callable.hpp>

///////////////////////////////////////////////////////////////////////////
// partition [alg.partitions]
//
STL2_OPEN_NAMESPACE {
	template <Permutable I, Sentinel<I> S, class Pred, class Proj = identity>
	requires
		IndirectCallablePredicate<__f<Pred>, projected<I, __f<Proj>>>()
	I partition(I first, S last, Pred&& pred_, Proj&& proj_ = Proj{})
	{
		auto pred = ext::make_callable_wrapper(__stl2::forward<Pred>(pred_));
		auto proj = ext::make_callable_wrapper(__stl2::forward<Proj>(proj_));
		first = __stl2::find_if_not(__stl2::move(first), last,
			__stl2::ref(pred), __stl2::ref(proj));
		if (first != last) {
			for (auto m = first; ++m != last;) {
				if (pred(proj(*m))) {
					__stl2::iter_swap(first, m);
					++first;
				}
			}
		}
		return first;
	}

	template <Permutable I, Sentinel<I> S, class Pred, class Proj = identity>
	requires
		BidirectionalIterator<I>() &&
		IndirectCallablePredicate<__f<Pred>, projected<I, __f<Proj>>>()
	I partition(I first, S last_, Pred&& pred_, Proj&& proj_ = Proj{})
	{
		auto pred = ext::make_callable_wrapper(__stl2::forward<Pred>(pred_));
		auto proj = ext::make_callable_wrapper(__stl2::forward<Proj>(proj_));
		auto last = __stl2::next(first, __stl2::move(last_));

		for (; first != last; ++first) {
			if (!pred(proj(*first))) {
				while (true) {
					if (--last == first) {
						return first;
					}
					if (pred(proj(*last))) {
						__stl2::iter_swap(first, last);
						break;
					}
				}
			}
		}
		return first;
	}

	template <ForwardRange Rng, class Pred, class Proj = identity>
	requires
		Permutable<iterator_t<Rng>>() &&
		IndirectCallablePredicate<__f<Pred>, projected<iterator_t<Rng>, __f<Proj>>>()
	safe_iterator_t<Rng>
	partition(Rng&& rng, Pred&& pred, Proj&& proj = Proj{})
	{
		return __stl2::partition(__stl2::begin(rng), __stl2::end(rng),
			__stl2::forward<Pred>(pred), __stl2::forward<Proj>(proj));
	}
} STL2_CLOSE_NAMESPACE

#endif
