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
#ifndef STL2_DETAIL_ITERATOR_MOVE_ITERATOR_HPP
#define STL2_DETAIL_ITERATOR_MOVE_ITERATOR_HPP

#include <stl2/type_traits.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/operator_arrow.hpp>
#include <stl2/detail/iterator/concepts.hpp>

STL2_OPEN_NAMESPACE {
  template <WeakInputIterator I>
    // requires Same<reference_t<I>, value_type_t<I>&>()
  class move_iterator {
  public:
    using iterator_type = I;
    using difference_type = difference_type_t<I>;
    using value_type = value_type_t<I>;
    using iterator_category = iterator_category_t<I>;
    using reference = rvalue_reference_t<I>;
    using pointer = I;
  
    move_iterator() = default;
    explicit move_iterator(I i)
      noexcept(is_nothrow_move_constructible<I>::value) :
      current_{__stl2::move(i)} {}
  
    move_iterator(const move_iterator<ConvertibleTo<I> >& u) :
      current_{u.base()} {}
  
    move_iterator& operator=(const move_iterator<ConvertibleTo<I> >& u) & {
      current_ = u.base();
      return *this;
    }
  
    iterator_type base() const
      noexcept(is_nothrow_copy_constructible<I>::value) {
      return current_;
    }
  
    reference operator*() const
      noexcept(is_nothrow_move_constructible<rvalue_reference_t<I>>::value &&
               noexcept(__stl2::iter_move(declval<I&>()))) {
      return __stl2::iter_move(current_);
    }
  
    // Extension
    STL2_CONSTEXPR_EXT decltype(auto) operator->() const {
      return __stl2::__operator_arrow(current_);
    }
  
    move_iterator& operator++() &
      noexcept(noexcept(++declval<I&>())) {
      ++current_;
      return *this;
    }
    move_iterator operator++(int) & {
      auto tmp = *this;
      ++*this;
      return tmp;
    }
  
    move_iterator& operator--() &
      noexcept(noexcept(--declval<I&>()))
      requires BidirectionalIterator<I>() {
      --current_;
      return *this;
    }
    move_iterator operator--(int) &
      requires BidirectionalIterator<I>() {
      auto tmp = *this;
      --*this;
      return tmp;
    }
  
    move_iterator operator+(difference_type n) const
      requires RandomAccessIterator<I>() {
      return move_iterator{current_ + n};
    }
    move_iterator& operator+=(difference_type n) &
      requires RandomAccessIterator<I>() {
      current_ += n;
      return *this;
    }
  
    move_iterator operator-(difference_type n) const
      requires RandomAccessIterator<I>() {
      return move_iterator{current_ - n};
    }
    move_iterator& operator-=(difference_type n) &
      requires RandomAccessIterator<I>() {
      current_ -= n;
      return *this;
    }
  
    reference operator[](difference_type n) const
      requires RandomAccessIterator<I>() {
      return __stl2::iter_move(current_ + n);
    }
  
    EqualityComparable{I1, I2}
    friend bool operator==(const move_iterator<I1>&,
                           const move_iterator<I2>&);
  
    StrictTotallyOrdered{I1, I2}
    friend bool operator<(const move_iterator<I1>&,
                          const move_iterator<I2>&);
  
    SizedIteratorRange{I1, I2}
    friend difference_type_t<I2> operator-(const move_iterator<I1>&,
                                        const move_iterator<I2>&);
  
  private:
    I current_{};
  };
  
  EqualityComparable{I1, I2}
  bool operator==(const move_iterator<I1>& a,
                  const move_iterator<I2>& b) {
    return a.current_ == b.current_;
  }
  
  EqualityComparable{I1, I2}
  bool operator!=(const move_iterator<I1>& a,
                  const move_iterator<I2>& b) {
    return !(a == b);
  }
  
  StrictTotallyOrdered{I1, I2}
  bool operator<(const move_iterator<I1>& a,
                 const move_iterator<I2>& b) {
    return a.current_ < b.current_;
  }
  
  StrictTotallyOrdered{I1, I2}
  bool operator>(const move_iterator<I1>& a,
                 const move_iterator<I2>& b) {
    return b < a;
  }
  
  StrictTotallyOrdered{I1, I2}
  bool operator<=(const move_iterator<I1>& a,
                  const move_iterator<I2>& b) {
    return !(b < a);
  }
  
  StrictTotallyOrdered{I1, I2}
  bool operator>=(const move_iterator<I1>& a,
                  const move_iterator<I2>& b) {
    return !(a < b);
  }
  
  SizedIteratorRange{I1, I2}
  difference_type_t<I2> operator-(const move_iterator<I1>& a,
                               const move_iterator<I2>& b) {
    return a.current_ - b.current_;
  }
  
  RandomAccessIterator{I}
  move_iterator<I> operator+(difference_type_t<I> n, const move_iterator<I>& x) {
    return x + n;
  }
  
  WeakInputIterator{I}
  auto make_move_iterator(I i) {
    return move_iterator<I>{__stl2::move(i)};
  }
} STL2_CLOSE_NAMESPACE

#endif
