#pragma once
#include "stdafx.h"

namespace statistics
{
	template <typename T, size_t N>
	constexpr auto get_max(const std::array<T, N>& data) -> T
	{
		return *std::max_element(data.begin(), data.end());
	}

	template <typename T, size_t N>
	constexpr auto get_min(const std::array<T, N>& data) -> T
	{
		return *std::min_element(data.begin(), data.end());
	}

	template <typename T, size_t N>
	constexpr auto get_average(const std::array<T, N>& data) -> T
	{
		auto sum = std::accumulate(data.begin(), data.end(), 0);
		return sum / static_cast<T>(N);
	}

	template <typename T, size_t N>
	constexpr auto get_variance(const std::array<T, N>& data) -> T
	{
		auto variance = std::numeric_limits<T>::min();
		auto average = statistics::get_average(data);
		auto count = static_cast<T>(N);

		std::for_each(data.begin(), data.end(), [&variance, &average](T x) { variance += (x - average) * (x - average); });

		return static_cast<T>(1) / (static_cast<T>(1) + N) * variance;
	}
}