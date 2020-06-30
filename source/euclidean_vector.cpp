// Copyright (c) Christopher Di Bella.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
#include "../include/comp6771/euclidean_vector.hpp"

namespace comp6771 {
	// default constructor
	euclidean_vector::euclidean_vector()
	: coords_(0.0) {}
	// single argument constructor
	euclidean_vector::euclidean_vector(int dim) {
		for (auto i = 0; i < dim; i++) {
			coords_.push_back(0.0);
		}
	}
	//
	euclidean_vector::euclidean_vector(int dim, double magnitude) {
		for (auto i = 0; i < dim; i++) {
			coords_.push_back(magnitude);
		}
	}
	//
	//
	//
	auto euclidean_vector::test() -> void {
		std::cout << "hello" << coords_[0] << std::endl;
	};
	// Implement solution herestd:vector<double> coords_
} // namespace comp6771
