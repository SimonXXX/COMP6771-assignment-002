#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <cstddef>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <sys/types.h>

TEST_CASE("Utility Functions") {
	SECTION("") {
		auto j1 = comp6771::euclidean_vector(5, 2.0);
		CHECK(euclidean_norm(j1) == std::sqrt(20));
		REQUIRE(j1.is_cached());
		j1[3] = 3.0;
		REQUIRE(!j1.is_cached());
		CHECK(euclidean_norm(j1) == std::sqrt(25));
		REQUIRE(j1.is_cached());
		j1.at(2) = 4.0;
		REQUIRE(!j1.is_cached());
		CHECK(euclidean_norm(j1) == std::sqrt(37));

		auto j2 = comp6771::euclidean_vector(5, 2.0);
		auto j3 = comp6771::euclidean_vector(5, 3.0);
		REQUIRE(!j2.is_cached());
		REQUIRE(!j3.is_cached());
		auto j4 = j2 + j3;
		REQUIRE(!j4.is_cached());
		CHECK(euclidean_norm(j4) == std::sqrt(125));

		auto j5 = comp6771::euclidean_vector(5, 16.0);
		CHECK(euclidean_norm(j5) == std::sqrt(5 * 16 * 16));
		j5 /= 4;
		REQUIRE(!j5.is_cached());
		CHECK(euclidean_norm(j5) == std::sqrt(5 * 16));
	}
}

TEST_CASE("constructors and member functions (at and dimensions") {
	// Test all the constructors and dimension method
	SECTION("default constructor") {
		auto a0 = comp6771::euclidean_vector();
		REQUIRE(a0.dimensions() == 1);
		CHECK(a0.at(0) == 0.0);
	}
	SECTION("Constructor 1: one parameter") {
		auto a1 = comp6771::euclidean_vector(4);
		REQUIRE(a1.dimensions() == 4);
		auto test = static_cast<std::vector<double>>(a1);
		CHECK(ranges::all_of(test.begin(), test.end(), [](double coord) { return coord == 0.0; }));
	}
	SECTION("Constructor 2: two parameters") {
		auto a2 = comp6771::euclidean_vector(13, -3.2);
		REQUIRE(a2.dimensions() == 13);
		auto test = static_cast<std::vector<double>>(a2);
		CHECK(ranges::all_of(test.begin(), test.end(), [](double coord) { return coord == -3.2; }));
	}
	SECTION("Constructor 3: two iterators") {
		auto v = std::vector<double>{3.4, 6.1, 1.1, 19.5, 23.0001, 10101010.101010, 0.0};
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
	SECTION("at & dimension functions") {
		// basic functionality tested in sections above
		auto const v1 = std::vector<double>{3.4, 5.6, 7.8, 111.222, 333.444};
		auto const e1 = comp6771::euclidean_vector(v1.begin(), v1.end());
		auto e2 = comp6771::euclidean_vector(v1.begin(), v1.end());

		CHECK_THROWS_WITH(e1.at(-11), "Index -11 is not valid for this euclidean_vector object");
		CHECK_NOTHROW(e2.at(0));
		CHECK_NOTHROW(e1.at(e1.dimensions() - 1));
		CHECK_THROWS_WITH(e2.at(e2.dimensions()),
		                  "Index 5 is not valid for this euclidean_vector object");
		CHECK_THROWS_WITH(e2.at(11), "Index 11 is not valid for this euclidean_vector object");
	}
}

TEST_CASE("copy/move constructors") {
	// Test all the copy/move constructors
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
}
TEST_CASE("operators - subscripts and unary +/-") {
	//
	SECTION("operators - subscripts and unary +/-") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b1 = comp6771::euclidean_vector(v.begin(), v.end());
		REQUIRE(b1.dimensions() == 7);
		auto test_double = 0.0;
		for (auto i = 0; i < b1.dimensions(); i++) {
			CHECK_NOTHROW(test_double == b1[i]);
		}

		CHECK_THROWS_WITH(test_double = b1[-2], "negative subscript");
		CHECK_THROWS_WITH(test_double = b1[7], "subscript too high");

		b1[4] = 25.25;
		CHECK(b1.at(4) == 25.25);
		CHECK_THROWS_WITH(b1[-3] = 1.1, "negative subscript");
		CHECK_THROWS_WITH(b1[17] = 1.1, "subscript too high");
		auto c1 = +b1;
		REQUIRE(c1.dimensions() == b1.dimensions());
		for (auto i = 0; i < b1.dimensions(); i++) {
			CHECK(c1[i] == b1[i]);
		}

		auto c2 = -b1;
		REQUIRE(c2.dimensions() == b1.dimensions());
		for (auto i = 0; i < b1.dimensions(); i++) {
			CHECK(c2[i] == -b1[i]);
		}
	}
}
TEST_CASE("compound operators") {
	// Test all the four compound operators
	SECTION("+=/-= operators") {
		auto v1 = std::vector<double>{3.4, 5.6, 7.8, 111.222};
		// auto v1 = std::vector<double>{3.4, 5.6, 7.8, 111.222};
		auto d1 = comp6771::euclidean_vector(v1.begin(), v1.end());
		auto d2 = comp6771::euclidean_vector(4, 10.5);
		auto d3 = comp6771::euclidean_vector(6, 10.5);
		d2 += d1;
		CHECK(d2[1] == 16.1);
		CHECK_THROWS_WITH(d3 += d1, "euclidean_vectors are different magnitudes");
		auto d4 = comp6771::euclidean_vector(4, 10.5);
		d4 -= d1;
		CHECK(d4[1] == 4.9);
		CHECK_THROWS_WITH(d3 -= d1, "euclidean_vectors are different magnitudes");
	}
	SECTION("*= and /= operators") {
		auto v1 = std::vector<double>{3.4, 5.6, 7.8, 111.222, 333.444};
		auto d1 = comp6771::euclidean_vector(v1.begin(), v1.end());
		d1 *= 5.67;
		auto i = 0;
		for (auto it : v1) {
			CHECK(d1[i++] == it * 5.67);
		}

		auto d2 = comp6771::euclidean_vector(5, 22.2);
		d2 /= 2;
		for (auto j = 0; j < d2.dimensions(); j++) {
			CHECK(d2[j] == 11.1);
		}
	}
	SECTION("type conversions") {
		auto const a = comp6771::euclidean_vector{0.0, 1.0, 2.0};
		auto const vf = static_cast<std::vector<double>>(a);
		// for (auto x : vf) {
		//	std::cout << x << "\n";
		//}
	}
}

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

// gsl::narrow_cast

// auto temp = static_cast<std::vector<double>>(f1);
// CHECK(ranges::all_of(temp.begin(), temp.end(), [](double coord) { return coord == 3.0; }));

// CHECK(ranges::all_of(f1.  get(), f1.get()+5, [](double coord) { return coord == 3.0; }));
// auto const a2 = comp6771::euclidean_vector(3, 3.0);
// auto const a3 = comp6771::euclidean_vector(3, 3.0);