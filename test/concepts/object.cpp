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
#include "validate.hpp"

#if VALIDATE_RANGES
namespace ranges {
template<class T, class...Args>
constexpr bool ConstructibleObject =
	ranges::concepts::models<ranges::concepts::ConstructibleObject, T, Args...>();

template<class T, class...Args>
constexpr bool BindableReference =
	ranges::concepts::models<ranges::concepts::BindableReference, T, Args...>();
} // namespace ranges

#elif VALIDATE_STL2
#include <stl2/detail/concepts/object.hpp>
STL2_OPEN_NAMESPACE {
template <class...Ts>
concept bool ConstructibleObject() {
	return ext::ConstructibleObject<Ts...>;
}
template <class...Ts>
concept bool BindableReference() {
	return ext::BindableReference<Ts...>;
}
} STL2_CLOSE_NAMESPACE
#endif

struct copyable {};

struct moveonly {
	moveonly() = default;
	moveonly(moveonly&&) = default;
	moveonly& operator=(moveonly&&) = default;
};

struct copyonly {
	copyonly() = default;
	copyonly(const copyonly&) = default;
	copyonly& operator=(const copyonly&) = default;
	copyonly(copyonly&&) = delete;
	copyonly& operator=(copyonly&&) = delete;
};

struct nonmovable {
	nonmovable() = default;
	nonmovable(nonmovable&&) = delete;
};

struct nondefaultconstructible {
	nondefaultconstructible(int) {}
};

struct indestructible {
	~indestructible() = delete;
};

struct explicit_default {
	explicit explicit_default() {}
};

struct explicit_move {
	explicit_move() = default;
	explicit explicit_move(explicit_move&&) = default;
};

struct explicit_copy {
	explicit_copy() = default;
	explicit_copy(explicit_copy&&) = default;
	explicit explicit_copy(const explicit_copy&) = default;
};

struct partial_overloaded_address {
	partial_overloaded_address* operator&();
};
struct overloaded_address {
	overloaded_address* operator&();
	const overloaded_address* operator&() const;
};
struct bad_overloaded_address {
	void operator&() const;
};
struct bad_overloaded_const_address {
	bad_overloaded_const_address* operator&();
	void operator&() const;
};

struct semiregular {};

struct regular {
	friend constexpr bool operator==(const regular&, const regular&) {
		return true;
	}
	friend constexpr bool operator!=(const regular&, const regular&) {
		return false;
	}
};

struct XXX
{
	XXX() = default;
	XXX(XXX&&) = delete;
	explicit XXX(int) {}
};

CONCEPT_ASSERT(ranges::Destructible<int>());
CONCEPT_ASSERT(ranges::Destructible<const int>());
CONCEPT_ASSERT(!ranges::Destructible<void>());
CONCEPT_ASSERT(!ranges::Destructible<int&>());
CONCEPT_ASSERT(!ranges::Destructible<void()>());
CONCEPT_ASSERT(ranges::Destructible<void(*)()>());
CONCEPT_ASSERT(!ranges::Destructible<void(&)()>());
CONCEPT_ASSERT(!ranges::Destructible<int[]>());
CONCEPT_ASSERT(!ranges::Destructible<int[2]>());
CONCEPT_ASSERT(ranges::Destructible<int(*)[2]>());
CONCEPT_ASSERT(!ranges::Destructible<int(&)[2]>());
CONCEPT_ASSERT(ranges::Destructible<moveonly>());
CONCEPT_ASSERT(ranges::Destructible<nonmovable>());
CONCEPT_ASSERT(!ranges::Destructible<indestructible>());

#if 0
// ill-formed (hard error)
struct incomplete;
CONCEPT_ASSERT(!ranges::Destructible<incomplete>());
#endif

CONCEPT_ASSERT(ranges::Destructible<partial_overloaded_address>());
CONCEPT_ASSERT(ranges::Destructible<overloaded_address>());
CONCEPT_ASSERT(!ranges::Destructible<bad_overloaded_address>());
CONCEPT_ASSERT(!ranges::Destructible<bad_overloaded_const_address>());

CONCEPT_ASSERT(ranges::Constructible<int>());
CONCEPT_ASSERT(ranges::Constructible<int const>());
CONCEPT_ASSERT(!ranges::Constructible<int const&>());
CONCEPT_ASSERT(!ranges::Constructible<int()>());
CONCEPT_ASSERT(!ranges::Constructible<int(&)()>());
CONCEPT_ASSERT(!ranges::Constructible<int[]>());
CONCEPT_ASSERT(!ranges::Constructible<int[5]>());
CONCEPT_ASSERT(!ranges::Constructible<nondefaultconstructible>());
CONCEPT_ASSERT(ranges::Constructible<int const(&)[5], int(&)[5]>());
CONCEPT_ASSERT(!ranges::Constructible<int, int(&)[3]>());

CONCEPT_ASSERT(ranges::ConstructibleObject<int, int>());
CONCEPT_ASSERT(ranges::ConstructibleObject<int, int&>());
CONCEPT_ASSERT(ranges::ConstructibleObject<int, int&&>());
CONCEPT_ASSERT(ranges::ConstructibleObject<int, const int>());
CONCEPT_ASSERT(ranges::ConstructibleObject<int, const int&>());
CONCEPT_ASSERT(ranges::ConstructibleObject<int, const int&&>());
CONCEPT_ASSERT(ranges::Constructible<int, int>());
CONCEPT_ASSERT(ranges::Constructible<int, int&>());
CONCEPT_ASSERT(ranges::Constructible<int, int&&>());
CONCEPT_ASSERT(ranges::Constructible<int, const int>());
CONCEPT_ASSERT(ranges::Constructible<int, const int&>());
CONCEPT_ASSERT(ranges::Constructible<int, const int&&>());

CONCEPT_ASSERT(ranges::ConstructibleObject<copyable, copyable>());
CONCEPT_ASSERT(ranges::ConstructibleObject<copyable, copyable&>());
CONCEPT_ASSERT(ranges::ConstructibleObject<copyable, copyable&&>());
CONCEPT_ASSERT(ranges::ConstructibleObject<copyable, const copyable>());
CONCEPT_ASSERT(ranges::ConstructibleObject<copyable, const copyable&>());
CONCEPT_ASSERT(ranges::ConstructibleObject<copyable, const copyable&&>());
CONCEPT_ASSERT(ranges::Constructible<copyable, copyable>());
CONCEPT_ASSERT(ranges::Constructible<copyable, copyable&>());
CONCEPT_ASSERT(ranges::Constructible<copyable, copyable&&>());
CONCEPT_ASSERT(ranges::Constructible<copyable, const copyable>());
CONCEPT_ASSERT(ranges::Constructible<copyable, const copyable&>());
CONCEPT_ASSERT(ranges::Constructible<copyable, const copyable&&>());

CONCEPT_ASSERT(!ranges::BindableReference<int&, int>());
CONCEPT_ASSERT(ranges::BindableReference<int&, int&>());
CONCEPT_ASSERT(!ranges::BindableReference<int&, int&&>());
CONCEPT_ASSERT(!ranges::BindableReference<int&, const int>());
CONCEPT_ASSERT(!ranges::BindableReference<int&, const int&>());
CONCEPT_ASSERT(!ranges::BindableReference<int&, const int&&>());
CONCEPT_ASSERT(!ranges::Constructible<int&, int>());
CONCEPT_ASSERT(ranges::Constructible<int&, int&>());
CONCEPT_ASSERT(!ranges::Constructible<int&, int&&>());
CONCEPT_ASSERT(!ranges::Constructible<int&, const int>());
CONCEPT_ASSERT(!ranges::Constructible<int&, const int&>());
CONCEPT_ASSERT(!ranges::Constructible<int&, const int&&>());

CONCEPT_ASSERT(ranges::BindableReference<const int&, int>());
CONCEPT_ASSERT(ranges::BindableReference<const int&, int&>());
CONCEPT_ASSERT(ranges::BindableReference<const int&, int&&>());
CONCEPT_ASSERT(ranges::BindableReference<const int&, const int>());
CONCEPT_ASSERT(ranges::BindableReference<const int&, const int&>());
CONCEPT_ASSERT(ranges::BindableReference<const int&, const int&&>());
CONCEPT_ASSERT(ranges::Constructible<const int&, int>());
CONCEPT_ASSERT(ranges::Constructible<const int&, int&>());
CONCEPT_ASSERT(ranges::Constructible<const int&, int&&>());
CONCEPT_ASSERT(ranges::Constructible<const int&, const int>());
CONCEPT_ASSERT(ranges::Constructible<const int&, const int&>());
CONCEPT_ASSERT(ranges::Constructible<const int&, const int&&>());

CONCEPT_ASSERT(ranges::BindableReference<int&&, int>());
CONCEPT_ASSERT(!ranges::BindableReference<int&&, int&>());
CONCEPT_ASSERT(ranges::BindableReference<int&&, int&&>());
CONCEPT_ASSERT(!ranges::BindableReference<int&&, const int>());
CONCEPT_ASSERT(!ranges::BindableReference<int&&, const int&>());
CONCEPT_ASSERT(!ranges::BindableReference<int&&, const int&&>());
CONCEPT_ASSERT(ranges::Constructible<int&&, int>());
CONCEPT_ASSERT(!ranges::Constructible<int&&, int&>());
CONCEPT_ASSERT(ranges::Constructible<int&&, int&&>());
CONCEPT_ASSERT(!ranges::Constructible<int&&, const int>());
CONCEPT_ASSERT(!ranges::Constructible<int&&, const int&>());
CONCEPT_ASSERT(!ranges::Constructible<int&&, const int&&>());

CONCEPT_ASSERT(ranges::BindableReference<const int&&, int>());
CONCEPT_ASSERT(!ranges::BindableReference<const int&&, int&>());
CONCEPT_ASSERT(ranges::BindableReference<const int&&, int&&>());
CONCEPT_ASSERT(ranges::BindableReference<const int&&, const int>());
CONCEPT_ASSERT(!ranges::BindableReference<const int&&, const int&>());
CONCEPT_ASSERT(ranges::BindableReference<const int&&, const int&&>());
CONCEPT_ASSERT(ranges::Constructible<const int&&, int>());
CONCEPT_ASSERT(!ranges::Constructible<const int&&, int&>());
CONCEPT_ASSERT(ranges::Constructible<const int&&, int&&>());
CONCEPT_ASSERT(ranges::Constructible<const int&&, const int>());
CONCEPT_ASSERT(!ranges::Constructible<const int&&, const int&>());
CONCEPT_ASSERT(ranges::Constructible<const int&&, const int&&>());

CONCEPT_ASSERT(ranges::Constructible<XXX, int>());

CONCEPT_ASSERT(ranges::DefaultConstructible<int>());
CONCEPT_ASSERT(ranges::DefaultConstructible<int const>());
CONCEPT_ASSERT(!ranges::DefaultConstructible<int&>());
CONCEPT_ASSERT(!ranges::DefaultConstructible<int const&>());
CONCEPT_ASSERT(!ranges::DefaultConstructible<int()>());
CONCEPT_ASSERT(!ranges::DefaultConstructible<int(&)()>());
CONCEPT_ASSERT(ranges::DefaultConstructible<double>());
CONCEPT_ASSERT(!ranges::DefaultConstructible<void>());
CONCEPT_ASSERT(!ranges::DefaultConstructible<int[]>());
CONCEPT_ASSERT(!ranges::DefaultConstructible<int[2]>());
CONCEPT_ASSERT(!ranges::DefaultConstructible<nondefaultconstructible>());

// It's hard to catch explicit default constructors, see
// http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1518.
// CONCEPT_ASSERT(!ranges::DefaultConstructible<explicit_default>());
CONCEPT_ASSERT(ranges::DefaultConstructible<explicit_move>());
CONCEPT_ASSERT(ranges::DefaultConstructible<explicit_copy>());

CONCEPT_ASSERT(!ranges::MoveConstructible<void>());
CONCEPT_ASSERT(ranges::MoveConstructible<int>());
CONCEPT_ASSERT(ranges::MoveConstructible<const int>());
CONCEPT_ASSERT(!ranges::MoveConstructible<int[4]>());
CONCEPT_ASSERT(!ranges::MoveConstructible<void()>());
CONCEPT_ASSERT(ranges::MoveConstructible<int &>());
CONCEPT_ASSERT(ranges::MoveConstructible<int &&>());
CONCEPT_ASSERT(ranges::MoveConstructible<const int &>());
CONCEPT_ASSERT(ranges::MoveConstructible<const int &&>());

CONCEPT_ASSERT(ranges::ConstructibleObject<moveonly, moveonly>());
CONCEPT_ASSERT(ranges::Constructible<moveonly, moveonly>());
CONCEPT_ASSERT(ranges::MoveConstructible<copyable>());
CONCEPT_ASSERT(ranges::MoveConstructible<moveonly>());
CONCEPT_ASSERT(!ranges::MoveConstructible<nonmovable>());
CONCEPT_ASSERT(!ranges::MoveConstructible<copyonly>());
CONCEPT_ASSERT(!ranges::MoveConstructible<explicit_move>());
CONCEPT_ASSERT(ranges::MoveConstructible<explicit_copy>());

CONCEPT_ASSERT(ranges::MoveConstructible<nonmovable &>());
CONCEPT_ASSERT(ranges::MoveConstructible<nonmovable &&>());
CONCEPT_ASSERT(ranges::MoveConstructible<const nonmovable &>());
CONCEPT_ASSERT(ranges::MoveConstructible<const nonmovable &&>());

CONCEPT_ASSERT(!ranges::CopyConstructible<void>());
CONCEPT_ASSERT(ranges::CopyConstructible<int>());
CONCEPT_ASSERT(ranges::CopyConstructible<const int>());
CONCEPT_ASSERT(ranges::CopyConstructible<int&>());
CONCEPT_ASSERT(!ranges::CopyConstructible<int&&>());
CONCEPT_ASSERT(ranges::CopyConstructible<const int&>());
CONCEPT_ASSERT(!ranges::CopyConstructible<const int&&>());
CONCEPT_ASSERT(!ranges::CopyConstructible<int[4]>());
CONCEPT_ASSERT(!ranges::CopyConstructible<void()>());

CONCEPT_ASSERT(ranges::CopyConstructible<copyable>());
CONCEPT_ASSERT(!ranges::CopyConstructible<moveonly>());
CONCEPT_ASSERT(!ranges::CopyConstructible<nonmovable>());
CONCEPT_ASSERT(!ranges::CopyConstructible<copyonly>());
CONCEPT_ASSERT(!ranges::CopyConstructible<explicit_move>());
CONCEPT_ASSERT(!ranges::CopyConstructible<explicit_copy>());
CONCEPT_ASSERT(ranges::CopyConstructible<nonmovable &>());
CONCEPT_ASSERT(!ranges::CopyConstructible<nonmovable &&>());
CONCEPT_ASSERT(ranges::CopyConstructible<const nonmovable &>());
CONCEPT_ASSERT(!ranges::CopyConstructible<const nonmovable &&>());

CONCEPT_ASSERT(ranges::Movable<int>());
CONCEPT_ASSERT(!ranges::Movable<const int>());
CONCEPT_ASSERT(ranges::Movable<double>());
CONCEPT_ASSERT(!ranges::Movable<void>());
CONCEPT_ASSERT(ranges::Movable<copyable>());
CONCEPT_ASSERT(ranges::Movable<moveonly>());
CONCEPT_ASSERT(!ranges::Movable<nonmovable>());
CONCEPT_ASSERT(!ranges::Movable<copyonly>());

CONCEPT_ASSERT(ranges::Copyable<int>());
CONCEPT_ASSERT(!ranges::Copyable<const int>());
CONCEPT_ASSERT(ranges::Copyable<double>());
CONCEPT_ASSERT(!ranges::Copyable<void>());
CONCEPT_ASSERT(ranges::Copyable<copyable>());
CONCEPT_ASSERT(!ranges::Copyable<moveonly>());
CONCEPT_ASSERT(!ranges::Copyable<nonmovable>());
CONCEPT_ASSERT(!ranges::Copyable<copyonly>());

CONCEPT_ASSERT(ranges::Semiregular<int>());
CONCEPT_ASSERT(ranges::Semiregular<double>());
CONCEPT_ASSERT(!ranges::Semiregular<void>());
CONCEPT_ASSERT(!ranges::Semiregular<int&>());
CONCEPT_ASSERT(ranges::Semiregular<semiregular>());
CONCEPT_ASSERT(ranges::Semiregular<regular>());
CONCEPT_ASSERT(ranges::Semiregular<copyable>());
CONCEPT_ASSERT(!ranges::Semiregular<moveonly>());
CONCEPT_ASSERT(!ranges::Semiregular<nonmovable>());
CONCEPT_ASSERT(!ranges::Semiregular<copyonly>());
CONCEPT_ASSERT(!ranges::Semiregular<explicit_move>());
CONCEPT_ASSERT(!ranges::Semiregular<explicit_copy>());

CONCEPT_ASSERT(ranges::Regular<int>());
CONCEPT_ASSERT(ranges::Regular<double>());
CONCEPT_ASSERT(!ranges::Regular<void>());
CONCEPT_ASSERT(!ranges::Regular<int&>());
CONCEPT_ASSERT(!ranges::Regular<semiregular>());
CONCEPT_ASSERT(ranges::Regular<regular>());
CONCEPT_ASSERT(!ranges::Regular<copyable>());
CONCEPT_ASSERT(!ranges::Regular<moveonly>());
CONCEPT_ASSERT(!ranges::Regular<nonmovable>());
CONCEPT_ASSERT(!ranges::Regular<copyonly>());
CONCEPT_ASSERT(!ranges::Regular<explicit_move>());
CONCEPT_ASSERT(!ranges::Regular<explicit_copy>());

CONCEPT_ASSERT(ranges::ConstructibleObject<std::initializer_list<int>>());
CONCEPT_ASSERT(ranges::Constructible<std::initializer_list<int>>());
CONCEPT_ASSERT(ranges::DefaultConstructible<std::initializer_list<int>>());

CONCEPT_ASSERT(ranges::ConstructibleObject<int*>());
CONCEPT_ASSERT(ranges::Constructible<int*>());
CONCEPT_ASSERT(ranges::DefaultConstructible<int*>());

int main() {}
