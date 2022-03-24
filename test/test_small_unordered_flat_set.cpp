//
// g++ -Wall -Wextra -Wpedantic -std=c++11 -g -I ../include -o test.out test_small_unordered_flat_set.cpp
// valgrind --leak-check=full ./test.out
//

#undef NDEBUG
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include <sfl/small_unordered_flat_set.hpp>

#include "MyInt.hpp"
#include "Person.hpp"
#include "test_allocator_1.hpp"
#include "test_allocator_2.hpp"
#include "test_allocator_3.hpp"
#include "test_allocator_4.hpp"

#ifndef SFL_TEST_ALLOCATOR
#define SFL_TEST_ALLOCATOR std::allocator
//#define SFL_TEST_ALLOCATOR sfl::test_allocator_1
//#define SFL_TEST_ALLOCATOR sfl::test_allocator_2
//#define SFL_TEST_ALLOCATOR sfl::test_allocator_3
//#define SFL_TEST_ALLOCATOR sfl::test_allocator_4
#endif

int main()
{
    using std::cout;
    using std::endl;

    cout << "Create empty containers and test all non-modifying "
            "member functions." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1;

        const sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s2;

        sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s3;

        const sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s4;

        assert(s1.empty());
        assert(s2.empty());
        assert(s3.empty());
        assert(s4.empty());

        assert(s1.size() == 0);
        assert(s2.size() == 0);
        assert(s3.size() == 0);
        assert(s4.size() == 0);

        assert(s1.capacity() == 0);
        assert(s2.capacity() == 0);
        assert(s3.capacity() == 5);
        assert(s4.capacity() == 5);

        assert(s1.begin() == s1.end());
        assert(s2.begin() == s2.end());
        assert(s3.begin() == s3.end());
        assert(s4.begin() == s4.end());

        assert(s1.cbegin() == s1.cend());
        assert(s2.cbegin() == s2.cend());
        assert(s3.cbegin() == s3.cend());
        assert(s4.cbegin() == s4.cend());

        assert(s1.begin() == s1.cbegin());
        assert(s2.begin() == s2.cbegin());
        assert(s3.begin() == s3.cbegin());
        assert(s4.begin() == s4.cbegin());

        assert(s1.begin() == s1.nth(0));
        assert(s2.begin() == s2.nth(0));
        assert(s3.begin() == s3.nth(0));
        assert(s4.begin() == s4.nth(0));

        assert(s1.cbegin() == s1.nth(0));
        assert(s2.cbegin() == s2.nth(0));
        assert(s3.cbegin() == s3.nth(0));
        assert(s4.cbegin() == s4.nth(0));

        assert(s1.index_of(s1.nth(0)) == 0);
        assert(s2.index_of(s2.nth(0)) == 0);
        assert(s3.index_of(s3.nth(0)) == 0);
        assert(s4.index_of(s4.nth(0)) == 0);

        assert(s1.find(42) == s1.end());
        assert(s2.find(42) == s2.end());
        assert(s3.find(42) == s3.end());
        assert(s4.find(42) == s4.end());

        assert(s1.count(42) == 0);
        assert(s2.count(42) == 0);
        assert(s3.count(42) == 0);
        assert(s4.count(42) == 0);

        assert(s1.contains(42) == false);
        assert(s2.contains(42) == false);
        assert(s3.contains(42) == false);
        assert(s4.contains(42) == false);

        auto a1 = s1.get_allocator(); (void)a1;
        auto a2 = s2.get_allocator(); (void)a2;
        auto a3 = s3.get_allocator(); (void)a3;
        auto a4 = s4.get_allocator(); (void)a4;

        const MyInt i(10);
        const MyInt j(20);

        auto keq1 = s1.key_eq(); assert(keq1(i, i)); assert(!keq1(i, j));
        auto keq2 = s2.key_eq(); assert(keq2(i, i)); assert(!keq2(i, j));
        auto keq3 = s3.key_eq(); assert(keq3(i, i)); assert(!keq3(i, j));
        auto keq4 = s4.key_eq(); assert(keq4(i, i)); assert(!keq4(i, j));
    }

    cout << "Create empty containers and test all lookup functions "
            "that support transparent lookup." << endl;
    {
        sfl::small_unordered_flat_set<Person, 0, PersonEqual,
            SFL_TEST_ALLOCATOR<Person>
        > s1;

        const sfl::small_unordered_flat_set<Person, 0, PersonEqual,
            SFL_TEST_ALLOCATOR<Person>
        > s2;

        Person p(42, "John");

        assert(s1.find(p) == s1.end());
        assert(s2.find(p) == s1.end());

        assert(s1.find(42) == s1.end());
        assert(s2.find(42) == s1.end());

        assert(s1.count(p) == 0);
        assert(s2.count(p) == 0);

        assert(s1.count(42) == 0);
        assert(s2.count(42) == 0);

        assert(s1.contains(p) == false);
        assert(s2.contains(p) == false);

        assert(s1.contains(42) == false);
        assert(s2.contains(42) == false);
    }

    cout << "Test emplace(Args&&...) (N > 0, with reallocation)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 3, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            cout << ">" << endl;
            auto res = s.emplace(10);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), true));
            assert(s.size() == 1);
            assert(s.capacity() == 3);
            assert(*s.nth(0) == 10);
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(10);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), false));
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(20);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(1), true));
            assert(s.size() == 2);
            assert(s.capacity() == 3);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 20);
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(20);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(1), false));
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(15);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(2), true));
            assert(s.size() == 3);
            assert(s.capacity() == 3);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 15);
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(15);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(2), false));
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(30);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), true));
            assert(s.size() == 4);
            assert(s.capacity() == 6);
            assert(*s.nth(0) == 30);
            assert(*s.nth(1) == 10);
            assert(*s.nth(2) == 20);
            assert(*s.nth(3) == 15);
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(30);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), false));
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(16);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(4), true));
            assert(s.size() == 5);
            assert(s.capacity() == 6);
            assert(*s.nth(0) == 30);
            assert(*s.nth(1) == 10);
            assert(*s.nth(2) == 20);
            assert(*s.nth(3) == 15);
            assert(*s.nth(4) == 16);
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(16);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(4), false));
        }
    }

    cout << "Test emplace(Args&&...) (N == 0, with reallocation)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            cout << ">" << endl;
            auto res = s.emplace(10);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), true));
            assert(s.size() == 1);
            assert(s.capacity() == 1);
            assert(*s.nth(0) == 10);
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(10);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), false));
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(20);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), true));
            assert(s.size() == 2);
            assert(s.capacity() == 2);
            assert(*s.nth(0) == 20);
            assert(*s.nth(1) == 10);
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(20);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), false));
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(15);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), true));
            assert(s.size() == 3);
            assert(s.capacity() == 4);
            assert(*s.nth(0) == 15);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 10);
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(15);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), false));
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(30);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(3), true));
            assert(s.size() == 4);
            assert(s.capacity() == 4);
            assert(*s.nth(0) == 15);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 10);
            assert(*s.nth(3) == 30);
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(30);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(3), false));
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(16);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), true));
            assert(s.size() == 5);
            assert(s.capacity() == 8);
            assert(*s.nth(0) == 16);
            assert(*s.nth(1) == 15);
            assert(*s.nth(2) == 20);
            assert(*s.nth(3) == 10);
            assert(*s.nth(4) == 30);
        }

        {
            cout << ">" << endl;
            auto res = s.emplace(16);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), false));
        }
    }

    cout << "Test non-modifying member functions on non-empty container (1)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        s.emplace(10);
        s.emplace(20);
        s.emplace(30);

        {
            auto it = s.begin();
            assert(*it == 10); ++it;
            assert(*it == 20); ++it;
            assert(*it == 30); ++it;
            assert(it == s.end());
        }

        {
            auto it = s.cbegin();
            assert(*it == 10); ++it;
            assert(*it == 20); ++it;
            assert(*it == 30); ++it;
            assert(it == s.cend());
        }

        {
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 30);
            assert(s.nth(3) == s.end());
        }

        {
            assert(std::next(s.begin(), 0) == s.nth(0));
            assert(std::next(s.begin(), 1) == s.nth(1));
            assert(std::next(s.begin(), 2) == s.nth(2));
        }

        {
            assert(std::next(s.cbegin(), 0) == s.nth(0));
            assert(std::next(s.cbegin(), 1) == s.nth(1));
            assert(std::next(s.cbegin(), 2) == s.nth(2));
        }

        {
            assert(s.index_of(s.nth(0)) == 0);
            assert(s.index_of(s.nth(1)) == 1);
            assert(s.index_of(s.nth(2)) == 2);
        }

        {
            auto data = s.data();
            assert(*data == 10); ++data;
            assert(*data == 20); ++data;
            assert(*data == 30); ++data;
        }

        {
            assert(s.find(10) == s.nth(0));
            assert(s.find(20) == s.nth(1));
            assert(s.find(30) == s.nth(2));
            assert(s.find(40) == s.end());
        }

        {
            assert(s.count(10) == 1);
            assert(s.count(20) == 1);
            assert(s.count(30) == 1);
            assert(s.count(40) == 0);
        }

        {
            assert(s.contains(10) == true);
            assert(s.contains(20) == true);
            assert(s.contains(30) == true);
            assert(s.contains(40) == false);
        }
    }

    cout << "Test non-modifying member functions on non-empty container (2)." << endl;
    {
        sfl::small_unordered_flat_set<Person, 10, PersonEqual,
            SFL_TEST_ALLOCATOR<Person>
        > s;

        s.emplace(10, "Name 10");
        s.emplace(20, "Name 20");
        s.emplace(30, "Name 30");

        {
            auto it = s.begin();
            assert(it->id() == 10 && it->name() == "Name 10"); ++it;
            assert(it->id() == 20 && it->name() == "Name 20"); ++it;
            assert(it->id() == 30 && it->name() == "Name 30"); ++it;
            assert(it == s.end());
        }

        {
            auto it = s.cbegin();
            assert(it->id() == 10 && it->name() == "Name 10"); ++it;
            assert(it->id() == 20 && it->name() == "Name 20"); ++it;
            assert(it->id() == 30 && it->name() == "Name 30"); ++it;
            assert(it == s.cend());
        }

        {
            assert(std::next(s.begin(), 0) == s.nth(0));
            assert(std::next(s.begin(), 1) == s.nth(1));
            assert(std::next(s.begin(), 2) == s.nth(2));
        }

        {
            assert(std::next(s.cbegin(), 0) == s.nth(0));
            assert(std::next(s.cbegin(), 1) == s.nth(1));
            assert(std::next(s.cbegin(), 2) == s.nth(2));
        }

        {
            assert(s.index_of(s.nth(0)) == 0);
            assert(s.index_of(s.nth(1)) == 1);
            assert(s.index_of(s.nth(2)) == 2);
        }

        {
            auto data = s.data();
            assert(data->id() == 10 && data->name() == "Name 10"); ++data;
            assert(data->id() == 20 && data->name() == "Name 20"); ++data;
            assert(data->id() == 30 && data->name() == "Name 30"); ++data;
        }

        {
            assert(s.find(Person(10, "xxx")) == s.nth(0));
            assert(s.find(Person(20, "xxx")) == s.nth(1));
            assert(s.find(Person(30, "xxx")) == s.nth(2));
            assert(s.find(Person(40, "xxx")) == s.end());
        }

        {
            assert(s.find(10) == s.nth(0));
            assert(s.find(20) == s.nth(1));
            assert(s.find(30) == s.nth(2));
            assert(s.find(40) == s.end());
        }

        {
            assert(s.count(Person(10, "xxx")) == 1);
            assert(s.count(Person(20, "xxx")) == 1);
            assert(s.count(Person(30, "xxx")) == 1);
            assert(s.count(Person(40, "xxx")) == 0);
        }

        {
            assert(s.count(10) == 1);
            assert(s.count(20) == 1);
            assert(s.count(30) == 1);
            assert(s.count(40) == 0);
        }

        {
            assert(s.contains(Person(10, "xxx")) == true);
            assert(s.contains(Person(20, "xxx")) == true);
            assert(s.contains(Person(30, "xxx")) == true);
            assert(s.contains(Person(40, "xxx")) == false);
        }

        {
            assert(s.contains(10) == true);
            assert(s.contains(20) == true);
            assert(s.contains(30) == true);
            assert(s.contains(40) == false);
        }
    }

    cout << "Test emplace_hint(const_iterator, Args&&...)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            cout << ">" << endl;
            // hint is ignored
            auto res = s.emplace_hint(s.nth(0), 10);
            cout << "<" << endl;

            assert(res == s.nth(0));
            assert(s.size() == 1);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 10);
        }

        {
            cout << ">" << endl;
            // hint is ignored
            auto res = s.emplace_hint(s.nth(0), 10);
            cout << "<" << endl;

            assert(res == s.nth(0));
            assert(s.size() == 1);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 10);
        }
    }

    cout << "Test insert(const value_type&)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            const MyInt v(10);

            cout << ">" << endl;
            auto res = s.insert(v);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), true));
            assert(s.size() == 1);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 10);
        }

        {
            const MyInt v(10);

            cout << ">" << endl;
            auto res = s.insert(v);
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), false));
            assert(s.size() == 1);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 10);
        }
    }

    cout << "Test insert(value_type&&)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            MyInt v(10);

            cout << ">" << endl;
            auto res = s.insert(std::move(v));
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), true));
            assert(v == -10);
            assert(s.size() == 1);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 10);
        }

        {
            MyInt v(10);

            cout << ">" << endl;
            auto res = s.insert(std::move(v));
            cout << "<" << endl;

            assert(res == std::make_pair(s.nth(0), false));
            assert(v == 10);
            assert(s.size() == 1);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 10);
        }
    }

    cout << "Test insert(const_iterator, const value_type&)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            const MyInt v(10);

            cout << ">" << endl;
            // hint is ignored
            auto res = s.insert(s.nth(0), v);
            cout << "<" << endl;

            assert(res == s.nth(0));
            assert(s.size() == 1);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 10);
        }

        {
            const MyInt v(10);

            cout << ">" << endl;
            // hint is ignored
            auto res = s.insert(s.nth(0), v);
            cout << "<" << endl;

            assert(res == s.nth(0));
            assert(s.size() == 1);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 10);
        }
    }

    cout << "Test insert(const_iterator, value_type&&)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            MyInt v(10);

            cout << ">" << endl;
            // hint is ignored
            auto res = s.insert(s.nth(0), std::move(v));
            cout << "<" << endl;

            assert(res == s.nth(0));
            assert(v == -10);
            assert(s.size() == 1);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 10);
        }

        {
            MyInt v(10);

            cout << ">" << endl;
            // hint is ignored
            auto res = s.insert(s.nth(0), std::move(v));
            cout << "<" << endl;

            assert(res == s.nth(0));
            assert(v == 10);
            assert(s.size() == 1);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 10);
        }
    }

    cout << "Test insert(InputIt, InputIt)." << endl;
    {
        const std::vector<MyInt> v({10, 10, 20, 20, 15, 15, 30, 30});;

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        cout << ">" << endl;
        s.insert(v.begin(), v.end());
        cout << "<" << endl;

        assert(s.size() == 4);
        assert(s.capacity() == 10);
        assert(*s.nth(0) == 10);
        assert(*s.nth(1) == 20);
        assert(*s.nth(2) == 15);
        assert(*s.nth(3) == 30);
    }

    cout << "Test insert(std::initializer_list)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        cout << ">" << endl;
        s.insert({10, 10, 20, 20, 15, 15, 30, 30});
        cout << "<" << endl;

        assert(s.size() == 4);
        assert(s.capacity() == 10);
        assert(*s.nth(0) == 10);
        assert(*s.nth(1) == 20);
        assert(*s.nth(2) == 15);
        assert(*s.nth(3) == 30);
    }

    cout << "Test clear()." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            cout << ">" << endl;
            s.emplace(10);
            s.emplace(20);
            s.emplace(30);
            s.emplace(40);
            cout << "<" << endl;

            assert(s.size() == 4);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 30);
            assert(*s.nth(3) == 40);
        }

        {
            cout << ">" << endl;
            s.clear();
            cout << "<" << endl;

            assert(s.size() == 0);
        }

        {
            cout << ">" << endl;
            s.emplace(50);
            s.emplace(60);
            cout << "<" << endl;

            assert(s.size() == 2);
            assert(*s.nth(0) == 50);
            assert(*s.nth(1) == 60);
        }

        {
            cout << ">" << endl;
            s.clear();
            cout << "<" << endl;

            assert(s.size() == 0);
        }
    }

    cout << "Test erase(const_iterator)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            cout << ">" << endl;
            s.emplace(10);
            s.emplace(20);
            s.emplace(30);
            s.emplace(40);
            cout << "<" << endl;

            assert(s.size() == 4);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 30);
            assert(*s.nth(3) == 40);
        }

        {
            cout << ">" << endl;
            auto res = s.erase(s.nth(1));
            cout << "<" << endl;

            assert(res == s.nth(1));
            assert(s.size() == 3);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 40);
            assert(*s.nth(2) == 30);
        }

        {
            cout << ">" << endl;
            auto res = s.erase(s.nth(0));
            cout << "<" << endl;

            assert(res == s.nth(0));
            assert(s.size() == 2);
            assert(*s.nth(0) == 30);
            assert(*s.nth(1) == 40);
        }

        {
            cout << ">" << endl;
            auto res = s.erase(s.nth(1));
            cout << "<" << endl;

            assert(res == s.nth(1));
            assert(s.size() == 1);
            assert(*s.nth(0) == 30);
        }

        {
            cout << ">" << endl;
            auto res = s.erase(s.nth(0));
            cout << "<" << endl;

            assert(res == s.nth(0));
            assert(s.size() == 0);
        }
    }

    cout << "Test erase(const_iterator, const_iterator)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            cout << ">" << endl;
            s.emplace(10);
            s.emplace(20);
            s.emplace(30);
            s.emplace(40);
            s.emplace(50);
            s.emplace(60);
            s.emplace(70);
            s.emplace(80);
            s.emplace(90);
            cout << "<" << endl;

            assert(s.size() == 9);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 30);
            assert(*s.nth(3) == 40);
            assert(*s.nth(4) == 50);
            assert(*s.nth(5) == 60);
            assert(*s.nth(6) == 70);
            assert(*s.nth(7) == 80);
            assert(*s.nth(8) == 90);
        }

        {
            cout << ">" << endl;
            // Erase nothing
            auto res = s.erase(s.nth(3), s.nth(3));
            cout << "<" << endl;

            assert(res == s.nth(3));
            assert(s.size() == 9);
        }

        {
            cout << ">" << endl;
            // Erase two elements near begin
            auto res = s.erase(s.nth(1), s.nth(3));
            cout << "<" << endl;

            assert(res == s.nth(1));
            assert(s.size() == 7);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 80);
            assert(*s.nth(2) == 90);
            assert(*s.nth(3) == 40);
            assert(*s.nth(4) == 50);
            assert(*s.nth(5) == 60);
            assert(*s.nth(6) == 70);
        }

        {
            cout << ">" << endl;
            // Erase two elements near end
            auto res = s.erase(s.nth(4), s.nth(6));
            cout << "<" << endl;

            assert(res == s.nth(4));
            assert(s.size() == 5);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 80);
            assert(*s.nth(2) == 90);
            assert(*s.nth(3) == 40);
            assert(*s.nth(4) == 70);
        }

        {
            cout << ">" << endl;
            // Remove two elements at begin
            auto res = s.erase(s.nth(0), s.nth(2));
            cout << "<" << endl;

            assert(res == s.nth(0));
            assert(s.size() == 3);
            assert(*s.nth(0) == 40);
            assert(*s.nth(1) == 70);
            assert(*s.nth(2) == 90);
        }

        {
            cout << ">" << endl;
            // Remove two elements at end
            auto res = s.erase(s.nth(1), s.nth(3));
            cout << "<" << endl;

            assert(res == s.nth(1));
            assert(s.size() == 1);
            assert(*s.nth(0) == 40);
        }

        {
            cout << ">" << endl;
            // Remove from begin to end
            auto res = s.erase(s.nth(0), s.nth(1));
            cout << "<" << endl;

            assert(res == s.nth(0));
            assert(s.size() == 0);
        }
    }

    cout << "Test erase(const Key&)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            cout << ">" << endl;
            s.emplace(10);
            s.emplace(20);
            s.emplace(30);
            s.emplace(40);
            cout << "<" << endl;

            assert(s.size() == 4);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 30);
            assert(*s.nth(3) == 40);
        }

        {
            const MyInt i(20);

            cout << ">" << endl;
            auto res = s.erase(i);
            cout << "<" << endl;

            assert(res == 1);
            assert(s.size() == 3);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 40);
            assert(*s.nth(2) == 30);
        }

        {
            const MyInt i(15);

            cout << ">" << endl;
            auto res = s.erase(i);
            cout << "<" << endl;

            assert(res == 0);
            assert(s.size() == 3);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 40);
            assert(*s.nth(2) == 30);
        }
    }

    cout << "Test erase(K&&) (transparent erase)." << endl;
    {
        sfl::small_unordered_flat_set<Person, 10, PersonEqual,
            SFL_TEST_ALLOCATOR<Person>
        > s;

        {
            cout << ">" << endl;
            s.emplace(10, "/10/");
            s.emplace(20, "/20/");
            s.emplace(20, "*20*");
            cout << "<" << endl;

            assert(s.size() == 2);
        }

        {
            cout << ">" << endl;
            auto res = s.erase(10);
            cout << "<" << endl;

            assert(res == 1);
            assert(s.size() == 1);
        }

        {
            cout << ">" << endl;
            auto res = s.erase(15);
            cout << "<" << endl;

            assert(res == 0);
            assert(s.size() == 1);
        }

        {
            const Person p(20, "persons are compared by ID only");

            cout << ">" << endl;
            auto res = s.erase(p);
            cout << "<" << endl;

            assert(res == 1);
            assert(s.size() == 0);
        }
    }

    cout << "Test swap(small_unordered_flat_set&)." << endl;
    {
        // s1 uses internal storage, s2 uses internal storage
        {
            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s1;

            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s2;

            s1.emplace(10);
            s1.emplace(11);
            s1.emplace(12);

            s2.emplace(20);
            s2.emplace(21);
            s2.emplace(22);
            s2.emplace(23);

            assert(s1.size() == 3);
            assert(*s1.nth(0) == 10);
            assert(*s1.nth(1) == 11);
            assert(*s1.nth(2) == 12);

            assert(s2.size() == 4);
            assert(*s2.nth(0) == 20);
            assert(*s2.nth(1) == 21);
            assert(*s2.nth(2) == 22);
            assert(*s2.nth(3) == 23);

            cout << ">" << endl;
            s1.swap(s2);
            cout << "<" << endl;

            assert(s1.size() == 4);
            assert(*s1.nth(0) == 20);
            assert(*s1.nth(1) == 21);
            assert(*s1.nth(2) == 22);
            assert(*s1.nth(3) == 23);

            assert(s2.size() == 3);
            assert(*s2.nth(0) == 10);
            assert(*s2.nth(1) == 11);
            assert(*s2.nth(2) == 12);

            cout << ">" << endl;
            s1.swap(s2);
            cout << "<" << endl;

            assert(s1.size() == 3);
            assert(*s1.nth(0) == 10);
            assert(*s1.nth(1) == 11);
            assert(*s1.nth(2) == 12);

            assert(s2.size() == 4);
            assert(*s2.nth(0) == 20);
            assert(*s2.nth(1) == 21);
            assert(*s2.nth(2) == 22);
            assert(*s2.nth(3) == 23);
        }

        // s1 uses internal storage, s2 uses external storage
        {
            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s1;

            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s2;

            s1.emplace(10);
            s1.emplace(11);
            s1.emplace(12);

            s2.emplace(20);
            s2.emplace(21);
            s2.emplace(22);
            s2.emplace(23);
            s2.emplace(24);
            s2.emplace(25);

            assert(s1.size() == 3);
            assert(*s1.nth(0) == 10);
            assert(*s1.nth(1) == 11);
            assert(*s1.nth(2) == 12);

            assert(s2.size() == 6);
            assert(*s2.nth(0) == 25);
            assert(*s2.nth(1) == 20);
            assert(*s2.nth(2) == 21);
            assert(*s2.nth(3) == 22);
            assert(*s2.nth(4) == 23);
            assert(*s2.nth(5) == 24);

            cout << ">" << endl;
            s1.swap(s2);
            cout << "<" << endl;

            assert(s1.size() == 6);
            assert(*s1.nth(0) == 25);
            assert(*s1.nth(1) == 20);
            assert(*s1.nth(2) == 21);
            assert(*s1.nth(3) == 22);
            assert(*s1.nth(4) == 23);
            assert(*s1.nth(5) == 24);

            assert(s2.size() == 3);
            assert(*s2.nth(0) == 10);
            assert(*s2.nth(1) == 11);
            assert(*s2.nth(2) == 12);
        }

        // s1 uses external storage, s2 uses internal storage
        {
            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s1;

            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s2;

            s1.emplace(10);
            s1.emplace(11);
            s1.emplace(12);
            s1.emplace(13);
            s1.emplace(14);
            s1.emplace(15);

            s2.emplace(20);
            s2.emplace(21);
            s2.emplace(22);

            assert(s1.size() == 6);
            assert(*s1.nth(0) == 15);
            assert(*s1.nth(1) == 10);
            assert(*s1.nth(2) == 11);
            assert(*s1.nth(3) == 12);
            assert(*s1.nth(4) == 13);
            assert(*s1.nth(5) == 14);

            assert(s2.size() == 3);
            assert(*s2.nth(0) == 20);
            assert(*s2.nth(1) == 21);
            assert(*s2.nth(2) == 22);

            cout << ">" << endl;
            s1.swap(s2);
            cout << "<" << endl;

            assert(s1.size() == 3);
            assert(*s1.nth(0) == 20);
            assert(*s1.nth(1) == 21);
            assert(*s1.nth(2) == 22);

            assert(s2.size() == 6);
            assert(*s2.nth(0) == 15);
            assert(*s2.nth(1) == 10);
            assert(*s2.nth(2) == 11);
            assert(*s2.nth(3) == 12);
            assert(*s2.nth(4) == 13);
            assert(*s2.nth(5) == 14);
        }

        // s1 uses external storage, s2 uses external storage
        {
            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s1;

            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s2;

            s1.emplace(10);
            s1.emplace(11);
            s1.emplace(12);
            s1.emplace(13);
            s1.emplace(14);
            s1.emplace(15);

            s2.emplace(20);
            s2.emplace(21);
            s2.emplace(22);
            s2.emplace(23);
            s2.emplace(24);
            s2.emplace(25);

            assert(s1.size() == 6);
            assert(*s1.nth(0) == 15);
            assert(*s1.nth(1) == 10);
            assert(*s1.nth(2) == 11);
            assert(*s1.nth(3) == 12);
            assert(*s1.nth(4) == 13);
            assert(*s1.nth(5) == 14);

            assert(s2.size() == 6);
            assert(*s2.nth(0) == 25);
            assert(*s2.nth(1) == 20);
            assert(*s2.nth(2) == 21);
            assert(*s2.nth(3) == 22);
            assert(*s2.nth(4) == 23);
            assert(*s2.nth(5) == 24);

            cout << ">" << endl;
            s1.swap(s2);
            cout << "<" << endl;

            assert(s1.size() == 6);
            assert(*s1.nth(0) == 25);
            assert(*s1.nth(1) == 20);
            assert(*s1.nth(2) == 21);
            assert(*s1.nth(3) == 22);
            assert(*s1.nth(4) == 23);
            assert(*s1.nth(5) == 24);

            assert(s2.size() == 6);
            assert(*s2.nth(0) == 15);
            assert(*s2.nth(1) == 10);
            assert(*s2.nth(2) == 11);
            assert(*s2.nth(3) == 12);
            assert(*s2.nth(4) == 13);
            assert(*s2.nth(5) == 14);
        }
    }

    cout << "Test reserve and shrink_to_fit (N > 0)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            cout << ">" << endl;
            s.emplace(10);
            s.emplace(20);
            s.emplace(30);
            cout << "<" << endl;

            assert(s.size() == 3);
            assert(s.capacity() == 5);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 30);
        }

        {
            cout << ">" << endl;
            s.reserve(4);
            cout << "<" << endl;

            assert(s.size() == 3);
            assert(s.capacity() == 5);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 30);
        }

        {
            cout << ">" << endl;
            s.reserve(10);
            cout << "<" << endl;

            assert(s.size() == 3);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 30);
        }

        {
            cout << ">" << endl;
            s.reserve(6);
            cout << "<" << endl;

            assert(s.size() == 3);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 30);
        }

        {
            cout << ">" << endl;
            s.shrink_to_fit();
            cout << "<" << endl;

            assert(s.size() == 3);
            assert(s.capacity() == 5);
            assert(*s.nth(0) == 10);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 30);
        }
    }

    cout << "Test reserve and shrink_to_fit (N == 0)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s;

        {
            cout << ">" << endl;
            s.emplace(10);
            s.emplace(20);
            s.emplace(30);
            cout << "<" << endl;

            assert(s.size() == 3);
            assert(s.capacity() == 4);
            assert(*s.nth(0) == 30);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 10);
        }

        {
            cout << ">" << endl;
            s.reserve(4);
            cout << "<" << endl;

            assert(s.size() == 3);
            assert(s.capacity() == 4);
            assert(*s.nth(0) == 30);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 10);
        }

        {
            cout << ">" << endl;
            s.reserve(10);
            cout << "<" << endl;

            assert(s.size() == 3);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 30);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 10);
        }

        {
            cout << ">" << endl;
            s.reserve(6);
            cout << "<" << endl;

            assert(s.size() == 3);
            assert(s.capacity() == 10);
            assert(*s.nth(0) == 30);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 10);
        }

        {
            cout << ">" << endl;
            s.shrink_to_fit();
            cout << "<" << endl;

            assert(s.size() == 3);
            assert(s.capacity() == 3);
            assert(*s.nth(0) == 30);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 10);
        }

        {
            cout << ">" << endl;
            s.clear();
            cout << "<" << endl;

            cout << ">" << endl;
            s.shrink_to_fit();
            cout << "<" << endl;

            assert(s.size() == 0);
            assert(s.capacity() == 0);
        }

        {
            cout << ">" << endl;
            s.emplace(10);
            s.emplace(20);
            s.emplace(30);
            cout << "<" << endl;

            assert(s.size() == 3);
            assert(s.capacity() == 4);
            assert(*s.nth(0) == 30);
            assert(*s.nth(1) == 20);
            assert(*s.nth(2) == 10);
        }
    }

    cout << "Test empty constructors." << endl;
    {
        std::equal_to<MyInt> eq;
        SFL_TEST_ALLOCATOR<MyInt> alloc;

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1;

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s2(eq);

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s3(alloc);

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s4(eq, alloc);

        assert(s1.size() == 0);
        assert(s1.capacity() == 10);

        assert(s2.size() == 0);
        assert(s2.capacity() == 10);

        assert(s3.size() == 0);
        assert(s3.capacity() == 10);

        assert(s4.size() == 0);
        assert(s4.capacity() == 10);
    }

    cout << "Test range constructors." << endl;
    {
        std::equal_to<MyInt> eq;
        SFL_TEST_ALLOCATOR<MyInt> alloc;

        const std::vector<MyInt> v({10, 20, 30, 10, 20});

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1(v.begin(), v.end());

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s2(v.begin(), v.end(), eq);

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s3(v.begin(), v.end(), alloc);

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s4(v.begin(), v.end(), eq, alloc);

        assert(s1.size() == 3);
        assert(s1.capacity() == 10);
        assert(*s1.nth(0) == 10);
        assert(*s1.nth(1) == 20);
        assert(*s1.nth(2) == 30);

        assert(s2.size() == 3);
        assert(s2.capacity() == 10);
        assert(*s2.nth(0) == 10);
        assert(*s2.nth(1) == 20);
        assert(*s2.nth(2) == 30);

        assert(s3.size() == 3);
        assert(s3.capacity() == 10);
        assert(*s3.nth(0) == 10);
        assert(*s3.nth(1) == 20);
        assert(*s3.nth(2) == 30);

        assert(s4.size() == 3);
        assert(s4.capacity() == 10);
        assert(*s4.nth(0) == 10);
        assert(*s4.nth(1) == 20);
        assert(*s4.nth(2) == 30);
    }

    cout << "Test initializer_list constructors." << endl;
    {
        std::equal_to<MyInt> eq;
        SFL_TEST_ALLOCATOR<MyInt> alloc;

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1({10, 20, 30, 10, 20});

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s2({10, 20, 30, 10, 20}, eq);

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s3({10, 20, 30, 10, 20}, alloc);

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s4({10, 20, 30, 10, 20}, eq, alloc);

        assert(s1.size() == 3);
        assert(s1.capacity() == 10);
        assert(*s1.nth(0) == 10);
        assert(*s1.nth(1) == 20);
        assert(*s1.nth(2) == 30);

        assert(s2.size() == 3);
        assert(s2.capacity() == 10);
        assert(*s2.nth(0) == 10);
        assert(*s2.nth(1) == 20);
        assert(*s2.nth(2) == 30);

        assert(s3.size() == 3);
        assert(s3.capacity() == 10);
        assert(*s3.nth(0) == 10);
        assert(*s3.nth(1) == 20);
        assert(*s3.nth(2) == 30);

        assert(s4.size() == 3);
        assert(s4.capacity() == 10);
        assert(*s4.nth(0) == 10);
        assert(*s4.nth(1) == 20);
        assert(*s4.nth(2) == 30);
    }

    cout << "Test copy constructors (N > 0)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1({10, 20, 30});

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s2(s1);

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s3(s1, SFL_TEST_ALLOCATOR<MyInt>());

        assert(s1.size() == 3);
        assert(s1.capacity() == 10);
        assert(*s1.nth(0) == 10);
        assert(*s1.nth(1) == 20);
        assert(*s1.nth(2) == 30);

        assert(s2.size() == 3);
        assert(s2.capacity() == 10);
        assert(*s2.nth(0) == 10);
        assert(*s2.nth(1) == 20);
        assert(*s2.nth(2) == 30);

        assert(s3.size() == 3);
        assert(s3.capacity() == 10);
        assert(*s3.nth(0) == 10);
        assert(*s3.nth(1) == 20);
        assert(*s3.nth(2) == 30);
    }

    cout << "Test copy constructors (N == 0)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1({10, 20, 30});

        sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s2(s1);

        sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s3(s1, SFL_TEST_ALLOCATOR<MyInt>());

        assert(s1.size() == 3);
        assert(s1.capacity() == 4);
        assert(*s1.nth(0) == 30);
        assert(*s1.nth(1) == 20);
        assert(*s1.nth(2) == 10);

        assert(s2.size() == 3);
        assert(s2.capacity() == 3);
        assert(*s2.nth(0) == 30);
        assert(*s2.nth(1) == 20);
        assert(*s2.nth(2) == 10);

        assert(s3.size() == 3);
        assert(s3.capacity() == 3);
        assert(*s3.nth(0) == 30);
        assert(*s3.nth(1) == 20);
        assert(*s3.nth(2) == 10);
    }

    cout << "Test move constructors (N > 0)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1({10, 20, 30});

        assert(s1.size() == 3);
        assert(s1.capacity() == 10);
        assert(*s1.nth(0) == 10);
        assert(*s1.nth(1) == 20);
        assert(*s1.nth(2) == 30);

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s2(std::move(s1));

        assert(s1.size() == 0);
        assert(s1.capacity() == 10);

        assert(s2.size() == 3);
        assert(s2.capacity() == 10);
        assert(*s2.nth(0) == 10);
        assert(*s2.nth(1) == 20);
        assert(*s2.nth(2) == 30);

        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s3(std::move(s2), SFL_TEST_ALLOCATOR<MyInt>());

        assert(s2.size() == 0);
        assert(s2.capacity() == 10);

        assert(s3.size() == 3);
        assert(s3.capacity() == 10);
        assert(*s3.nth(0) == 10);
        assert(*s3.nth(1) == 20);
        assert(*s3.nth(2) == 30);
    }

    cout << "Test move constructors (N == 0)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1({10, 20, 30});

        assert(s1.size() == 3);
        assert(s1.capacity() == 4);
        assert(*s1.nth(0) == 30);
        assert(*s1.nth(1) == 20);
        assert(*s1.nth(2) == 10);

        sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s2(std::move(s1));

        assert(s1.size() == 0);
        assert(s1.capacity() == 0);

        assert(s2.size() == 3);
        assert(s2.capacity() == 3 || s2.capacity() == 4); // Capacity depends on allocators.
        assert(*s2.nth(0) == 30);
        assert(*s2.nth(1) == 20);
        assert(*s2.nth(2) == 10);

        sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s3(std::move(s2), SFL_TEST_ALLOCATOR<MyInt>());

        assert(s2.size() == 0);
        assert(s2.capacity() == 0);

        assert(s3.size() == 3);
        assert(s3.capacity() == 3 || s3.capacity() == 4); // Capacity depends on allocators.
        assert(*s3.nth(0) == 30);
        assert(*s3.nth(1) == 20);
        assert(*s3.nth(2) == 10);
    }

    cout << "Test copy assignment operator." << endl;
    {
        // n <= capacity && n <= size
        {
            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s1({10, 11, 12});

            assert(s1.size() == 3);
            assert(s1.capacity() == 5);
            assert(*s1.nth(0) == 10);
            assert(*s1.nth(1) == 11);
            assert(*s1.nth(2) == 12);

            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s2({20, 21});

            assert(s2.size() == 2);
            assert(s2.capacity() == 5);
            assert(*s2.nth(0) == 20);
            assert(*s2.nth(1) == 21);

            cout << ">" << endl;
            s1 = s2;
            cout << "<" << endl;

            assert(s1.size() == 2);
            assert(s1.capacity() == 5);
            assert(*s1.nth(0) == 20);
            assert(*s1.nth(1) == 21);

            assert(s2.size() == 2);
            assert(s2.capacity() == 5);
            assert(*s2.nth(0) == 20);
            assert(*s2.nth(1) == 21);
        }

        // n <= capacity && n > size
        {
            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s1({10, 11, 12});

            assert(s1.size() == 3);
            assert(s1.capacity() == 5);
            assert(*s1.nth(0) == 10);
            assert(*s1.nth(1) == 11);
            assert(*s1.nth(2) == 12);

            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s2({20, 21, 22, 23, 24});

            assert(s2.size() == 5);
            assert(s2.capacity() == 5);
            assert(*s2.nth(0) == 20);
            assert(*s2.nth(1) == 21);
            assert(*s2.nth(2) == 22);
            assert(*s2.nth(3) == 23);
            assert(*s2.nth(4) == 24);

            cout << ">" << endl;
            s1 = s2;
            cout << "<" << endl;

            assert(s1.size() == 5);
            assert(s1.capacity() == 5);
            assert(*s1.nth(0) == 20);
            assert(*s1.nth(1) == 21);
            assert(*s1.nth(2) == 22);
            assert(*s1.nth(3) == 23);
            assert(*s1.nth(4) == 24);

            assert(s2.size() == 5);
            assert(s2.capacity() == 5);
            assert(*s2.nth(0) == 20);
            assert(*s2.nth(1) == 21);
            assert(*s2.nth(2) == 22);
            assert(*s2.nth(3) == 23);
            assert(*s2.nth(4) == 24);
        }

        // n > capacity
        {
            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s1({10, 11, 12});

            assert(s1.size() == 3);
            assert(s1.capacity() == 5);
            assert(*s1.nth(0) == 10);
            assert(*s1.nth(1) == 11);
            assert(*s1.nth(2) == 12);

            sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            > s2({20, 21, 22, 23, 24, 25});

            assert(s2.size() == 6);
            assert(s2.capacity() == 10);
            assert(*s2.nth(0) == 25);
            assert(*s2.nth(1) == 20);
            assert(*s2.nth(2) == 21);
            assert(*s2.nth(3) == 22);
            assert(*s2.nth(4) == 23);
            assert(*s2.nth(5) == 24);

            cout << ">" << endl;
            s1 = s2;
            cout << "<" << endl;

            assert(s1.size() == 6);
            assert(s1.capacity() == 6);
            assert(*s1.nth(0) == 25);
            assert(*s1.nth(1) == 20);
            assert(*s1.nth(2) == 21);
            assert(*s1.nth(3) == 22);
            assert(*s1.nth(4) == 23);
            assert(*s1.nth(5) == 24);

            assert(s2.size() == 6);
            assert(s2.capacity() == 10);
            assert(*s2.nth(0) == 25);
            assert(*s2.nth(1) == 20);
            assert(*s2.nth(2) == 21);
            assert(*s2.nth(3) == 22);
            assert(*s2.nth(4) == 23);
            assert(*s2.nth(5) == 24);
        }
    }

    cout << "Test move assignment operator (N > 0)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1({10, 11, 12});

        assert(s1.size() == 3);
        assert(s1.capacity() == 5);
        assert(*s1.nth(0) == 10);
        assert(*s1.nth(1) == 11);
        assert(*s1.nth(2) == 12);

        sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s2({20, 21});

        assert(s2.size() == 2);
        assert(s2.capacity() == 5);
        assert(*s2.nth(0) == 20);
        assert(*s2.nth(1) == 21);

        cout << ">" << endl;
        s1 = std::move(s2);
        cout << "<" << endl;

        assert(s1.size() == 2);
        assert(s1.capacity() == 5);
        assert(*s1.nth(0) == 20);
        assert(*s1.nth(1) == 21);

        assert(s2.size() == 0);
        assert(s2.capacity() == 5);

        cout << ">" << endl;
        s2 = std::move(s1);
        cout << "<" << endl;

        assert(s1.size() == 0);
        assert(s1.capacity() == 5);

        assert(s2.size() == 2);
        assert(s2.capacity() == 5);
        assert(*s2.nth(0) == 20);
        assert(*s2.nth(1) == 21);
    }

    cout << "Test move assignment operator (N == 0)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1({10, 11, 12});

        assert(s1.size() == 3);
        assert(s1.capacity() == 4);
        assert(*s1.nth(0) == 12);
        assert(*s1.nth(1) == 11);
        assert(*s1.nth(2) == 10);

        sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s2({20, 21});

        assert(s2.size() == 2);
        assert(s2.capacity() == 2);
        assert(*s2.nth(0) == 21);
        assert(*s2.nth(1) == 20);

        cout << ">" << endl;
        s1 = std::move(s2);
        cout << "<" << endl;

        assert(s1.size() == 2);
        assert(s1.capacity() == 2);
        assert(*s1.nth(0) == 21);
        assert(*s1.nth(1) == 20);

        assert(s2.size() == 0);
        assert(s2.capacity() == 0);

        sfl::small_unordered_flat_set<MyInt, 0, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s3({30, 31});

        assert(s3.size() == 2);
        assert(s3.capacity() == 2);
        assert(*s3.nth(0) == 31);
        assert(*s3.nth(1) == 30);

        cout << ">" << endl;
        s3 = std::move(s1);
        cout << "<" << endl;

        assert(s1.size() == 0);
        assert(s1.capacity() == 0);

        assert(s3.size() == 2);
        assert(s3.capacity() == 2);
        assert(*s3.nth(0) == 21);
        assert(*s3.nth(1) == 20);
    }

    cout << "Test initializer_list assignment operator." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1({10, 11, 12});

        assert(s1.size() == 3);
        assert(s1.capacity() == 5);
        assert(*s1.nth(0) == 10);
        assert(*s1.nth(1) == 11);
        assert(*s1.nth(2) == 12);

        cout << ">" << endl;
        s1 = {20, 21, 22, 20, 21};
        cout << "<" << endl;

        assert(s1.size() == 3);
        assert(s1.capacity() == 5);
        assert(*s1.nth(0) == 20);
        assert(*s1.nth(1) == 21);
        assert(*s1.nth(2) == 22);
    }

    cout << "Test non-member comparison operators." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1({10, 20, 30});

        sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s2({20, 30, 10});

        sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s3({30, 40, 10, 20});

        assert(s1 == s1);
        assert(s1 == s2);
        assert(s1 != s3);
    }

    cout << "Test non-member swap." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1({10, 11, 12});

        sfl::small_unordered_flat_set<MyInt, 5, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s2({20, 21, 22});

        using std::swap;
        swap(s1, s2);
    }

    cout << "Test non-member erase_if(small_unordered_flat_set&, Predicate)." << endl;
    {
        sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
            SFL_TEST_ALLOCATOR<MyInt>
        > s1({10, 20, 30});

        using const_reference =
            typename sfl::small_unordered_flat_set<MyInt, 10, std::equal_to<MyInt>,
                SFL_TEST_ALLOCATOR<MyInt>
            >::const_reference;

        assert(s1.size() == 3);
        assert(*s1.nth(0) == 10);
        assert(*s1.nth(1) == 20);
        assert(*s1.nth(2) == 30);

        {
            cout << ">" << endl;
            auto res = erase_if
            (
                s1,
                [](const_reference& val){ return val == 42; }
            );
            cout << "<" << endl;

            assert(res == 0);
            assert(s1.size() == 3);
            assert(*s1.nth(0) == 10);
            assert(*s1.nth(1) == 20);
            assert(*s1.nth(2) == 30);
        }

        {
            cout << ">" << endl;
            auto res = erase_if
            (
                s1,
                [](const_reference& val){ return val == 20; }
            );
            cout << "<" << endl;

            assert(res == 1);
            assert(s1.size() == 2);
            assert(*s1.nth(0) == 10);
            assert(*s1.nth(1) == 30);
        }
    }
}
