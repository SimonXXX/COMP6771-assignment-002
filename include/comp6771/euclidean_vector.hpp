#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

//#include <compare>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <ostream>
#include <range/v3/algorithm.hpp>
#include <range/v3/iterator.hpp>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace comp6771 {
	class euclidean_vector_error : public std::runtime_error {
	public:
		explicit euclidean_vector_error(std::string const& what) noexcept
		: std::runtime_error(what) {}
	};

	class euclidean_vector {
	public:
		euclidean_vector();
		explicit euclidean_vector(int dim);
		euclidean_vector(int dim, double magnitude);
		euclidean_vector(std::vector<double>::const_iterator begin,
		                 std::vector<double>::const_iterator end);
		euclidean_vector(std::initializer_list<double>);

		euclidean_vector(euclidean_vector const&); // copy constructor

		// euclidean_vector(euclidean_vector&&) noexcept; // move constructor

		auto operator=(euclidean_vector const&) -> euclidean_vector&;
		auto operator[](int const&) const -> double;
		auto operator[](int const&) -> double&;
		auto operator+() -> euclidean_vector;
		auto operator-() -> euclidean_vector;
		auto operator+=(euclidean_vector const&) -> euclidean_vector&;
		auto operator-=(euclidean_vector const&) -> euclidean_vector&;
		auto operator*=(euclidean_vector const&) -> euclidean_vector&;
		auto operator/=(euclidean_vector const&) -> euclidean_vector&;

		explicit operator std::vector<double>() const {
			auto temp = std::vector<double>{};
			for (size_t i = 0; i < static_cast<size_t>(dim_); i++) {
				temp.push_back(magnitudes_[i]);
			}
			return temp;
		}

		auto get_coords() -> double*;
		[[nodiscard]] auto get_dim() const -> size_t;

	private:
		// auto swap(euclidean_vector& other) -> euclidean_vector&;
		// ass2 spec requires we use double[]
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitudes_;
		size_t dim_;
	};
} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
