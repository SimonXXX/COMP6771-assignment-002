#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <cstddef>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <sys/types.h>

TEST_CASE("constructors and member functions (at and dimensions") {
	// Test all the constructors and dimension method
	SECTION("default") {
		auto a0 = comp6771::euclidean_vector();
		REQUIRE(a0.dimensions() == 1);
		CHECK(a0.at(0) == 0.0);
	}
	SECTION("one") {
		auto a1 = comp6771::euclidean_vector(4);
		REQUIRE(a1.dimensions() == 4);
		for (int i = 0; i < a1.dimensions(); i++) {
			CHECK(a1.at(i) == 0.0);
		}
	}
	SECTION("two") {
		auto a2 = comp6771::euclidean_vector(3, 3.2);
		REQUIRE(a2.dimensions() == 3);

		for (int i = 0; i < a2.dimensions(); i++) {
			CHECK(a2.at(i) == 3.2);
		}
		// CHECK(ranges::all_of([0], a2.get_coords()[2],[&](auto coord) { return coord == 3.0; }));
		// auto const a2 = comp6771::euclidean_vector(3, 3.0);
		// auto const a3 = comp6771::euclidean_vector(3, 3.0);

		// CHECK(fmt::format("{}", a1 + a2 + a3) == "[9 9 9]");
	}
	SECTION("Constructor 3: two iterators") {
		auto v = std::vector<double>{3.4, 6.1, 1.1, 19.5, 23.0001, 10101010.101010, 0.0};
		auto a3 = comp6771::euclidean_vector(v.begin(), v.end());
		REQUIRE(a3.dimensions() == 7);
		auto it = v.begin();
		for (int i = 0; i < a3.dimensions(); i++) {
			CHECK(a3.at(i) == *it++);
		}
	}
	SECTION("Constructor 4: initialiser list") {
		auto v = std::vector<double>{3.4, 6.1, 1.1, 19.5, 23.0001, 10101010.101010, 0.0};
		auto a3 = comp6771::euclidean_vector{3.4, 6.1, 1.1, 19.5, 23.0001, 10101010.101010, 0.0};
		REQUIRE(a3.dimensions() == 7);
		auto i = 0;
		for (auto const it : v) {
			CHECK(a3.at(i++) == it);
		}
	}
	SECTION("at functions") {
		auto const v1 = std::vector<double>{3.4, 5.6, 7.8, 111.222, 333.444};
		auto const e1 = comp6771::euclidean_vector(v1.begin(), v1.end());
		auto e2 = comp6771::euclidean_vector(v1.begin(), v1.end());
		for (auto i = 0; i < e1.dimensions(); i++) {
			CHECK(e1.at(i) == v1[static_cast<size_t>(i)]);
		}
		CHECK_THROWS_WITH(e1.at(-11), "Index X is not valid for this euclidean_vector object");

		for (auto i = 0; i < e2.dimensions(); i++) {
			CHECK(e2.at(i) == e1.at(i));
		}
		CHECK_THROWS_WITH(e2.at(e2.dimensions()),
		                  "Index X is not valid for this euclidean_vector object");
		CHECK_THROWS_WITH(e2.at(11), "Index X is not valid for this euclidean_vector object");
	}
}

TEST_CASE("copy/move constructors") {
	// Test all the copy/move constructors
	SECTION("copy constructor") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b1 = comp6771::euclidean_vector(v.begin(), v.end());
		auto b2 = comp6771::euclidean_vector{b1};
		REQUIRE(b1.dimensions() == 7);
		REQUIRE(b2.dimensions() == 7);
		for (size_t i = 0; i < static_cast<size_t>(b1.dimensions()); i++) {
			CHECK(b1.get_coords()[i] == b2.get_coords()[i]);
		}
	}
	SECTION("copy assignment") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b1 = comp6771::euclidean_vector(v.begin(), v.end());
		auto b2 = comp6771::euclidean_vector();
		REQUIRE(b1.dimensions() == 7);
		{
			REQUIRE(b2.dimensions() == 1);
			b2 = b1;
			REQUIRE(b1.dimensions() == 7);
			REQUIRE(b2.dimensions() == 7);
		}
		for (size_t i = 0; i < static_cast<size_t>(b1.dimensions()); i++) {
			CHECK(b1.get_coords()[i] == b2.get_coords()[i]);
		}
	}
	SECTION("operators - subscripts and unary +/-") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b1 = comp6771::euclidean_vector(v.begin(), v.end());
		REQUIRE(b1.dimensions() == 7);
		auto test_double = 0.0;
		for (int i = 0; i < static_cast<int>(b1.dimensions()); i++) {
			CHECK_NOTHROW(test_double == b1.get_coords()[i]);
		}

		CHECK_THROWS_WITH(test_double = b1[-2], "negative subscript");
		CHECK_THROWS_WITH(test_double = b1[7], "subscript too high");

		b1[4] = 25.25;
		CHECK(b1.at(4) == 25.25);
		CHECK_THROWS_WITH(b1[-3] = 1.1, "negative subscript");
		CHECK_THROWS_WITH(b1[17] = 1.1, "subscript too high");
		auto c1 = +b1;
		REQUIRE(c1.dimensions() == b1.dimensions());
		for (size_t i = 0; i < static_cast<size_t>(b1.dimensions()); i++) {
			CHECK(c1.get_coords()[i] == b1.get_coords()[i]);
		}

		auto c2 = -b1;
		REQUIRE(c2.dimensions() == b1.dimensions());
		for (size_t i = 0; i < static_cast<size_t>(b1.dimensions()); i++) {
			CHECK(c2.get_coords()[i] == -b1.get_coords()[i]);
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
		CHECK(d2.get_coords()[1] == 16.1);
		CHECK_THROWS_WITH(d3 += d1, "euclidean_vectors are different magnitudes");
		auto d4 = comp6771::euclidean_vector(4, 10.5);
		d4 -= d1;
		CHECK(d4.get_coords()[1] == 4.9);
		CHECK_THROWS_WITH(d3 -= d1, "euclidean_vectors are different magnitudes");
	}
	SECTION("*= and /= operators") {
		auto v1 = std::vector<double>{3.4, 5.6, 7.8, 111.222, 333.444};
		auto d1 = comp6771::euclidean_vector(v1.begin(), v1.end());
		auto d2 = comp6771::euclidean_vector(5, 11.1);
		auto d3 = comp6771::euclidean_vector(7, 11.1);
		d2 *= d1;
		for (size_t i = 0; i < static_cast<size_t>(d2.dimensions()); i++) {
			CHECK(d2.get_coords()[i] == 11.1 * d1.get_coords()[i]);
		}
		CHECK_THROWS_WITH(d3 *= d1, "euclidean_vectors are different magnitudes");
		auto d4 = comp6771::euclidean_vector(5, 11.1);
		d4 /= d1;
		for (size_t i = 0; i < static_cast<size_t>(d4.dimensions()); i++) {
			CHECK(d4.get_coords()[i] == 11.1 / d1.get_coords()[i]);
		}
		CHECK_THROWS_WITH(d3 /= d1, "euclidean_vectors are different magnitudes");
	}
	SECTION("type conversions") {
		auto const a = comp6771::euclidean_vector{0.0, 1.0, 2.0};
		auto const vf = static_cast<std::vector<double>>(a);
		for (auto x : vf) {
			std::cout << x << "\n";
		}
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
		auto f12 = comp6771::euclidean_vector{0.0, 3.0, 4.5, 2.222, 140.9};
		auto const f3 = comp6771::euclidean_vector{0.0, 1.0, 2.0, 1.111, 45.45, 999.0};
		auto const f4 = f1 + f2;
		CHECK(f4 == f12);

		auto f5 = comp6771::euclidean_vector(1);
		f5 = f1 + f2;

		auto f6 = comp6771::euclidean_vector(1);
		CHECK_THROWS_WITH(f6 = f1 + f3, "vectors different sizes");
	}
	SECTION("* and / operators") {}
	SECTION("output stream") {
		auto const f1 = comp6771::euclidean_vector{0.0, 2.0, 2.5, 1.111, 95.45};
		auto f2 = comp6771::euclidean_vector{0.0, 1.0, 2.0, 1.111, 45.45};
		auto f12 = comp6771::euclidean_vector{0.0, 3.0, 4.5, 2.222, 140.9};
		auto const f3 = comp6771::euclidean_vector{0.0, 1.0, 2.0, 1.111, 45.45, 999.0};
		std::cout << f1 << "\n" << f2 << "\n" << f12 << "\n" << f3 << "\n";
	}

} // gsl::narrow_cast