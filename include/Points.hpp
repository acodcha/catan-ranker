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

  struct sort_descending {
    bool operator()(const Points& points_1, const Points& points_2) const noexcept {
      return points_1.value() > points_2.value();
    }
  };

  struct hash {
    std::size_t operator()(const Points& points) const {
      return std::hash<double>()(points.value());
    }
  };

protected:

  double value_{0.0};

};

} // namespace CatanLeaderboard
