#pragma once 

#include <limits>

class Interval {
public:
    double min, max;

    constexpr Interval() noexcept : min(+std::numeric_limits<double>::infinity()), max(-std::numeric_limits<double>::infinity()) {}
    constexpr Interval(double min, double max) noexcept : min(min), max(max) {}

    [[nodiscard]] constexpr bool isEmpty() const noexcept { return min > max; }
    [[nodiscard]] constexpr double size() const noexcept { return isEmpty() ? 0.0 : max - min; }

    [[nodiscard]] constexpr bool contains(double value) const noexcept {
        return min <= value && value <= max;
    }

    [[nodiscard]] constexpr bool surrounds(double value) const noexcept {
        return min < value && value < max;
    }

	static const Interval empty;
	static const Interval universe;
};

inline const Interval Interval::empty(+std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());

inline const Interval Interval::universe(-std::numeric_limits<double>::infinity(), +std::numeric_limits<double>::infinity());
