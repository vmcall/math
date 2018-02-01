#pragma once
#include "stdafx.h"
#include "algebra.hpp"

// VECTOR
namespace geo
{
	template <typename T, size_t N, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	struct vector {
		template <typename ...R>
		vector(const R&&... args) : elements{ static_cast<T>(args)... } {}

		vector(vector&& args) = default;
		vector(const vector& args) = default;

		vector& operator=(const vector& args)
		{
			this->elements = args.elements;
			return *this;
		}

		// OVERLOADS
		template <typename my_type, size_t my_size>
		using common_vector_t = vector<std::common_type_t<T, my_type>, std::max(N, my_size)>;

		template <typename my_type, size_t my_size>
		auto operator +(const vector<my_type, my_size>& other) -> common_vector_t<my_type, my_size>
		{
			common_vector_t<my_type, my_size> vec = *this;

			for (std::size_t i = 0; i < std::min(N, my_size); ++i)
				vec.elements[i] = (i >= this->elements.size() ? 0 : this->elements[i]) + (i >= other.elements.size() ? 0 : other.elements[i]);

			return vec;
		}
		auto operator +(T arg) -> vector<T, N>
		{
			auto vec = *this;

			for (std::size_t i = 0; i < N; ++i)
				vec.elements[i] += arg;

			return vec;
		}
		template <typename my_type, size_t my_size>
		auto operator -(const vector<my_type, my_size>& other) -> common_vector_t<my_type, my_size>
		{
			common_vector_t<my_type, my_size> vec = *this;

			for (std::size_t i = 0; i < std::min(N, my_size); ++i)
				vec.elements[i] = (i >= this->elements.size() ? 0 : this->elements[i]) - (i >= other.elements.size() ? 0 : other.elements[i]);

			return vec;
		}
		auto operator -(T arg) -> vector<T, N>
		{
			auto vec = *this;

			for (std::size_t i = 0; i < N; ++i)
				vec.elements[i] -= arg;

			return vec;
		}
		template <typename my_type, size_t my_size>
		auto operator *(const vector<my_type, my_size>& other) -> common_vector_t<my_type, my_size>
		{
			common_vector_t<my_type, my_size> vec = *this;

			for (std::size_t i = 0; i < std::min(N, my_size); ++i)
				vec.elements[i] = (i >= this->elements.size() ? 0 : this->elements[i]) * (i >= other.elements.size() ? 0 : other.elements[i]);

			return vec;
		}
		template <typename my_type, size_t my_size>
		auto operator *=(const vector<my_type, my_size>& other) -> vector<T, N>
		{
			*this = *this * other;
			return *this;
		}
		auto operator *(T arg) -> vector<T, N>
		{
			auto vec = *this;

			for (std::size_t i = 0; i < N; ++i)
				vec.elements[i] *= arg;

			return vec;
		}
		auto operator *=(T arg) -> vector<T, N>
		{
			*this = *this * arg;
			return *this;
		}

		template <typename my_type, size_t my_size>
		auto operator /(const vector<my_type, my_size>& other) -> common_vector_t<my_type, my_size>
		{
			common_vector_t<my_type, my_size> vec = *this;

			for (std::size_t i = 0; i < std::min(N, my_size); ++i)
				vec.elements[i] = (i >= this->elements.size() ? 1 : this->elements[i]) / (i >= other.elements.size() ? 1 : other.elements[i]);

			return vec;
		}
		auto operator /(T arg) -> vector<T, N>
		{
			auto vec = *this;

			for (std::size_t i = 0; i < N; ++i)
				vec.elements[i] /= arg;

			return vec;
		}
		auto operator /=(T arg) -> vector<T, N>
		{
			for (std::size_t i = 0; i < N; ++i)
				this->elements[i] /= arg;

			return *this;
		}

		// HELPERS
		template <typename = std::enable_if_t<N >= 1>>
		auto x() -> T&
		{
			return elements[0];
		}
		template <typename = std::enable_if_t<N >= 2>>
		auto y() -> T&
		{
			return elements[1];
		}
		template <typename = std::enable_if_t<N >= 3>>
		auto z() -> T&
		{
			return elements[2];
		}
		template <typename = std::enable_if_t<N >= 4>>
		auto w() -> T&
		{
			return elements[3];
		}
		
		// MATHEMATICAL HELPERS
		auto length() -> T
		{
			T tmp{};
			std::for_each(this->elements.begin(), this->elements.end(), [&tmp](T x) { tmp += x * x; });

			return static_cast<T>(sqrt(tmp));
		};
		auto distance(const geo::vector<T, N>& other) -> T
		{
			T tmp{};

			for (size_t i = 0; i < N; i++)
				tmp += (this->elements[i] - other.elements[i]) * (this->elements[i] - other.elements[i]);

			return static_cast<T>(sqrt(tmp));
		}
		auto mid_point(const geo::vector<T, N>& other) -> geo::vector<T, N>
		{
			vector<T, N> vec;

			for (size_t i = 0; i < N; i++)
				vec.elements[i] = (this->elements[i] + other.elements[i]) / 2;

			return vec;
		}
		auto dot_product(const geo::vector<T, N>& other) -> T
		{
			T product{};

			for (size_t i = 0; i < N; i++)
				product += this->elements[i] * other.elements[i];

			return product;
		}

		// 2D
		auto cross_product_2d(geo::vector<T, N>& other) -> T
		{
			return (this->x() * other.y()) - (other.x() * this->y());
		}
		auto equation_2d(geo::vector<T, N>& other) -> alg::linear_equation<T>
		{
			auto midpoint = this->mid_point(other);
			auto delta = *this - other;

			auto slope = delta.y() / delta.x();

			auto b = midpoint.y() - slope * midpoint.x();

			std::cout << "[Midpoint] " << midpoint.x() << ";" << midpoint.y() << std::endl;
			std::cout << "[Delta] " << delta.x() << ";" << delta.y() << std::endl;
			std::cout << "[Slope] " << slope << std::endl;
			std::cout << "[B] " << b << std::endl;

			return alg::linear_equation<T>(slope, b);
		}

		private:
			std::array<T, N> elements{};
	};

	template <typename T, size_t N>
	struct line {

		template <typename ...R>
		line(R&&... args) : points{ static_cast<geo::vector<T, N, void>>(args)... } {}

		line(line&& args) = default;
		line(line& args) = default;

		line& operator=(const line& args)
		{
			this->points = args.points;
			return *this;
		}

		// ACCESSORS
		using my_vec_t = geo::vector<T, N>;
		auto origin() -> my_vec_t&
		{
			return points[0];
		}
		auto end() -> my_vec_t&
		{
			return points[1];
		}
		auto delta() -> my_vec_t
		{
			return this->end() - this->origin();
		}

		// MATHEMATICAL HELPERS
		auto length() -> T
		{
			return this->origin().distance(this->end());
		}
		auto intersection_2d(line<T, N>& other, my_vec_t* out) -> bool
		{
			// WIKIPEDIA - LINE INTERSECTION
			auto denominator = this->delta().x() * other.delta().y() - this->delta().y() * other.delta().x();

			if (denominator == static_cast<T>(0))
				return false;	

			auto x_1 = (this->origin().x() * this->end().y() - this->origin().y() * this->end().x()) * other.delta().x() - this->delta().x() * (other.origin().x() * other.end().y() - other.origin().y() * other.end().x());
			auto y_1 = (this->origin().x() * this->end().y() - this->origin().y() * this->end().x()) * other.delta().y() - this->delta().y() * (other.origin().x() * other.end().y() - other.origin().y() * other.end().x());
			auto determinant = this->delta().x() * other.delta().y() - this->delta().y() * other.delta().x();

			*out = my_vec_t(-(x_1 / determinant), -(y_1 / determinant));

			return true;
		}

	private:
		std::array<my_vec_t, 2> points{};
	};
	
	template <typename T, size_t N>
	struct polygon {

		template <typename ...R>
		polygon(R&&... args) : elements{ args... } {}

		polygon(polygon&& args) = default;
		polygon(polygon& args) = default;

		polygon& operator=(const polygon& args)
		{
			this->elements = args.elements;
			return *this;
		}

		using my_vec_t = geo::vector<T, N>;
		using my_line_t = geo::line<T, N>;

		// MATHEMATICAL HELPERS
		auto midpoints() -> std::vector<my_vec_t>
		{
			std::vector<my_vec_t> pts;

			for (size_t i = 0; i < elements.size(); i++)
				pts.emplace_back(this->elements.at(i).mid_point(this->elements.at((i + 1) % elements.size())));

			return pts;
		}
		auto sides() -> std::vector<my_line_t>
		{
			std::vector<my_line_t> side_vec;

			for (size_t i = 0; i < elements.size(); i++)
			{
				auto current_point = this->elements.at(i);
				auto next_point = this->elements.at((i + 1) % elements.size());
				side_vec.emplace_back(current_point, next_point);
			}

			return side_vec;
		}
		auto side_equation_2d() -> std::vector<alg::linear_equation<T>>
		{
			std::vector<alg::linear_equation<T>> result;

			for (size_t i = 0; i < elements.size(); i++)
			{
				auto current_point = this->elements.at(i);
				auto next_point = this->elements.at((i + 1) % elements.size());

				result.emplace_back(current_point.equation_2d(next_point));
			}
			
			return result;
		}
		auto area() -> T
		{
			T result{};
			auto previous_point = this->elements.at(this->elements.size() - 1);
			for (size_t i = 0; i < elements.size(); i++)
			{
				auto current_point = this->elements.at(i);

				// SHOELACE FORMULA
				result += (previous_point.x() + current_point.x()) * (previous_point.y() - current_point.y());

				previous_point = current_point;

			}
			return abs(result) / 2;
		}
		auto circumference() -> T
		{	
			T result;
			std::for_each(this->sides().begin(), this->sides().end(), [&result](my_line_t side) { result += side.length() });
			return result;
		}
		auto angles() -> std::vector<T>
		{
			std::vector<T> angles;

			for (size_t i = 0; i < elements.size(); i++)
			{
				polygon<T, N> new_triangle{ this->elements[i], this->elements[(i + 1) % elements.size()], this->elements[(i + 2) % elements.size()] };

				// LAW OF COSINUS
				auto side_a = new_triangle.sides()[0].length();
				auto side_b = new_triangle.sides()[1].length();
				auto side_c = new_triangle.sides()[2].length();

				const auto cos0 = (pow(side_a, 2) + pow(side_b, 2) - pow(side_c, 2)) / (2 * side_a * side_b);
				angles.emplace_back(acos(cos0) * 180 / M_PI);
			}

			return angles;
		}
		auto slopes() -> std::vector<T>
		{
			std::vector<T> slopes;

			for (size_t i = 0; i < elements.size(); i++)
			{
				auto current_point = this->elements[i];
				auto next_point = this->elements[(i + 1) % elements.size()];
				auto delta = current_point - next_point;

				// delta_y / delta_x
				slopes.emplace_back(delta.y() / delta.x());
			}

			return slopes;
		}
		auto perpendicular_slopes() -> std::vector<T>
		{
			std::vector<T> slopes = this->slopes();

			std::for_each(slopes.begin(), slopes.end(), [](T& slope) {slope = -1 / slope; });

			return slopes;
		}
		auto circumcenter() -> my_vec_t
		{
			my_vec_t result;

			auto midpoints = this->midpoints();
			auto slopes = this->perpendicular_slopes();

			// y-y1 = m(x-x1)
			// y = y1 + mx + mx1

			return result;
		}

		// TRIANGLES ONLY
		auto triangle_medians() -> std::vector<T>
		{
			std::vector<T> medians;

			if (elements.size() != 3)
				return medians;

			for (size_t i = 0; i < elements.size(); i++)
			{
				auto side_ab = this->sides().at(i).length();
				auto side_bc = this->sides().at((i + 1) % elements.size()).length();
				auto side_ac = this->sides().at((i + 2) % elements.size()).length();

				// AM^2 = AB^2/2 + AC^2/2 - BC^2/4
				auto am = sqrt((pow(side_ab, 2)) / 2 + (pow(side_ac, 2)) / 2 - (pow(side_bc, 2)) / 4);

				medians.emplace_back(am);
			}

			return medians;
		}

	private:
		std::vector<my_vec_t> elements{};
	};
}

using vector2i_t = geo::vector<int32_t, 2>;
using polyi_t = geo::polygon<int32_t, 2>;
using linei_t = geo::line<int32_t, 2>;
using vector2d_t = geo::vector<double, 2>;
using polyd_t = geo::polygon<double, 2>;
using lined_t = geo::line<double, 2>;
