// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "../include/comp6771/euclidean_vector.hpp"
#include <algorithm>
#include <cstddef>
#include <fmt/ostream.h>
#include <iterator>
#include <stdexcept>
#include <sys/types.h>

namespace comp6771 {
	using std::vector;

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
		for (size_t i = 0; i < static_cast<size_t>(dim); i++) {
			magnitudes_[i] = magnitude;
		}
	}

	// constructor - begin and end iterators
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

	// constructor - initialiser list
	euclidean_vector::euclidean_vector(std::initializer_list<double> l)
	// ass2 spec requires we use double[]
	// NOLINTNEXTLINE(modernize-avoid-c-arrays)
	: magnitudes_(std::make_unique<double[]>(static_cast<size_t>(l.size())))
	, dim_{static_cast<size_t>(l.size())} {
		auto i = size_t{0};
		for (auto const it : l) {
			magnitudes_[i++] = it;
		}
	};

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

	auto euclidean_vector::at(int i) const -> double {
		if ((i < 0) or (static_cast<size_t>(i) >= dim_)) {
			throw std::length_error("Index X is not valid for this euclidean_vector object");
		}
		return magnitudes_[static_cast<size_t>(i)];
	};

	auto euclidean_vector::at(int i) -> double& {
		if ((i < 0) or (static_cast<size_t>(i) >= dim_)) {
			throw std::length_error("Index X is not valid for this euclidean_vector object");
		}
		return magnitudes_[static_cast<size_t>(i)];
	};
	auto euclidean_vector::dimensions() const -> int {
		return static_cast<int>(dim_);
	};

	// 5. Friends
	//===========

	auto operator==(euclidean_vector const& first, euclidean_vector const& second) -> bool {
		if (first.dim_ != second.dim_) {
			return false;
		}
		auto equal = true;
		for (auto i = 0; i < static_cast<int>(first.dim_) or !equal; i++) {
			equal = first[i] == second[i];
		}
		return equal;
	}

	auto operator!=(euclidean_vector const& first, euclidean_vector const& second) -> bool {
		if (first.dim_ != second.dim_) {
			return true;
		}
		auto equal = true;
		for (auto i = 0; i < static_cast<int>(first.dim_) or !equal; i++) {
			equal = first[i] == second[i];
		}
		return equal;
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

	auto operator*(euclidean_vector const& evec, double const& multiplier) -> euclidean_vector {
		auto multiplied = std::vector<double>{};
		for (auto i = 0; i < static_cast<int>(evec.dim_); i++) {
			multiplied.push_back(evec[i] * multiplier);
		}
		return euclidean_vector(multiplied.begin(), multiplied.end());
	};

	auto operator*(double const& multiplier, euclidean_vector const& evec) -> euclidean_vector {
		return evec * multiplier;
	};

	auto operator/(euclidean_vector const& evec, double const& divisor) -> euclidean_vector {
		auto divided = std::vector<double>{};
		for (auto i = 0; i < static_cast<int>(evec.dim_); i++) {
			divided.push_back(evec[i] / divisor);
		}
		return euclidean_vector(divided.begin(), divided.end());
	};
	auto operator<<(std::ostream& os, euclidean_vector const& evec) -> std::ostream& {
		os << fmt::format("[{}]", fmt::join(static_cast<vector<double>>(evec), " "));
		return os;
	}

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
