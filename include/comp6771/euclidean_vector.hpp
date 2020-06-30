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
		euclidean_vector(int dim);
		euclidean_vector(int dim, double magnitude);
		auto test() -> void;

	private:
		std::vector<double> coords_;
	};
} // namespace comp6771
#endif // COMP6771_EUCLIDEAN_VECTOR_HPP
