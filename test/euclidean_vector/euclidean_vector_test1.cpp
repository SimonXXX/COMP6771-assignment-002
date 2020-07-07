#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>
#include <sys/types.h>

TEST_CASE("constructors") {
	// Test all the constructors
	SECTION("default") {
		auto a0 = comp6771::euclidean_vector();
		REQUIRE(a0.get_dim() == 1);
		CHECK(a0.get_coords()[0] == 0.0);
	}
	SECTION("one") {
		auto a1 = comp6771::euclidean_vector(4);
		REQUIRE(a1.get_dim() == 4);
		for (size_t i = 0; i < a1.get_dim(); i++) {
			CHECK(a1.get_coords()[i] == 0.0);
		}
	}
	SECTION("two") {
		auto a2 = comp6771::euclidean_vector(3, 3.2);
		REQUIRE(a2.get_dim() == 3);

		for (size_t i = 0; i < a2.get_dim(); i++) {
			CHECK(a2.get_coords()[i] == 3.2);
		}
		// CHECK(ranges::all_of([0], a2.get_coords()[2],[&](auto coord) { return coord == 3.0; }));
		// auto const a2 = comp6771::euclidean_vector(3, 3.0);
		// auto const a3 = comp6771::euclidean_vector(3, 3.0);

		// CHECK(fmt::format("{}", a1 + a2 + a3) == "[9 9 9]");
	}
	SECTION("Constructor 3: two iterators") {
		auto v = std::vector<double>{3.4, 6.1, 1.1, 19.5, 23.0001, 10101010.101010, 0.0};
		auto a3 = comp6771::euclidean_vector(v.begin(), v.end());
		REQUIRE(a3.get_dim() == 7);
		auto it = v.begin();
		for (size_t i = 0; i < a3.get_dim(); i++) {
			CHECK(a3.get_coords()[i] == *it++);
		}
	}
}
TEST_CASE("copy/move constructors") {
	// Test all the copy/move constructors
	SECTION("copy constructor") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b1 = comp6771::euclidean_vector(v.begin(), v.end());
		auto b2 = comp6771::euclidean_vector{b1};
		REQUIRE(b1.get_dim() == 7);
		REQUIRE(b2.get_dim() == 7);
		for (size_t i = 0; i < b1.get_dim(); i++) {
			CHECK(b1.get_coords()[i] == b2.get_coords()[i]);
		}
	}
	SECTION("copy assignment") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b1 = comp6771::euclidean_vector(v.begin(), v.end());
		auto b2 = comp6771::euclidean_vector();
		REQUIRE(b1.get_dim() == 7);
		{
			REQUIRE(b2.get_dim() == 1);
			b2 = b1;
			REQUIRE(b1.get_dim() == 7);
			REQUIRE(b2.get_dim() == 7);
		}
		for (size_t i = 0; i < b1.get_dim(); i++) {
			CHECK(b1.get_coords()[i] == b2.get_coords()[i]);
		}
	}
	SECTION("operators - subscripts and unary +/-") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b1 = comp6771::euclidean_vector(v.begin(), v.end());
		REQUIRE(b1.get_dim() == 7);
		auto test_double = 0.0;
		for (int i = 0; i < static_cast<int>(b1.get_dim()); i++) {
			CHECK_NOTHROW(test_double == b1.get_coords()[i]);
		}

		CHECK_THROWS_WITH(test_double = b1[-2], "negative subscript");
		CHECK_THROWS_WITH(test_double = b1[7], "subscript too high");

		b1[4] = 25.25;
		CHECK(b1.get_coords()[4] == 25.25);
		CHECK_THROWS_WITH(b1[-3] = 1.1, "negative subscript");
		CHECK_THROWS_WITH(b1[17] = 1.1, "subscript too high");
		auto c1 = +b1;
		REQUIRE(c1.get_dim() == b1.get_dim());
		for (size_t i = 0; i < b1.get_dim(); i++) {
			CHECK(c1.get_coords()[i] == b1.get_coords()[i]);
		}

		auto c2 = -b1;
		REQUIRE(c2.get_dim() == b1.get_dim());
		for (size_t i = 0; i < b1.get_dim(); i++) {
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
		for (size_t i = 0; i < d2.get_dim(); i++) {
			CHECK(d2.get_coords()[i] == 11.1 * d1.get_coords()[i]);
		}
		CHECK_THROWS_WITH(d3 *= d1, "euclidean_vectors are different magnitudes");
		auto d4 = comp6771::euclidean_vector(5, 11.1);
		d4 /= d1;
		for (size_t i = 0; i < d4.get_dim(); i++) {
			CHECK(d4.get_coords()[i] == 11.1 / d1.get_coords()[i]);
		}
		CHECK_THROWS_WITH(d3 /= d1, "euclidean_vectors are different magnitudes");

		SECTION("type conversions") {
			auto const a = comp6771::euclidean_vector(3, 3.3);
			auto const vf = static_cast<std::vector<double>>(a);
			for (auto x : vf) {
				std::cout << x << "\n";
			}
		}
	}
} // gsl::narrow_cast