// ofekbarshalom@gmail.com

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "../code/MyContainer.hpp"

using namespace container;
using namespace std;

TEST_CASE("Basic operations") {
    MyContainer<int> c;
    CHECK(c.size() == 0);

    c.add(5);
    c.add(3);
    c.add(8);
    CHECK(c.size() == 3);

    c.remove(3);
    CHECK(c.size() == 2);

    CHECK_THROWS_AS(c.remove(42), runtime_error);
}

TEST_CASE("AscendingOrderIterator works") {
    MyContainer<int> c;
    c.add(7);
    c.add(1);
    c.add(3);

    vector<int> result;
    for (auto it = c.begin_ascending_order(); it != c.end_ascending_order(); ++it) {
        result.push_back(*it);
    }

    CHECK(result == vector<int>{1, 3, 7});
}

TEST_CASE("DescendingOrderIterator works") {
    MyContainer<int> c;
    c.add(7);
    c.add(1);
    c.add(3);

    vector<int> result;
    for (auto it = c.begin_descending_order(); it != c.end_descending_order(); ++it) {
        result.push_back(*it);
    }

    CHECK(result == vector<int>{7, 3, 1});
}

TEST_CASE("SideCrossOrderIterator works") {
    MyContainer<int> c;
    c.add(7);
    c.add(1);
    c.add(3);
    c.add(9);
    c.add(5);

    vector<int> result;
    for (auto it = c.begin_sidecross_order(); it != c.end_sidecross_order(); ++it) {
        result.push_back(*it);
    }

    CHECK(result == vector<int>{1, 9, 3, 7, 5});
}

TEST_CASE("ReverseOrderIterator works") {
    MyContainer<int> c;
    c.add(4);
    c.add(6);
    c.add(2);

    vector<int> result;
    for (auto it = c.begin_reverse_order(); it != c.end_reverse_order(); ++it) {
        result.push_back(*it);
    }

    CHECK(result == vector<int>{2, 6, 4});
}

TEST_CASE("OrderIterator works") {
    MyContainer<int> c;
    c.add(10);
    c.add(20);
    c.add(30);

    vector<int> result;
    for (auto it = c.begin_order(); it != c.end_order(); ++it) {
        result.push_back(*it);
    }

    CHECK(result == vector<int>{10, 20, 30});
}

TEST_CASE("MiddleOutOrderIterator works") {
    MyContainer<int> c;
    c.add(1);
    c.add(2);
    c.add(3);
    c.add(4);
    c.add(5);

    vector<int> result;
    for (auto it = c.begin_middleout_order(); it != c.end_middleout_order(); ++it) {
        result.push_back(*it);
    }

    CHECK(result == vector<int>{3, 2, 4, 1, 5});
}

TEST_CASE("MyContainer works with string") {
    MyContainer<string> c;
    c.add("banana");
    c.add("apple");
    c.add("cherry");

    vector<string> result;
    for (auto it = c.begin_ascending_order(); it != c.end_ascending_order(); ++it) {
        result.push_back(*it);
    }

    CHECK(result == vector<string>{"apple", "banana", "cherry"});
}

TEST_CASE("MyContainer works with double") {
    MyContainer<double> c;
    c.add(3.14);
    c.add(1.41);
    c.add(2.72);

    vector<double> result;
    for (auto it = c.begin_ascending_order(); it != c.end_ascending_order(); ++it) {
        result.push_back(*it);
    }

    CHECK(result == vector<double>{1.41, 2.72, 3.14});
}

