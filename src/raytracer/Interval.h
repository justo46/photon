#pragma once 

#include "math_utils.h"

class Interval {
public:
    double min, max;

    constexpr Interval() noexcept : min(+infinity), max(-infinity) {}
    constexpr Interval(double min, double max) noexcept : min(min), max(max) {}

    [[nodiscard]] constexpr bool isEmpty() const noexcept { return min > max; }
    [[nodiscard]] constexpr double size() const noexcept { return isEmpty() ? 0.0 : max - min; }

    [[nodiscard]] constexpr bool contains(double value) const noexcept {
        return min <= value && value <= max;
    }

    [[nodiscard]] constexpr bool surrounds(double value) const noexcept {
        return min < value && value < max;
    }

    [[nodiscard]] constexpr double clamp(double value) const noexcept {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

	static const Interval empty;
	static const Interval universe;
};

inline constexpr Interval Interval::empty(+infinity, -infinity);
inline constexpr Interval Interval::universe(-infinity, +infinity);
