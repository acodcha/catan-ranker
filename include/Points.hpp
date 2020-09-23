#pragma once

#include "Base.hpp"

namespace CatanLeaderboard {

class Points {

public:

  constexpr Points() noexcept {}

  constexpr Points(const double value) noexcept : value_(value) {}

  constexpr double value() const noexcept {
    return value_;
  }

  std::string print() const noexcept {
    return std::to_string((uint_least8_t)std::round(value_));
  }

  constexpr bool operator==(const Points other) const noexcept {
    return value_ == other.value_;
  }

  constexpr bool operator!=(const Points other) const noexcept {
    return value_ != other.value_;
  }

  constexpr bool operator<(const Points other) const noexcept {
    return value_ < other.value_;
  }

  constexpr bool operator<=(const Points other) const noexcept {
    return value_ <= other.value_;
  }

  constexpr bool operator>(const Points other) const noexcept {
    return value_ > other.value_;
  }

  constexpr bool operator>=(const Points other) const noexcept {
    return value_ >= other.value_;
  }

  constexpr Points operator+(const Points& other) const noexcept {
    return {value_ + other.value_};
  }

  constexpr Points operator+(const double real) const noexcept {
    return {value_ + real};
  }

  constexpr void operator+=(const Points& other) noexcept {
    value_ += other.value_;
  }

  constexpr void operator+=(const double real) noexcept {
    value_ += real;
  }

  constexpr Points operator-(const Points& other) const noexcept {
    return {value_ - other.value_};
  }

  constexpr Points operator-(const double real) const noexcept {
    return {value_ - real};
  }

  constexpr void operator-=(const Points& other) noexcept {
    value_ -= other.value_;
  }

  constexpr void operator-=(const double real) noexcept {
    value_ -= real;
  }

  constexpr Points operator*(const double real) const noexcept {
    return {value_ * real};
  }

  constexpr void operator*=(const double real) noexcept {
    value_ *= real;
  }

  constexpr Points operator/(const double real) const noexcept {
    return {value_ / real};
  }

  constexpr void operator/=(const double real) noexcept {
    value_ /= real;
  }

  struct sort_descending {
    bool operator()(const Points& points_1, const Points& points_2) const noexcept {
      return points_1.value() > points_2.value();
    }
  };

protected:

  double value_{0.0};

};

} // namespace CatanLeaderboard

namespace std {

  template <> struct hash<CatanLeaderboard::Points> {

    size_t operator()(const CatanLeaderboard::Points& points) const {
      return hash<double>()(points.value());
    }

  };

} // namespace std
