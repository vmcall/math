#pragma once
#include "stdafx.h"

namespace alg
{
	template <typename T>
	struct linear_equation
	{
		// THIS IS A CLASS FOR EQUATIONS THAT ARE STRUCTURED
		// f(x) = ax + b

		linear_equation(T slope, T y_intercept) : a(slope), b(y_intercept) {};

		// ACCESSORS
		auto slope() -> T&
		{
			return this->a;
		}
		auto y_intercept() -> T&
		{
			return this->b;
		}

		auto evaluate(T x) -> T
		{
			return a * x + b;
		}

		// HELPERS
		auto formula() -> std::string
		{
			char buffer[25];
			sprintf_s(buffer, 25, "f(x) = %0.2lfx + %0.2lf", this->slope(), this->y_intercept());

			return std::string(buffer);
		}

	private:
		T a;
		T b;
	};
}