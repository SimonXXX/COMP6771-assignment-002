// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "../include/comp6771/euclidean_vector.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fmt/ostream.h>
#include <functional>
#include <iterator>
#include <numeric>
#include <stdexcept>
#include <sys/types.h>

namespace comp6771 {

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
		std::fill(magnitudes_.get(), magnitudes_.get() + dim_, magnitude);
	}

	// constructor - begin and end iterators
	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator begin,
	                                   std::vector<double>::const_iterator end)
	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	: magnitudes_(std::make_unique<double[]>(static_cast<size_t>(ranges::distance(begin, end))))
	, dim_{static_cast<size_t>(ranges::distance(begin, end))} {
		std::copy(begin, end, magnitudes_.get());
		//	size_t i = 0;
		//	for (auto it = begin; it != end; ++it) {
		//		magnitudes_[i++] = *it;
		//	}
	}

	// constructor - initialiser list
	euclidean_vector::euclidean_vector(std::initializer_list<double> l)
	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	: magnitudes_(std::make_unique<double[]>(static_cast<size_t>(l.size())))
	, dim_{static_cast<size_t>(l.size())} {
		// auto i = size_t{0};
		std::copy(l.begin(), l.end(), magnitudes_.get());
		//	for (auto const it : l) {
		//		magnitudes_[i++] = it;
		//	}
	}

	// COPY CONSTRUCTOR
	euclidean_vector::euclidean_vector(euclidean_vector const& orig)
	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	: magnitudes_(std::make_unique<double[]>(static_cast<size_t>(orig.dim_)))
	, dim_{orig.dim_} {
		std::copy(orig.magnitudes_.get(), orig.magnitudes_.get() + dim_, magnitudes_.get());
	};

	// MOVE CONSTRUCTOR
	euclidean_vector::euclidean_vector(euclidean_vector&& orig) noexcept
	: magnitudes_{std::move(orig.magnitudes_)}
	, dim_{orig.dim_} {
		orig.dim_ = 0;
		orig.magnitudes_ = nullptr;
	};

	auto euclidean_vector::operator=(euclidean_vector const& orig) -> euclidean_vector& {
		if (this != &orig) {
			dim_ = orig.dim_;
			magnitudes_.reset();
			// ass2 spec requires we use double[]
			// NOLINTNEXTLINE(modernize-avoid-c-arrays)
			magnitudes_ = (std::make_unique<double[]>(static_cast<size_t>(dim_)));
			std::copy(orig.magnitudes_.get(), orig.magnitudes_.get() + dim_, magnitudes_.get());
		}
		return *this;
	};

	auto euclidean_vector::operator=(euclidean_vector&& orig) noexcept -> euclidean_vector& {
		if (this != &orig) {
			magnitudes_ = std::move(orig.magnitudes_);
			dim_ = orig.dim_;
			orig.dim_ = 0;
			orig.magnitudes_ = nullptr;
		}
		return *this;
	};

	auto euclidean_vector::operator[](int const& i) const -> double {
		if (i < 0) {
			throw std::out_of_range("negative subscript");
		}
		if (static_cast<size_t>(i) >= dim_) {
			throw std::out_of_range("subscript too high");
		}
		return magnitudes_[static_cast<size_t>(i)];
	};

	auto euclidean_vector::operator[](int const& i) -> double& {
		if (i < 0) {
			throw std::out_of_range("negative subscript");
		}
		if (static_cast<size_t>(i) >= dim_) {
			throw std::out_of_range("subscript too high");
		}
		cache_flag_ = false;
		return magnitudes_[static_cast<size_t>(i)];
	};

	auto euclidean_vector::operator+() -> euclidean_vector {
		/*	auto v = std::vector<double>{};
		   for (size_t i = 0; i < dim_; i++) {
		      v.push_back(magnitudes_[i]);
		   }
		   return euclidean_vector(v.begin(), v.end());  */
		return euclidean_vector(*this);
	};

	auto euclidean_vector::operator-() -> euclidean_vector {
		auto negated = std::vector<double>(dim_);
		// for (size_t i = 0; i < dim_; i++) {
		//	negated.push_back(-magnitudes_[i]);
		//}
		std::transform(magnitudes_.get(), magnitudes_.get() + dim_, negated.begin(), std::negate<>());
		return euclidean_vector(negated.begin(), negated.end());
	};

	// operators +=  and  -=  plus helper function
	auto euclidean_vector::operator+=(euclidean_vector const& other) -> euclidean_vector& {
		return add_takeaway(other, true);
	};
	auto euclidean_vector::operator-=(euclidean_vector const& other) -> euclidean_vector& {
		return add_takeaway(other, false);
	};
	auto euclidean_vector::add_takeaway(euclidean_vector const& other, bool add) -> euclidean_vector& {
		if (dim_ != other.dim_) {
			throw std::length_error("euclidean_vectors are different magnitudes");
		}
		std::transform(magnitudes_.get(),
		               magnitudes_.get() + dim_,
		               other.magnitudes_.get(),
		               magnitudes_.get(),
		               [&](double a, double b) { return add ? a + b : a - b; });
		// for (size_t i = 0; i < dim_; i++) {
		//	magnitudes_[i] += add ? other.magnitudes_[i] : -other.magnitudes_[i];
		//}
		cache_flag_ = false;
		return *this;
	}

	// operators *=  and  /=
	auto euclidean_vector::operator*=(double const& multiplier) -> euclidean_vector& {
		std::transform(magnitudes_.get(), magnitudes_.get() + dim_, magnitudes_.get(), [&](double a) {
			return a * multiplier;
		});
		cache_flag_ = false;
		return *this;
	};
	auto euclidean_vector::operator/=(double const& divisor) -> euclidean_vector& {
		std::transform(magnitudes_.get(), magnitudes_.get() + dim_, magnitudes_.get(), [&](double a) {
			return a / divisor;
		});
		cache_flag_ = false;
		return *this;
	};

	auto euclidean_vector::at(int i) const -> double {
		if ((i < 0) or (static_cast<size_t>(i) >= dim_)) {
			throw std::length_error(
			   fmt::format("Index {} is not valid for this euclidean_vector object", i));
		}
		return magnitudes_[static_cast<size_t>(i)];
	};

	auto euclidean_vector::at(int i) -> double& {
		if ((i < 0) or (static_cast<size_t>(i) >= dim_)) {
			throw std::length_error(
			   fmt::format("Index {} is not valid for this euclidean_vector object", i));
		}
		cache_flag_ = false;
		return magnitudes_[static_cast<size_t>(i)];
	};

	auto euclidean_vector::dimensions() const -> int {
		return static_cast<int>(dim_);
	};

	// setters and getters
	// ===================
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
	auto euclidean_vector::get_ptr() const -> double* {
		return &magnitudes_[0];
	}
	// 5. Friends
	//===========

	auto operator==(euclidean_vector const& first, euclidean_vector const& second) -> bool {
		if (first.dim_ != second.dim_) {
			return false;
		}
		auto equal = true;
		auto epsilon = 0.01;
		for (auto i = 0; i < static_cast<int>(first.dim_) and equal; i++) {
			equal = std::fabs(first[i] - second[i]) < epsilon;
		}
		return equal;
	}

	auto operator!=(euclidean_vector const& first, euclidean_vector const& second) -> bool {
		return !(first == second);
	}

	auto operator+(euclidean_vector const& first, euclidean_vector const& second) -> euclidean_vector {
		if (first.dim_ != second.dim_) {
			throw std::out_of_range("vectors different sizes");
		}
		auto added_mags = std::vector<double>{};
		for (auto i = 0; i < static_cast<int>(first.dim_); i++) {
			added_mags.push_back(first[i] + second[i]);
		}

		return euclidean_vector(added_mags.begin(), added_mags.end());
	}

	auto operator-(euclidean_vector const& first, euclidean_vector const& second) -> euclidean_vector {
		if (first.dim_ != second.dim_) {
			throw std::out_of_range("vectors different sizes");
		}
		auto difference = std::vector<double>{};
		for (auto i = 0; i < static_cast<int>(first.dim_); i++) {
			difference.push_back(first[i] - second[i]);
		}

		return euclidean_vector(difference.begin(), difference.end());
	}

	auto operator*(euclidean_vector const& v, double const& multiplier) -> euclidean_vector {
		auto multiplied = std::vector<double>{};
		for (auto i = 0; i < static_cast<int>(v.dim_); i++) {
			multiplied.push_back(v[i] * multiplier);
		}
		return euclidean_vector(multiplied.begin(), multiplied.end());
	}

	auto operator*(double const& multiplier, euclidean_vector const& v) -> euclidean_vector {
		return v * multiplier;
	}

	auto operator/(euclidean_vector const& v, double const& divisor) -> euclidean_vector {
		auto divided = std::vector<double>{};
		for (auto i = 0; i < static_cast<int>(v.dim_); i++) {
			divided.push_back(v[i] / divisor);
		}
		return euclidean_vector(divided.begin(), divided.end());
	}

	auto operator<<(std::ostream& os, euclidean_vector const& v) -> std::ostream& {
		os << fmt::format("[{}]", fmt::join(static_cast<std::vector<double>>(v), " "));
		return os;
	}

	// 6. Utility functions
	auto euclidean_norm(euclidean_vector const& v) -> double {
		if (v.dimensions() == 0) {
			throw std::out_of_range("euclidian_vector with no dimensions does not have a norm");
		}
		if (!v.is_cached()) { // if cached norm value is valid return that otherwise calculate it
			auto vv = static_cast<std::vector<double>>(v);
			v.set_cache_norm(std::sqrt(std::inner_product(vv.begin(), vv.end(), vv.begin(), 0.0)));
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
			                        "unit "
			                        "vector");
		}
		return v / norm;
	}

	auto dot(euclidean_vector const& x, euclidean_vector const& y) -> double {
		if (x.dimensions() != y.dimensions()) {
			throw std::out_of_range(fmt::format("Dimensions of LHS({}}) and RHS({}}) do not match",
			                                    x.dimensions(),
			                                    y.dimensions()));
		}
		auto xv = std::vector<double>(x);
		auto yv = std::vector<double>(y);
		return std::inner_product(xv.begin(), xv.end(), yv.begin(), 0.0);
	}
} // namespace comp6771
/*	if (dim_ != other.dim_) {
      throw std::length_error("euclidean_vectors are different magnitudes");
   }
   for (size_t i = 0; i < dim_; i++) {
      magnitudes_[i] += other.magnitudes_[i];
   }
   cache_flag_ = false;
   return *this; */