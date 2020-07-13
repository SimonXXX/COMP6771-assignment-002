// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "../include/comp6771/euclidean_vector.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>

#include <functional>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <sys/types.h>

namespace comp6771 {
	//
	// CONSTRUCTORS (ass2 section 1)
	// =============================
	//
	// default constructor
	euclidean_vector::euclidean_vector()
	: euclidean_vector::euclidean_vector(1, 0.0) {}

	// single argument constructor
	euclidean_vector::euclidean_vector(int dim)
	: euclidean_vector::euclidean_vector(dim, 0.0) {}

	// constructor - size & default value
	euclidean_vector::euclidean_vector(int dim, double magnitude)
	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	: magnitudes_(std::make_unique<double[]>(static_cast<size_t>(dim)))
	, dim_{static_cast<size_t>(dim)} {
		std::fill(this->cbegin(), this->cend(), magnitude);
	}

	// constructor - begin and end iterators
	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator begin,
	                                   std::vector<double>::const_iterator end)
	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	: magnitudes_(std::make_unique<double[]>(static_cast<size_t>(ranges::distance(begin, end))))
	, dim_{static_cast<size_t>(ranges::distance(begin, end))} {
		std::copy(begin, end, this->cbegin());
	}

	// constructor - initialiser list
	euclidean_vector::euclidean_vector(std::initializer_list<double> l)
	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	: magnitudes_(std::make_unique<double[]>(static_cast<size_t>(l.size())))
	, dim_{static_cast<size_t>(l.size())} {
		std::copy(l.begin(), l.end(), this->cbegin());
	}

	// copy constructor
	euclidean_vector::euclidean_vector(euclidean_vector const& orig)
	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	: magnitudes_(std::make_unique<double[]>(static_cast<size_t>(orig.dim_)))
	, dim_{orig.dim_} {
		std::copy(orig.cbegin(), orig.cend(), this->cbegin());
	}

	// move constructor
	euclidean_vector::euclidean_vector(euclidean_vector&& orig) noexcept
	: magnitudes_{std::move(orig.magnitudes_)}
	, dim_{orig.dim_} {
		orig.dim_ = 0;
		orig.magnitudes_ = nullptr;
	}

	// OPERATIONS (ass2 section 3)
	// ===========================================
	//
	// copy assignment
	auto euclidean_vector::operator=(euclidean_vector const& orig) -> euclidean_vector& {
		if (this != &orig) {
			auto copy = euclidean_vector(orig);
			std::swap(copy, *this);
			return *this;
		}
		return *this;
	}

	// move assignment
	auto euclidean_vector::operator=(euclidean_vector&& orig) noexcept -> euclidean_vector& {
		if (this != &orig) {
			magnitudes_ = std::move(orig.magnitudes_);
			dim_ = orig.dim_;
			orig.dim_ = 0;
			orig.magnitudes_ = nullptr;
		}
		return *this;
	}

	// subscript operator[] - const
	auto euclidean_vector::operator[](int const& i) const -> double {
		if ((i < 0) or (static_cast<size_t>(i) >= dim_)) {
			throw std::out_of_range(
			   fmt::format("Index {} is not valid for this euclidean_vector object", i));
		}
		return magnitudes_[static_cast<size_t>(i)];
	}
	// subscript operator[]
	auto euclidean_vector::operator[](int const& i) -> double& {
		if ((i < 0) or (static_cast<size_t>(i) >= dim_)) {
			throw std::out_of_range(
			   fmt::format("Index {} is not valid for this euclidean_vector object", i));
		}
		cache_flag_ = false;
		return magnitudes_[static_cast<size_t>(i)];
	}

	// unary plus
	auto euclidean_vector::operator+() const -> euclidean_vector {
		return euclidean_vector(*this);
	}

	// unary negation
	auto euclidean_vector::operator-() const -> euclidean_vector {
		auto negated = std::vector<double>(dim_);
		std::transform(this->cbegin(), this->cend(), negated.begin(), std::negate<>());
		return euclidean_vector(negated.begin(), negated.end());
	}

	// compound operators +=  and  -=  plus helper function
	auto euclidean_vector::operator+=(euclidean_vector const& other) -> euclidean_vector& {
		return add_minus(other, true);
	}
	auto euclidean_vector::operator-=(euclidean_vector const& other) -> euclidean_vector& {
		return add_minus(other, false);
	}
	auto euclidean_vector::add_minus(euclidean_vector const& other, bool add) -> euclidean_vector& {
		if (dim_ != other.dim_) {
			throw std::length_error("euclidean_vectors are different magnitudes");
		}
		std::transform(this->cbegin(),
		               this->cend(),
		               other.cbegin(),
		               this->cbegin(),
		               [&](double a, double b) { return add ? a + b : a - b; });
		cache_flag_ = false;
		return *this;
	}

	// compound operators *=  and  /=
	auto euclidean_vector::operator*=(double const& multiplier) -> euclidean_vector& {
		auto div = [&](double x) { return x * multiplier; };
		std::transform(this->cbegin(), this->cend(), this->cbegin(), div);
		cache_flag_ = false;
		return *this;
	}
	auto euclidean_vector::operator/=(double const& divisor) -> euclidean_vector& {
		auto div = [&](double x) { return x / divisor; };
		std::transform(this->cbegin(), this->cend(), this->cbegin(), div);
		cache_flag_ = false;
		return *this;
	}

	// MEMBER FUNCTIONS (ass2 section 4)
	// ===========================================
	//
	auto euclidean_vector::at(int i) const -> double {
		if ((i < 0) or (static_cast<size_t>(i) >= dim_)) {
			throw std::length_error(
			   fmt::format("Index {} is not valid for this euclidean_vector object", i));
		}
		return magnitudes_[static_cast<size_t>(i)];
	}

	auto euclidean_vector::at(int i) -> double& {
		if ((i < 0) or (static_cast<size_t>(i) >= dim_)) {
			throw std::length_error(
			   fmt::format("Index {} is not valid for this euclidean_vector object", i));
		}
		cache_flag_ = false;
		return magnitudes_[static_cast<size_t>(i)];
	}

	auto euclidean_vector::dimensions() const -> int {
		return static_cast<int>(dim_);
	};

	// SETTERS AND GETTERS (for cached norm functionality)
	// ===================================================
	//
	auto euclidean_vector::is_cached() const -> bool {
		return cache_flag_;
	}
	auto euclidean_vector::set_cache_norm(double new_norm) const {
		cache_flag_ = true;
		cached_norm_ = new_norm;
	}
	auto euclidean_vector::get_cache_norm() const -> double {
		return cached_norm_;
	}

	// UTILITY FUNCTIONS (ass2 section 6)
	// ===========================================
	//
	auto euclidean_norm(euclidean_vector const& v) -> double {
		if (v.dimensions() == 0) {
			throw std::out_of_range("euclidian_vector with no dimensions does not have a norm");
		}
		if (!v.is_cached()) { // if cached norm value is valid return that otherwise calculate it
			v.set_cache_norm(std::sqrt(std::transform_reduce(v.cbegin(), v.cend(), v.cbegin(), 0.0)));
		}
		return v.get_cache_norm();
	}

	auto unit(euclidean_vector const& v) -> euclidean_vector {
		if (v.dimensions() == 0) {
			throw std::out_of_range("euclidean_vector with no dimensions does not have a unit "
			                        "vector");
		}
		auto norm = euclidean_norm(v);
		if (norm == 0.0) {
			throw std::out_of_range("euclidean_vector with zero euclidean normal does not have a "
			                        "unit vector");
		}
		return v / norm;
	}

	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double {
		if (x.dimensions() != y.dimensions()) {
			throw std::out_of_range(fmt::format("Dimensions of LHS({}}) and RHS({}}) do not match",
			                                    x.dimensions(),
			                                    y.dimensions()));
		}
		return std::inner_product(x.cbegin(), x.cend(), y.cbegin(), 0.0);
	}
} // namespace comp6771
