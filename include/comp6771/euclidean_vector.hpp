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
		euclidean_vector(euclidean_vector&&) noexcept; // move constructor

		auto operator=(euclidean_vector const&) -> euclidean_vector&;
		auto operator=(euclidean_vector&&) noexcept -> euclidean_vector&;

		auto operator[](int const&) const -> double;
		auto operator[](int const&) -> double&;
		auto operator+() -> euclidean_vector;
		auto operator-() -> euclidean_vector;
		auto operator+=(euclidean_vector const&) -> euclidean_vector&;
		auto operator-=(euclidean_vector const&) -> euclidean_vector&;
		auto operator*=(double const&) -> euclidean_vector&;
		auto operator/=(double const&) -> euclidean_vector&;

		explicit operator std::vector<double>() const {
			auto temp = std::vector<double>{};
			for (size_t i = 0; i < static_cast<size_t>(dim_); i++) {
				temp.push_back(magnitudes_[i]);
			}
			return temp;
		}

		[[nodiscard]] auto at(int) const -> double;
		auto at(int) -> double&;
		[[nodiscard]] auto dimensions() const -> int;

		// 5. friends
		friend auto operator==(euclidean_vector const&, euclidean_vector const&) -> bool;
		friend auto operator!=(euclidean_vector const&, euclidean_vector const&) -> bool;
		friend auto operator+(euclidean_vector const&, euclidean_vector const&) -> euclidean_vector;
		friend auto operator-(euclidean_vector const&, euclidean_vector const&) -> euclidean_vector;
		friend auto operator*(euclidean_vector const&, double const&) -> euclidean_vector;
		friend auto operator*(double const&, euclidean_vector const&) -> euclidean_vector;
		friend auto operator/(euclidean_vector const&, double const&) -> euclidean_vector;
		friend auto operator<<(std::ostream&, euclidean_vector const&) -> std::ostream&;

		[[nodiscard]] auto is_cached() const -> bool;
		auto set_cache_norm(double) const;
		[[nodiscard]] auto get_cache_norm() const -> double;
		auto get_ptr() const -> double*;

	private:
		// ass2 spec requires we use double[]
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitudes_;
		size_t dim_ = 0;
		mutable double cached_norm_ = 0.0;
		mutable bool cache_flag_ = false;

		auto add_takeaway(euclidean_vector const&, bool) -> euclidean_vector&;
	};

	// 6. Utility functions
	auto euclidean_norm(euclidean_vector const& v) -> double;
	auto unit(euclidean_vector const& v) -> euclidean_vector;
	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;

} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
