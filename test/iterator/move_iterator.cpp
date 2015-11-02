// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//  Copyright Eric Niebler 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#include <stl2/iterator.hpp>
#include <stl2/functional.hpp>
#include <vector>
#include "../simple_test.hpp"

struct A {
  static std::size_t copy_count;
  static std::size_t move_count;

  static void clear() { copy_count = move_count = 0; }

  A() = default;
  A(const A&) { ++copy_count; }
  A(A&&) noexcept { ++move_count; }
  A& operator=(const A&) & { ++copy_count; return *this; }
  A& operator=(A&&) & noexcept { ++move_count; return *this; }
};

std::size_t A::copy_count;
std::size_t A::move_count;

void test_move_iterator() {
  static constexpr std::size_t N = 42;
  std::vector<A> vec(N);
  std::vector<A> vec2;
  vec2.reserve(__stl2::size(vec));

  A::clear();

  {
    auto first = __stl2::make_move_iterator(__stl2::begin(vec)),
      last = __stl2::make_move_iterator(__stl2::end(vec));
    auto out = __stl2::back_inserter(vec2);

    for (; first != last; ++first, ++out) {
      *out = *first;
    }
  }

  CHECK(__stl2::size(vec2) == N);
  CHECK(A::copy_count == std::size_t{0});
  CHECK(A::move_count == N);
}

void test_iter_move() {
  static constexpr std::size_t N = 42;
  std::vector<A> vec(N);
  std::vector<A> vec2;
  vec2.reserve(__stl2::size(vec));

  A::clear();

  {
    auto first = __stl2::begin(vec),
      last = __stl2::end(vec);
    auto out = __stl2::back_inserter(vec2);

    for (; first != last; ++first, ++out) {
      *out = __stl2::iter_move(first);
    }
  }

  CHECK(__stl2::size(vec2) == N);
  CHECK(A::copy_count == std::size_t{0});
  CHECK(A::move_count == N);
}

void test_both() {
  static constexpr std::size_t N = 42;
  std::vector<A> vec(N);
  std::vector<A> vec2;
  vec2.reserve(__stl2::size(vec));

  A::clear();

  {
    auto first = __stl2::make_move_iterator(__stl2::begin(vec)),
      last = __stl2::make_move_iterator(__stl2::end(vec));
    auto out = __stl2::back_inserter(vec2);

    for (; first != last; ++first, ++out) {
      *out = __stl2::iter_move(first);
    }
  }

  CHECK(__stl2::size(vec2) == N);
  CHECK(A::copy_count == std::size_t{0});
  CHECK(A::move_count == N);
}

template <class T>
class proxy_iterator {
public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using iterator_category = __stl2::random_access_iterator_tag;

  proxy_iterator() = default;
  explicit proxy_iterator(T* p) :
    ptr_{p} {}

  __stl2::reference_wrapper<T> operator*() const {
    return __stl2::reference_wrapper<T>{*ptr_};
  }

  bool operator==(const proxy_iterator& that) const {
    return ptr_ == that.ptr_;
  }
  bool operator!=(const proxy_iterator& that) const {
    return !(*this == that);
  }

  proxy_iterator& operator++() & {
    ++ptr_;
    return *this;
  }
  proxy_iterator operator++(int) & {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  friend T&& iter_move(proxy_iterator p) {
    return __stl2::move(*p.ptr_);
  }

private:
  T* ptr_;
};

void test_proxy_iterator() {
  static constexpr std::size_t N = 42;
  std::vector<A> vec(N);
  std::vector<A> vec2;
  vec2.reserve(__stl2::size(vec));

  A::clear();

  {
    auto first = __stl2::make_move_iterator(proxy_iterator<A>{__stl2::data(vec)}),
      last = __stl2::make_move_iterator(proxy_iterator<A>{__stl2::data(vec) + __stl2::size(vec)});
    auto out = __stl2::back_inserter(vec2);

    for (; first != last; ++first, ++out) {
      *out = *first;
    }
  }

  CHECK(__stl2::size(vec2) == N);
  CHECK(A::copy_count == std::size_t{0});
  CHECK(A::move_count == N);
}

template <class> class show_type;

void test_operator_arrow() {
  struct A {
    int value = 42;
    A() = default;
    A(A&& that) :
      value{__stl2::exchange(that.value, 0)} {}
    A(const A&) = delete;
    A(int v) : value{v} {}
    A& operator=(A&& that) & {
      value = __stl2::exchange(that.value, 0);
      return *this;
    }
    A& operator=(const A&) & = delete;
  };
  A a{42};
  A b{0};
  auto i = __stl2::make_move_iterator(&a);
  CHECK(i->value == 42);
  b = *i;
  CHECK(i->value == 0);
  CHECK(b.value == 42);
}

int main() {
  test_move_iterator();
  test_iter_move();
  test_both();
  test_proxy_iterator();
  test_operator_arrow();
  return ::test_result();
}
