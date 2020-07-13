// COMP6771 - Assignment 2 - Simon Garrod - z326122
#include "comp6771/euclidean_vector.hpp"

#include <catch2/catch.hpp>
#include <chrono>
#include <cstddef>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <iostream>

// TESTS ON OPERATORS

TEST_CASE("Unary operators") {
	SECTION("unary plus") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b1 = comp6771::euclidean_vector(v.begin(), v.end());

		auto const c1 = +b1;
		REQUIRE(c1.dimensions() == b1.dimensions());
		for (auto i = 0; i < b1.dimensions(); i++) {
			CHECK(c1[i] == b1[i]);
		}
	}
	SECTION("unary minus") {
		auto v = std::vector<double>{3.4, 5.6, 7.8, 111.222, 3333.4444, 55555.55555, 0.0};
		auto b1 = comp6771::euclidean_vector(v.begin(), v.end());
		auto c2 = -b1;
		REQUIRE(c2.dimensions() == b1.dimensions());
		for (auto i = 0; i < b1.dimensions(); i++) {
			CHECK(c2[i] == -b1[i]);
		}
	}
}
TEST_CASE("compound operators") {
	SECTION("+=/-= operators") {
		auto v1 = std::vector<double>{3.4, 5.6, 7.8, 111.222};
		auto const d1 = comp6771::euclidean_vector(v1.begin(), v1.end());
		auto d2 = comp6771::euclidean_vector(4, 10.5);
		auto d3 = comp6771::euclidean_vector(6, 10.5);
		d2 += d1;
		CHECK(d2[1] == 16.1);

		CHECK_THROWS_WITH(d3 += d1, "euclidean_vectors are different magnitudes");
		auto d4 = comp6771::euclidean_vector(4, 10.5);
		d4 -= d1;
		CHECK(d4[1] == 4.9);
		CHECK_THROWS_WITH(d3 -= d1, "euclidean_vectors are different magnitudes");

		d3 += d3;
		CHECK(ranges::all_of(d3.cbegin(), d3.cend(), [](double coord) { return coord == 21.0; }));
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
}
TEST_CASE("type conversions") {
	SECTION("type conversions") {
		auto const a = comp6771::euclidean_vector{0.0, 1.0, 2.0};
		auto const vf = static_cast<std::vector<double>>(a);
		auto const b = comp6771::euclidean_vector(vf.begin(), vf.end());
		CHECK(a == b);
		auto a1 = comp6771::euclidean_vector{0.0, 1.0, 2.0};
		auto lf = static_cast<std::list<double>>(a1);
		auto b1 = comp6771::euclidean_vector(vf.begin(), vf.end());
		CHECK(a1 == b1);
	}
}
