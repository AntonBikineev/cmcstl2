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
#ifndef STL2_DETAIL_CONCEPTS_FUNCTION_HPP
#define STL2_DETAIL_CONCEPTS_FUNCTION_HPP

#include <stl2/type_traits.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/concepts/compare.hpp>
#include <stl2/detail/concepts/core.hpp>
#include <stl2/detail/concepts/object.hpp>
#include <stl2/detail/functional/invoke.hpp>

///////////////////////////////////////////////////////////////////////////
// Callable Concepts [concepts.lib.callables]
//
STL2_OPEN_NAMESPACE {
	///////////////////////////////////////////////////////////////////////////
	// Callable [concepts.lib.callables.callable]
	//
	// FIXME: remove this transitional paranoia check.
	template <class T>
	constexpr bool __force_non_reference() {
		static_assert(!is_reference<T>::value);
		return true;
	}

	template <class F, class...Args>
	concept bool Callable() {
		return __force_non_reference<F>() &&
			CopyConstructible<F>() &&
			ext::Invokable<F&, Args...>();
	}

	///////////////////////////////////////////////////////////////////////////
	// RegularCallable [concepts.lib.callables.regularcallable]
	//
	template <class F, class...Args>
	concept bool RegularCallable() {
		return Callable<F, Args...>();
	}

	///////////////////////////////////////////////////////////////////////////
	// Predicate [concepts.lib.callables.predicate]
	//
	template <class F, class...Args>
	concept bool Predicate() {
		return RegularCallable<F, Args...>() &&
			Boolean<result_of_t<F&(Args...)>>();
	}

	///////////////////////////////////////////////////////////////////////////
	// Relation [concepts.lib.callables.relation]
	//
	template <class R, class T>
	concept bool Relation() {
		return Predicate<R, T, T>();
	}

	template <class R, class T, class U>
	concept bool Relation() {
		return Relation<R, T>() &&
			Relation<R, U>() &&
			Predicate<R, T, U>() &&
			Predicate<R, U, T>() &&
			CommonReference<const T&, const U&>() &&
			Relation<R, common_reference_t<const T&, const U&>>();
	}

	///////////////////////////////////////////////////////////////////////////
	// StrictWeakOrder [concepts.lib.callables.strictweakorder]
	//
	template <class R, class T>
	concept bool StrictWeakOrder() {
		return Relation<R, T>();
	}

	template <class R, class T, class U>
	concept bool StrictWeakOrder() {
		return Relation<R, T, U>();
	}
} STL2_CLOSE_NAMESPACE

#endif
