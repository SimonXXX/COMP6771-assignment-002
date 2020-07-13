// COMP6771 - Assignment 2 - Simon Garrod - z326122
//
#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <chrono>
#include <cstddef>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <sys/time.h>

// TESTS ON CONSTRUCTORS AND ASSIGNMENT OPERATORS

TEST_CASE("Test all the non-copy/move constructors") {
	SECTION("default constructor") {
		auto a0 = comp6771::euclidean_vector();
		REQUIRE(a0.dimensions() == 1);
		CHECK(a0.at(0) == 0.0);
	}
	SECTION("Constructor 1: one parameter") {
		auto a1 = comp6771::euclidean_vector(4);
		REQUIRE(a1.dimensions() == 4);
		CHECK(ranges::all_of(a1.cbegin(), a1.cend(), [](double coord) { return coord == 0.0; }));
	}
	SECTION("Constructor 2: two parameters") {
		auto a2 = comp6771::euclidean_vector(13, -3.2);
		REQUIRE(a2.dimensions() == 13);
		CHECK(ranges::all_of(a2.cbegin(), a2.cend(), [](double coord) { return coord == -3.2; }));
	}
	SECTION("Constructor 3: two iterators") {
		auto const v = std::vector<double>{3.4, 6.1, 1.1, 19.5, 23.0001, 10101010.101010, 0.0};
		auto a3 = comp6771::euclidean_vector(v.begin(), v.end());
		REQUIRE(a3.dimensions() == 7);
		auto i = 0;
		for (auto it : v) {
			CHECK(a3.at(i++) == it);
		}
	}
	SECTION("Constructor 4: initialiser list") {
		auto v = std::vector<double>{3.4, 6.1, 1.1, 19.5, 23.0001, 10101010.101010, 0.0};
		auto a3 = comp6771::euclidean_vector{3.4, 6.1, 1.1, 19.5, 23.0001, 10101010.101010, 0.0};
		REQUIRE(a3.dimensions() == 7);
		auto i = 0;
		for (auto it : v) {
			CHECK(a3.at(i++) == it);
		}
	}
	SECTION("Zero dimensions") {
		auto a4 = comp6771::euclidean_vector(0);
		REQUIRE(a4.dimensions() == 0);
		CHECK_THROWS_WITH(a4.at(0) == 0.0, "Index 0 is not valid for this euclidean_vector object");
		auto a5 = comp6771::euclidean_vector(0, 55.123);
		REQUIRE(a5.dimensions() == 0);
		CHECK_THROWS_WITH(a5.at(0) == 0.0, "Index 0 is not valid for this euclidean_vector object");
	}
}

TEST_CASE("Test the copy/move constructor/assignment") {
	SECTION("copy constructor") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b1 = comp6771::euclidean_vector(v.begin(), v.end());
		auto b2 = comp6771::euclidean_vector{b1};
		REQUIRE(b2.dimensions() == b1.dimensions());
		CHECK(b2 == b1);
		auto const b3 = comp6771::euclidean_vector{b1};
		REQUIRE(b3.dimensions() == b1.dimensions());
		CHECK(b3 == b1);
	}
	SECTION("move constructor") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b4 = comp6771::euclidean_vector(v.begin(), v.end());
		auto b5 = b4;
		auto a_move = comp6771::euclidean_vector(std::move(b4));
		CHECK(a_move == b5);
		// NOLINTNEXTLINE(bugprone-use-after-move)
		CHECK(b5 != b4);
		// NOLINTNEXTLINE(bugprone-use-after-move)
		CHECK(b4.dimensions() == 0);
	}

	SECTION("copy assignment") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b1 = comp6771::euclidean_vector();
		auto b2 = comp6771::euclidean_vector(7, 3.3);
		{
			auto b3 = comp6771::euclidean_vector(v.begin(), v.end());
			REQUIRE(b1.dimensions() == 1);
			REQUIRE(b2.dimensions() == 7);
			b1 = b3;
			b2 = b3;
		} // b3 goes out of scope
		REQUIRE(b1.dimensions() == 7);
		REQUIRE(b2.dimensions() == 7);
		auto i = 0;
		for (auto it : v) { // check against initialising vector
			CHECK(b1.at(i) == it);
			CHECK(b2.at(i++) == it);
		}
	}
	SECTION("move assignment") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b4 = comp6771::euclidean_vector(v.begin(), v.end());
		auto b5 = b4;
		auto a_move = comp6771::euclidean_vector(15, 12.345);
		a_move = std::move(b4);
		CHECK(a_move == b5);
		// NOLINTNEXTLINE(bugprone-use-after-move)
		CHECK(b5 != b4);
		// NOLINTNEXTLINE(bugprone-use-after-move)
		CHECK(b4.dimensions() == 0);
	}
}
TEST_CASE("at and dimensions") {
	SECTION("at & dimension functions") {
		// some basic functionality tested in sections above
		auto const v1 = std::vector<double>{3.4, 5.6, 7.8, 111.222, 333.444};
		auto const e1 = comp6771::euclidean_vector(v1.begin(), v1.end());
		auto e2 = comp6771::euclidean_vector(v1.begin(), v1.end());
		// at()
		CHECK_NOTHROW(e2.at(3) = e1[4]); // at used for assignment
		CHECK(e2[3] == e1.at(4));
		CHECK_NOTHROW(e2[4] = e1.at(1)); // [] used for assignment
		CHECK(e2.at(4) == e1[1]);
		CHECK_THROWS_WITH(e1.at(-11), "Index -11 is not valid for this euclidean_vector object");
		CHECK_NOTHROW(e2.at(0));
		CHECK_NOTHROW(e1.at(e1.dimensions() - 1));
		CHECK_THROWS_WITH(e2.at(e2.dimensions()),
		                  "Index 5 is not valid for this euclidean_vector object");
		CHECK_THROWS_WITH(e2.at(11), "Index 11 is not valid for this euclidean_vector object");

		// [] operator
		auto test_double = 0.0;
		CHECK_THROWS_WITH(test_double = e1[-2],
		                  "Index -2 is not valid for this euclidean_vector object");
		CHECK_THROWS_WITH(test_double = e1[5], "Index 5 is not valid for this euclidean_vector object");
		e2[4] = 25.25;
		CHECK(e2.at(4) == 25.25);
		CHECK_THROWS_WITH(e2[-3] = 1.1, "Index -3 is not valid for this euclidean_vector object");
		CHECK_THROWS_WITH(e2[17] = 1.1, "Index 17 is not valid for this euclidean_vector object");
	}
}