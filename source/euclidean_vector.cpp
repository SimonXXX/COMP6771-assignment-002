// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "../include/comp6771/euclidean_vector.hpp"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <sys/types.h>

namespace comp6771 {
	// default constructor
	euclidean_vector::euclidean_vector()
	: euclidean_vector::euclidean_vector(1, 0.0) {}

	// single argument constructor
	euclidean_vector::euclidean_vector(int dim)
	: euclidean_vector::euclidean_vector(dim, 0.0) {}

	//
	euclidean_vector::euclidean_vector(int dim, double magnitude)
	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	: magnitudes_(std::make_unique<double[]>(static_cast<size_t>(dim)))
	, dim_{static_cast<size_t>(dim)} {
		for (ulong i = 0; i < static_cast<ulong>(dim); i++) {
			magnitudes_[i] = magnitude;
		}
	}

	euclidean_vector::euclidean_vector(std::vector<double>::const_iterator begin,
	                                   std::vector<double>::const_iterator end)
	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	: magnitudes_(std::make_unique<double[]>(static_cast<size_t>(ranges::distance(begin, end))))
	, dim_{static_cast<size_t>(ranges::distance(begin, end))} {
		size_t i = 0;
		for (auto it = begin; it != end; ++it) {
			magnitudes_[i++] = *it;
		}
	}

	auto euclidean_vector::get_coords() -> double* {
		return magnitudes_.get();
	};
	auto euclidean_vector::get_dim() const -> size_t {
		return dim_;
	};

	// COPY CONSTRUCTOR
	euclidean_vector::euclidean_vector(euclidean_vector const& orig)
	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	: magnitudes_(std::make_unique<double[]>(static_cast<size_t>(orig.dim_)))
	, dim_{orig.dim_} {
		for (ulong i = 0; i < static_cast<ulong>(dim_); i++) {
			magnitudes_[i] = orig.magnitudes_[i];
		}
	};

	auto euclidean_vector::operator=(euclidean_vector const& orig) -> euclidean_vector& {
		if (this != &orig) {
			dim_ = orig.dim_;
			magnitudes_.reset();
			// NOLINTNEXTLINE(modernize-avoid-c-arrays)
			magnitudes_ = (std::make_unique<double[]>(static_cast<size_t>(dim_)));
			memcpy(magnitudes_.get(), orig.magnitudes_.get(), dim_ * sizeof(double));
			// for (ulong i = 0; i < static_cast<ulong>(dim_); i++) {
			//	magnitudes_[i] = orig.magnitudes_[i];
			//}
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
		return magnitudes_[static_cast<size_t>(i)];
	};

	auto euclidean_vector::operator+() -> euclidean_vector {
		auto temp = std::vector<double>{};
		// memcpy(magnitudes_.get(), magnitudes_.get(), dim_ * sizeof(double));
		for (size_t i = 0; i < static_cast<ulong>(dim_); i++) {
			temp.push_back(magnitudes_[i]);
		}
		return euclidean_vector(temp.begin(), temp.end());
	};

	auto euclidean_vector::operator-() -> euclidean_vector {
		auto temp = std::vector<double>{};
		// memcpy(magnitudes_.get(), magnitudes_.get(), dim_ * sizeof(double));
		for (size_t i = 0; i < static_cast<ulong>(dim_); i++) {
			temp.push_back(-magnitudes_[i]);
		}
		return euclidean_vector(temp.begin(), temp.end());
	};

	auto euclidean_vector::operator+=(euclidean_vector const& other) -> euclidean_vector& {
		if (dim_ != other.dim_) {
			throw std::length_error("euclidean_vectors are different magnitudes");
		}
		for (size_t i = 0; i < static_cast<size_t>(dim_); i++) {
			magnitudes_[i] += other.magnitudes_[i];
		}
		return *this;
	};
	auto euclidean_vector::operator-=(euclidean_vector const& other) -> euclidean_vector& {
		if (dim_ != other.dim_) {
			throw std::length_error("euclidean_vectors are different magnitudes");
		}
		for (size_t i = 0; i < static_cast<size_t>(dim_); i++) {
			magnitudes_[i] -= other.magnitudes_[i];
		}
		return *this;
	};

	auto euclidean_vector::operator*=(euclidean_vector const& other) -> euclidean_vector& {
		if (dim_ != other.dim_) {
			throw std::length_error("euclidean_vectors are different magnitudes");
		}
		for (size_t i = 0; i < static_cast<size_t>(dim_); i++) {
			magnitudes_[i] *= other.magnitudes_[i];
		}
		return *this;
	};
	auto euclidean_vector::operator/=(euclidean_vector const& other) -> euclidean_vector& {
		if (dim_ != other.dim_) {
			throw std::length_error("euclidean_vectors are different magnitudes");
		}
		for (size_t i = 0; i < static_cast<size_t>(dim_); i++) {
			magnitudes_[i] /= other.magnitudes_[i];
		}
		return *this;
	};

	/*auto euclidean_vector::swap(euclidean_vector& other) -> euclidean_vector& {
	      std::swap(dim_, other.dim_);
	   magnitudes_.reset();
	   // NOLINTNEXTLINE(modernize-avoid-c-arrays)
	   magnitudes_ = (std::make_unique<double[]>(static_cast<size_t>(dim_)));
	   for (ulong i = 0; i < static_cast<ulong>(dim_); i++) {
	      magnitudes_[i] = other.magnitudes_[i];
	   }
	}; */

} // namespace comp6771
