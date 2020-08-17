#ifndef COMP6771_EUCLIDEAN_VECTOR_HPP
#define COMP6771_EUCLIDEAN_VECTOR_HPP

// COMP6771 - Assignment 2 - Simon Garrod - z326122
//
#include <fmt/ostream.h>
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
		// constructors/destrictor
		euclidean_vector();
		explicit euclidean_vector(int dim);
		euclidean_vector(int dim, double magnitude);
		euclidean_vector(std::vector<double>::const_iterator begin,
		                 std::vector<double>::const_iterator end);
		euclidean_vector(std::initializer_list<double>);
		euclidean_vector(euclidean_vector const&);
		euclidean_vector(euclidean_vector&&) noexcept;
		~euclidean_vector() = default;

		// move/copy assigners
		auto operator=(euclidean_vector const&) -> euclidean_vector&;
		auto operator=(euclidean_vector&&) noexcept -> euclidean_vector&;

		// operators
		auto operator[](int const&) const -> double;
		auto operator[](int const&) -> double&;
		auto operator+() const -> euclidean_vector;
		auto operator-() const -> euclidean_vector;
		auto operator+=(euclidean_vector const&) -> euclidean_vector&;
		auto operator-=(euclidean_vector const&) -> euclidean_vector&;
		auto operator*=(double const&) -> euclidean_vector&;
		auto operator/=(double const&) -> euclidean_vector&;

		// member functions
		[[nodiscard]] auto at(int) const -> double;
		auto at(int) -> double&;
		[[nodiscard]] auto dimensions() const -> int;
		auto cbegin() const -> double* {
			return magnitudes_.get();
		}
		auto cend() const -> double* {
			return magnitudes_.get() + dim_;
		}

		// member functions (cached norm value functionality)
		[[nodiscard]] auto is_cached() const -> bool;
		auto set_cache_norm(double) const;
		[[nodiscard]] auto get_cache_norm() const -> double;

		// explicit type conversion operators
		explicit operator std::vector<double>() const {
			auto v = std::vector<double>(dim_);
			std::copy(this->cbegin(), this->cend(), v.begin());
			return v;
		}
		explicit operator std::list<double>() const {
			auto l = std::list<double>(dim_);
			std::copy(this->cbegin(), this->cend(), l.begin());
			return l;
		}

		// friend functions
		auto friend operator==(euclidean_vector const& v1, euclidean_vector const& v2) -> bool {
			if (v1.dim_ != v2.dim_) {
				return false;
			}
			auto epsilon = 0.000001;
			return std::equal(v1.cbegin(), v1.cend(), v2.cbegin(), [&](double const& a, double const& b) {
				return std::fabs(a - b) < epsilon;
			});
		}

		auto friend operator!=(euclidean_vector const& first, euclidean_vector const& second) -> bool {
			return !(first == second);
		}

		auto friend operator+(euclidean_vector const& first, euclidean_vector const& second)
		   -> euclidean_vector {
			if (first.dim_ != second.dim_) {
				throw std::out_of_range("vectors different sizes");
			}
			auto added = std::vector<double>(first.dim_);
			std::transform(first.cbegin(), first.cend(), second.cbegin(), added.begin(), std::plus<>{});
			return euclidean_vector(added.begin(), added.end());
		}

		auto friend operator-(euclidean_vector const& first, euclidean_vector const& second)
		   -> euclidean_vector {
			if (first.dim_ != second.dim_) {
				throw std::out_of_range("vectors different sizes");
			}
			auto minus = std::vector<double>(first.dim_);
			std::transform(first.cbegin(), first.cend(), second.cbegin(), minus.begin(), std::minus<>{});
			return euclidean_vector(minus.begin(), minus.end());
		}

		auto friend operator*(euclidean_vector const& v, double const& multiplier) -> euclidean_vector {
			auto multiplied = std::vector<double>(v.dim_);
			std::transform(v.cbegin(), v.cend(), multiplied.begin(), [&](double const& a) {
				return a * multiplier;
			});
			return euclidean_vector(multiplied.begin(), multiplied.end());
		}

		auto friend operator*(double const& multiplier, euclidean_vector const& v) -> euclidean_vector {
			return v * multiplier;
		}

		auto friend operator/(euclidean_vector const& v, double const& divisor) -> euclidean_vector {
			auto divided = std::vector<double>(v.dim_);
			std::transform(v.cbegin(), v.cend(), divided.begin(), [&](double const& a) {
				return a / divisor;
			});
			return euclidean_vector(divided.begin(), divided.end());
		}

		auto friend operator<<(std::ostream& os, euclidean_vector const& v) -> std::ostream& {
			os << fmt::format("[{:1}]", fmt::join(static_cast<std::vector<double>>(v), " "));
			return os;
		}

	private:
		// ass2 spec requires we use double[]
		// NOLINTNEXTLINE(modernize-avoid-c-arrays)
		std::unique_ptr<double[]> magnitudes_;
		size_t dim_ = 0;
		mutable double cached_norm_ = 0.0;
		mutable bool cache_flag_ = false;
		// helper function for the compound addition and subtraction operators
		auto add_minus(euclidean_vector const&, bool) -> euclidean_vector&;
	};

	// 6. Utility functions
	auto euclidean_norm(euclidean_vector const& v) -> double;
	auto unit(euclidean_vector const& v) -> euclidean_vector;
	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double;

} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
