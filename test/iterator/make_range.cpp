#include <stl2/iterator.hpp>
#include "../simple_test.hpp"

using namespace __stl2;

int main() {
	{
		using I = ext::range<int*, int*>;
		using CI = ext::range<const int*, const int*>;
		static_assert(View<I>());
		static_assert(SizedRange<I>());
		static_assert(ext::ContiguousRange<I>());
		static_assert(BoundedRange<I>());
		static_assert(View<CI>());
		static_assert(SizedRange<CI>());
		static_assert(ext::ContiguousRange<CI>());
		static_assert(BoundedRange<CI>());
	}

	{
		static constexpr int some_ints[] = {2, 3, 5, 7, 11, 13};
		static constexpr std::size_t n = size(some_ints);
		auto r = ext::make_range(some_ints + 0, some_ints + n);
		using R = decltype(r);
		static_assert(View<R>());
		static_assert(SizedRange<R>());
		static_assert(ext::ContiguousRange<R>());
		static_assert(BoundedRange<R>());

		CHECK(begin(r) == some_ints + 0);
		CHECK(end(r) == some_ints + n);
		CHECK(!empty(r));
		CHECK(std::size_t(size(r)) == n);
		CHECK(data(r) == some_ints);
	}

	return test_result();
}
