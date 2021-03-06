// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
//  Copyright 2005 - 2007 Adobe Systems Incorporated
//  Distributed under the MIT License(see accompanying file LICENSE_1_0_0.txt
//  or a copy at http://stlab.adobe.com/licenses.html)

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// Implementation based on the code in libc++
//   http://http://libcxx.llvm.org/

#include <stl2/detail/algorithm/unique_copy.hpp>
#include <cstring>
#include "../simple_test.hpp"
#include "../test_utils.hpp"
#include "../test_iterators.hpp"

namespace stl2 = __stl2;

struct count_equal
{
	static unsigned count;
	template <class T>
	bool operator()(const T& x, const T& y)
		{++count; return x == y;}
};

unsigned count_equal::count = 0;

template <class InIter, class OutIter, typename Sent = InIter>
void
test_iter()
{
	const int ia[] = {0};
	const unsigned sa = sizeof(ia)/sizeof(ia[0]);
	int ja[sa] = {-1};
	count_equal::count = 0;
	std::pair<InIter, OutIter> r = stl2::unique_copy(InIter(ia), Sent(ia+sa), OutIter(ja), count_equal());
	CHECK(base(r.first) == ia + sa);
	CHECK(base(r.second) == ja + sa);
	CHECK(ja[0] == 0);
	CHECK(count_equal::count == sa-1);

	const int ib[] = {0, 1};
	const unsigned sb = sizeof(ib)/sizeof(ib[0]);
	int jb[sb] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(InIter(ib), Sent(ib+sb), OutIter(jb), count_equal());
	CHECK(base(r.first) == ib + sb);
	CHECK(base(r.second) == jb + sb);
	CHECK(jb[0] == 0);
	CHECK(jb[1] == 1);
	CHECK(count_equal::count == sb-1);

	const int ic[] = {0, 0};
	const unsigned sc = sizeof(ic)/sizeof(ic[0]);
	int jc[sc] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(InIter(ic), Sent(ic+sc), OutIter(jc), count_equal());
	CHECK(base(r.first) == ic + sc);
	CHECK(base(r.second) == jc + 1);
	CHECK(jc[0] == 0);
	CHECK(count_equal::count == sc-1);

	const int id[] = {0, 0, 1};
	const unsigned sd = sizeof(id)/sizeof(id[0]);
	int jd[sd] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(InIter(id), Sent(id+sd), OutIter(jd), count_equal());
	CHECK(base(r.first) == id + sd);
	CHECK(base(r.second) == jd + 2);
	CHECK(jd[0] == 0);
	CHECK(jd[1] == 1);
	CHECK(count_equal::count == sd-1);

	const int ie[] = {0, 0, 1, 0};
	const unsigned se = sizeof(ie)/sizeof(ie[0]);
	int je[se] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(InIter(ie), Sent(ie+se), OutIter(je), count_equal());
	CHECK(base(r.first) == ie + se);
	CHECK(base(r.second) == je + 3);
	CHECK(je[0] == 0);
	CHECK(je[1] == 1);
	CHECK(je[2] == 0);
	CHECK(count_equal::count == se-1);

	const int ig[] = {0, 0, 1, 1};
	const unsigned sg = sizeof(ig)/sizeof(ig[0]);
	int jg[sg] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(InIter(ig), Sent(ig+sg), OutIter(jg), count_equal());
	CHECK(base(r.first) == ig + sg);
	CHECK(base(r.second) == jg + 2);
	CHECK(jg[0] == 0);
	CHECK(jg[1] == 1);
	CHECK(count_equal::count == sg-1);

	const int ih[] = {0, 1, 1};
	const unsigned sh = sizeof(ih)/sizeof(ih[0]);
	int jh[sh] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(InIter(ih), Sent(ih+sh), OutIter(jh), count_equal());
	CHECK(base(r.first) == ih + sh);
	CHECK(base(r.second) == jh + 2);
	CHECK(jh[0] == 0);
	CHECK(jh[1] == 1);
	CHECK(count_equal::count == sh-1);

	const int ii[] = {0, 1, 1, 1, 2, 2, 2};
	const unsigned si = sizeof(ii)/sizeof(ii[0]);
	int ji[si] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(InIter(ii), Sent(ii+si), OutIter(ji), count_equal());
	CHECK(base(r.first) == ii + si);
	CHECK(base(r.second) == ji + 3);
	CHECK(ji[0] == 0);
	CHECK(ji[1] == 1);
	CHECK(ji[2] == 2);
	CHECK(count_equal::count == si-1);
}

template <class InIter, class OutIter, typename Sent = InIter>
void
test_range()
{
	const int ia[] = {0};
	const unsigned sa = sizeof(ia)/sizeof(ia[0]);
	int ja[sa] = {-1};
	count_equal::count = 0;
	std::pair<InIter, OutIter> r = stl2::unique_copy(::as_lvalue(stl2::ext::make_range(InIter(ia), Sent(ia+sa))), OutIter(ja), count_equal());
	CHECK(base(r.first) == ia + sa);
	CHECK(base(r.second) == ja + sa);
	CHECK(ja[0] == 0);
	CHECK(count_equal::count == sa-1);

	const int ib[] = {0, 1};
	const unsigned sb = sizeof(ib)/sizeof(ib[0]);
	int jb[sb] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(::as_lvalue(stl2::ext::make_range(InIter(ib), Sent(ib+sb))), OutIter(jb), count_equal());
	CHECK(base(r.first) == ib + sb);
	CHECK(base(r.second) == jb + sb);
	CHECK(jb[0] == 0);
	CHECK(jb[1] == 1);
	CHECK(count_equal::count == sb-1);

	const int ic[] = {0, 0};
	const unsigned sc = sizeof(ic)/sizeof(ic[0]);
	int jc[sc] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(::as_lvalue(stl2::ext::make_range(InIter(ic), Sent(ic+sc))), OutIter(jc), count_equal());
	CHECK(base(r.first) == ic + sc);
	CHECK(base(r.second) == jc + 1);
	CHECK(jc[0] == 0);
	CHECK(count_equal::count == sc-1);

	const int id[] = {0, 0, 1};
	const unsigned sd = sizeof(id)/sizeof(id[0]);
	int jd[sd] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(::as_lvalue(stl2::ext::make_range(InIter(id), Sent(id+sd))), OutIter(jd), count_equal());
	CHECK(base(r.first) == id + sd);
	CHECK(base(r.second) == jd + 2);
	CHECK(jd[0] == 0);
	CHECK(jd[1] == 1);
	CHECK(count_equal::count == sd-1);

	const int ie[] = {0, 0, 1, 0};
	const unsigned se = sizeof(ie)/sizeof(ie[0]);
	int je[se] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(::as_lvalue(stl2::ext::make_range(InIter(ie), Sent(ie+se))), OutIter(je), count_equal());
	CHECK(base(r.first) == ie + se);
	CHECK(base(r.second) == je + 3);
	CHECK(je[0] == 0);
	CHECK(je[1] == 1);
	CHECK(je[2] == 0);
	CHECK(count_equal::count == se-1);

	const int ig[] = {0, 0, 1, 1};
	const unsigned sg = sizeof(ig)/sizeof(ig[0]);
	int jg[sg] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(::as_lvalue(stl2::ext::make_range(InIter(ig), Sent(ig+sg))), OutIter(jg), count_equal());
	CHECK(base(r.first) == ig + sg);
	CHECK(base(r.second) == jg + 2);
	CHECK(jg[0] == 0);
	CHECK(jg[1] == 1);
	CHECK(count_equal::count == sg-1);

	const int ih[] = {0, 1, 1};
	const unsigned sh = sizeof(ih)/sizeof(ih[0]);
	int jh[sh] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(::as_lvalue(stl2::ext::make_range(InIter(ih), Sent(ih+sh))), OutIter(jh), count_equal());
	CHECK(base(r.first) == ih + sh);
	CHECK(base(r.second) == jh + 2);
	CHECK(jh[0] == 0);
	CHECK(jh[1] == 1);
	CHECK(count_equal::count == sh-1);

	const int ii[] = {0, 1, 1, 1, 2, 2, 2};
	const unsigned si = sizeof(ii)/sizeof(ii[0]);
	int ji[si] = {-1};
	count_equal::count = 0;
	r = stl2::unique_copy(::as_lvalue(stl2::ext::make_range(InIter(ii), Sent(ii+si))), OutIter(ji), count_equal());
	CHECK(base(r.first) == ii + si);
	CHECK(base(r.second) == ji + 3);
	CHECK(ji[0] == 0);
	CHECK(ji[1] == 1);
	CHECK(ji[2] == 2);
	CHECK(count_equal::count == si-1);
}

template<class InIter, class OutIter>
void test()
{
	using Sent = typename sentinel_type<InIter>::type;
	test_iter<InIter, OutIter>();
	test_iter<InIter, OutIter, Sent>();

	test_range<InIter, OutIter>();
	test_range<InIter, OutIter, Sent>();
}

struct S
{
	int i,j;
};

bool operator==(S l, S r)
{
	return l.i == r.i && l.j == r.j;
}

int main()
{
	test<input_iterator<const int*>, output_iterator<int*> >();
	test<input_iterator<const int*>, forward_iterator<int*> >();
	test<input_iterator<const int*>, bidirectional_iterator<int*> >();
	test<input_iterator<const int*>, random_access_iterator<int*> >();
	test<input_iterator<const int*>, int*>();

	test<forward_iterator<const int*>, output_iterator<int*> >();
	test<forward_iterator<const int*>, forward_iterator<int*> >();
	test<forward_iterator<const int*>, bidirectional_iterator<int*> >();
	test<forward_iterator<const int*>, random_access_iterator<int*> >();
	test<forward_iterator<const int*>, int*>();

	test<bidirectional_iterator<const int*>, output_iterator<int*> >();
	test<bidirectional_iterator<const int*>, forward_iterator<int*> >();
	test<bidirectional_iterator<const int*>, bidirectional_iterator<int*> >();
	test<bidirectional_iterator<const int*>, random_access_iterator<int*> >();
	test<bidirectional_iterator<const int*>, int*>();

	test<random_access_iterator<const int*>, output_iterator<int*> >();
	test<random_access_iterator<const int*>, forward_iterator<int*> >();
	test<random_access_iterator<const int*>, bidirectional_iterator<int*> >();
	test<random_access_iterator<const int*>, random_access_iterator<int*> >();
	test<random_access_iterator<const int*>, int*>();

	test<const int*, output_iterator<int*> >();
	test<const int*, forward_iterator<int*> >();
	test<const int*, bidirectional_iterator<int*> >();
	test<const int*, random_access_iterator<int*> >();
	test<const int*, int*>();

	// Test projections:
	{
		S const ia[] = {{1,1},{2,2},{3,3},{3,4},{4,5},{5,6},{5,7},{5,8},{6,9},{7,10}};
		S ib[stl2::size(ia)];
		std::pair<S const *, S *> r = stl2::unique_copy(ia, ib, stl2::equal_to<>(), &S::i);
		CHECK(r.first == stl2::end(ia));
		CHECK(r.second == ib + 7);
		check_equal(stl2::ext::make_range(ib, ib+7), {S{1,1},S{2,2},S{3,3},S{4,5},S{5,6},S{6,9},S{7,10}});
	}

	// Test rvalue ranges:
	{
		S const ia[] = {{1,1},{2,2},{3,3},{3,4},{4,5},{5,6},{5,7},{5,8},{6,9},{7,10}};
		S ib[stl2::size(ia)];
		auto r = stl2::unique_copy(stl2::move(ia), ib, stl2::equal_to<>(), &S::i);
		CHECK(r.first.get_unsafe() == stl2::end(ia));
		CHECK(r.second == ib + 7);
		check_equal(stl2::ext::make_range(ib, ib+7), {S{1,1},S{2,2},S{3,3},S{4,5},S{5,6},S{6,9},S{7,10}});
	}

	return ::test_result();
}
