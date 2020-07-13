// COMP6771 - Assignment 2 - Simon Garrod - z326122
//
#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <chrono>
#include <cstddef>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>

// TESTS ON FRIEND FUNCTION

TEST_CASE("friends") {
	// Test all the four compound operators
	SECTION("== and != operators") {
		auto const f1 = comp6771::euclidean_vector{0.0, -1.0, 2.0, 1.111, -45.45};
		auto f2 = comp6771::euclidean_vector{0.0, -1.0, 2.0, 1.111, -45.45};
		auto const f3 = comp6771::euclidean_vector{0.0, -1.0, 2.0, 1.111, -45.45, 999.0};
		CHECK(f1 == f2);
		CHECK(f2 == f1);
		CHECK(f2 != f3);
		CHECK(f3 != f2);
		f2[3] = 54.54;
		CHECK(f1 != f2);
		CHECK(f2 != f1);
	}
	SECTION("+ and - operators") {
		auto const f1 = comp6771::euclidean_vector{0.0, 2.0, 2.5, 1.111, 95.45};
		auto f2 = comp6771::euclidean_vector{0.0, 1.0, 2.0, 1.111, 45.45};
		auto const f3 = comp6771::euclidean_vector{0.0, 1.0, 2.0, 1.111, 45.45, 999.0};

		auto const f4 = f1 + f2; // add and construct a new object
		auto f12 = comp6771::euclidean_vector{0.0, 3.0, 4.5, 2.222, 140.9};
		CHECK(f4 == f12);

		auto f5 = comp6771::euclidean_vector(1);
		f5 = f1 + f2; // add and replace an existing object
		CHECK(f5 == f12);

		auto f6 = comp6771::euclidean_vector(1); // different dimensions
		CHECK_THROWS_WITH(f6 = f1 + f3, "vectors different sizes");

		auto const f7 = f1 - f2; // take away and construct a new object
		auto f127 = comp6771::euclidean_vector{0.0, 1.0, 0.5, 0.0, 50.0};
		CHECK(f7 == f127);

		auto f8 = comp6771::euclidean_vector(1);
		f8 = f1 - f2; // add and replace an existing object
		CHECK(f8 == f127);

		auto f6a = comp6771::euclidean_vector(1); // different dimensions
		CHECK_THROWS_WITH(f6a = f1 - f3, "vectors different sizes");

		CHECK(f6 == f6a); // the exception throwing has not altered these objects

		// taking away from self and adding to self (clang doesnt like identical parameters)
		// NOLINTNEXTLINE
		f2 = f2 - f2;
		CHECK(ranges::all_of(f2.cbegin(), f2.cend(), [](double coord) { return coord == 0.0; }));
		// taking away from self and adding to self (using unary operator)
		f12 = f12 + -f12;
		CHECK(ranges::all_of(f12.cbegin(), f12.cend(), [](double coord) { return coord == 0.0; }));
	}
	SECTION("* and / operators") {
		auto const g1 = comp6771::euclidean_vector{0.0, 2.0, 3.0, -1.1, -22.22};
		auto g2 = comp6771::euclidean_vector{0.0, 1.0, 2.0, 3.3, 4.4};
		auto g3 = comp6771::euclidean_vector{0.0, 3.0, 4.5, 2.222, 140.9};

		auto const g4a = 3 * g1; // multipier first parameter
		auto const g4b = g1 * 3; // multipier second parameter
		auto g13 = comp6771::euclidean_vector{0.0, 6.0, 9.0, -3.3, -66.66};
		CHECK(g4a == g13);
		CHECK(g4b == g13);

		auto g5a = comp6771::euclidean_vector(30);
		auto g5b = comp6771::euclidean_vector(15, -99.111);
		g5a = 3 * g1; // multipier first parameter
		g5b = g1 * 3; // multipier second parameter
		CHECK(g5a == g13);
		CHECK(g5b == g13);

		auto const g6 = g1 / 10; // divide
		auto g16 = comp6771::euclidean_vector{0.0, 0.2, 0.3, -0.11, -2.222};
		CHECK(g6 == g16);

		auto g7 = comp6771::euclidean_vector(15, -99.111);
		g7 = g1 / 10; // divide
		CHECK(g7 == g16);
	}
	SECTION("output stream") {
		auto h1 = comp6771::euclidean_vector{0.0, 2.0, 2.5, 1.111, 95.45};
		auto h2 = comp6771::euclidean_vector(5, 2.0);
		auto h3 = comp6771::euclidean_vector(5, 1.0);
		CHECK(fmt::format("{}", h1) == "[0.0 2.0 2.5 1.111 95.45]");
	}
}
