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

// TESTS ON UTILITY FUNCTIONS

// function for benchmarking
// Catch2 does have a BENCHMARK macro, but couldn't get it working
// but as this is only for checking norm value checking this will surfice
auto timestamp() -> int64_t {
	return duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
	   .count();
}

TEST_CASE("Utility Functions") {
	SECTION("euclidean norm") {
		auto j1 = comp6771::euclidean_vector(5, 2.0);
		CHECK(euclidean_norm(j1) == std::sqrt(20));

		// caching after []
		REQUIRE(j1.is_cached());
		j1[3] = 3.0;
		REQUIRE(!j1.is_cached());
		CHECK(euclidean_norm(j1) == std::sqrt(25));
		REQUIRE(j1.is_cached());

		// caching after .at()
		j1.at(2) = 4.0;
		REQUIRE(!j1.is_cached());
		CHECK(euclidean_norm(j1) == std::sqrt(37));

		// euclidean_vector doesnt affect caching
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

		// caching is faster
		auto j7 = comp6771::euclidean_vector(150000, 21.987654321);
		int64_t t0 = timestamp();
		euclidean_norm(j7);
		int64_t t1 = timestamp();
		auto p1 = t1 - t0; // time without caching

		t0 = timestamp();
		euclidean_norm(j7);
		t1 = timestamp();
		auto p2 = t1 - t0; // time with caching
		CHECK(p1 > p2);

		t0 = timestamp();
		j7.at(1) = 0.0; // change one value so cach is not valid
		euclidean_norm(j7);
		t1 = timestamp();
		auto p3 = t1 - t0; // time without caching
		CHECK(p3 > p2);
	}
	SECTION("unit") {
		auto d1 = comp6771::euclidean_vector{4.0, 111.1, -23.23, 2.1213};
		auto d1_norm = euclidean_norm(d1);
		auto d2 = comp6771::euclidean_vector(0);
		auto d3 = comp6771::euclidean_vector{};
		CHECK_THROWS_WITH(d3 = unit(d2),
		                  "euclidean_vector with no dimensions does not have a unit vector");
		d3 = unit(d1);
		for (auto i = 0; i < 4; i++) {
			CHECK(std::fabs(d3[i] - (d1[i] / d1_norm)) < 0.00001);
		}
		auto d4 = comp6771::euclidean_vector(6, 0.0);
		CHECK_THROWS_WITH(d3 = unit(d4),
		                  "euclidean_vector with zero euclidean normal does not have a unit vector");
	}

	SECTION("dot ") {
		auto e1 = comp6771::euclidean_vector(4, 2.1213);
		auto e2 = comp6771::euclidean_vector(4, 10.01);
		auto e3 = comp6771::euclidean_vector(6, 2.1213);
		auto dot_prod = dot(e1, e2);
		CHECK(dot_prod == 4 * (10.01 * 2.1213));
		CHECK_THROWS_WITH(dot_prod = dot(e2, e3), "Dimensions of LHS(4) and RHS(6) do not match");
	}
}
